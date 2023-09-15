cmake_minimum_required(VERSION 3.21 FATAL_ERROR)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build libraries static" FORCE)

# Docs flags
if (SAPPHIRE_BUILDING_DOCS)
	set(SAPPHIRE_DISABLE_CLI_CALLS On)
	set(CMAKE_EXPORT_COMPILE_COMMANDS On)
	set(SAPPHIRE_DISABLE_PRECOMPILED_HEADERS On)
endif()

# Read version
file(READ VERSION SAPPHIRE_VERSION)
string(STRIP "${SAPPHIRE_VERSION}" SAPPHIRE_VERSION)

# Check if version has a tag like v1.0.0-alpha
string(FIND ${SAPPHIRE_VERSION} "-" SAPPHIRE_VERSION_HAS_TAG)
if (NOT ${SAPPHIRE_VERSION_HAS_TAG} EQUAL "-1")
	string(REGEX MATCH "[a-z]+(\.[0-9]+)?$" SAPPHIRE_VERSION_TAG ${SAPPHIRE_VERSION})
	string(SUBSTRING "${SAPPHIRE_VERSION}" 0 ${SAPPHIRE_VERSION_HAS_TAG} SAPPHIRE_VERSION)
	string(FIND ${SAPPHIRE_VERSION_TAG} "." SAPPHIRE_VERSION_TAG_HAS_NUMBER)

	# Extract tag type and number from tag
	if (NOT ${SAPPHIRE_VERSION_TAG_HAS_NUMBER} EQUAL "-1")
		string(SUBSTRING "${SAPPHIRE_VERSION_TAG}" 0 ${SAPPHIRE_VERSION_TAG_HAS_NUMBER} SAPPHIRE_VERSION_TAG_TYPE)
		math(EXPR SAPPHIRE_VERSION_TAG_HAS_NUMBER "${SAPPHIRE_VERSION_TAG_HAS_NUMBER} + 1")
		string(SUBSTRING "${SAPPHIRE_VERSION_TAG}" ${SAPPHIRE_VERSION_TAG_HAS_NUMBER} -1 SAPPHIRE_VERSION_TAG_NUMBER)
	else()
		set(SAPPHIRE_VERSION_TAG_TYPE "${SAPPHIRE_VERSION_TAG}")
		set(SAPPHIRE_VERSION_TAG_NUMBER "")
	endif()

	# Capitalize first letter of tag type
	string(SUBSTRING ${SAPPHIRE_VERSION_TAG_TYPE} 0 1 FIRST_LETTER)
	string(TOUPPER ${FIRST_LETTER} FIRST_LETTER)
	string(REGEX REPLACE "^.(.*)" "${FIRST_LETTER}\\1" SAPPHIRE_VERSION_TAG_TYPE "${SAPPHIRE_VERSION_TAG_TYPE}")

	message(STATUS "Version: ${SAPPHIRE_VERSION}, tag: ${SAPPHIRE_VERSION_TAG} (type: ${SAPPHIRE_VERSION_TAG_TYPE}, number: ${SAPPHIRE_VERSION_TAG_NUMBER})")
else()
	set(SAPPHIRE_VERSION_TAG "")
	set(SAPPHIRE_VERSION_TAG_TYPE "")
	set(SAPPHIRE_VERSION_TAG_NUMBER "")

	message(STATUS "Version: ${SAPPHIRE_VERSION}")
endif()

project(sapphire VERSION ${SAPPHIRE_VERSION} LANGUAGES CXX C)

add_library(${PROJECT_NAME} INTERFACE)

if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
	set(SAPPHIRE_ALWAYS_BUILD_CODEGEN ON)
endif()

if (NOT DEFINED SAPPHIRE_DEBUG AND (CMAKE_BUILD_TYPE STREQUAL Debug OR CMAKE_BUILD_TYPE STREQUAL RelWithDebInfo))
	set(SAPPHIRE_DEBUG ON)
endif()

# define it in command line or your project system
if (SAPPHIRE_DEBUG) 
	target_compile_definitions(${PROJECT_NAME} INTERFACE -DSAPPHIRE_DEBUG)
endif()

# Rerun CMake on VERSION file change
set_target_properties(${PROJECT_NAME} PROPERTIES CMAKE_CONFIGURE_DEPENDS VERSION)

target_compile_definitions(${PROJECT_NAME} INTERFACE -DPROJECT_NAME=${CMAKE_PROJECT_NAME})

set(SAPPHIRE_CODEGEN_PATH ${CMAKE_CURRENT_BINARY_DIR}/codegenned)
set(SAPPHIRE_BIN_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(SAPPHIRE_LOADER_PATH ${CMAKE_CURRENT_SOURCE_DIR}/loader)
set(SAPPHIRE_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR})

include(cmake/SapphireFile.cmake)
include(cmake/Platform.cmake)
include(cmake/CPM.cmake)

if (PROJECT_IS_TOP_LEVEL AND NOT SAPPHIRE_BUILDING_DOCS)
	set(MAT_JSON_AS_INTERFACE ON)
endif()
CPMAddPackage("gh:KWHYTHUB/json#a47f570")
CPMAddPackage("gh:fmtlib/fmt#9.1.0")
CPMAddPackage("gh:gulrak/filesystem#3e5b930")

target_compile_definitions(${PROJECT_NAME} INTERFACE MAT_JSON_DYNAMIC=1)

# this is needed for cross compilation on linux,
# since fmtlib will fail to compile otherwise
if (SAPPHIRE_DISABLE_FMT_CONSTEVAL)
	message(VERBOSE "Disabling FMT_CONSTEVAL")
	target_compile_definitions(fmt PUBLIC -DFMT_CONSTEVAL=)
endif()

# Tulip hook (hooking)
if (PROJECT_IS_TOP_LEVEL AND NOT SAPPHIRE_BUILDING_DOCS)
	set(TULIP_LINK_SOURCE ON)
endif()
set(CMAKE_WARN_DEPRECATED OFF CACHE BOOL "" FORCE)
CPMAddPackage("gh:KWHYTHUB/TulipHook#3423a29")
set(CMAKE_WARN_DEPRECATED ON CACHE BOOL "" FORCE)

# Silence warnings from dependencies
include(CheckCXXCompilerFlag)
check_cxx_compiler_flag(-Wno-everything SUPPORTS_W_NO_EVERYTHING)
if (SUPPORTS_W_NO_EVERYTHING)
	if (TARGET capstone)
		target_compile_options(capstone PRIVATE -Wno-everything)
	endif()
endif()

target_sources(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/entry.cpp)

# Filesystem implementation in a separate target because i couldnt disable pch
add_library(SapphireFilesystemImpl ${CMAKE_CURRENT_SOURCE_DIR}/FilesystemImpl.cpp)
target_compile_features(SapphireFilesystemImpl PUBLIC cxx_std_20)
target_link_libraries(SapphireFilesystemImpl PUBLIC ghc_filesystem)

include(ExternalProject)
set(SAPPHIRE_CODEGEN_BINARY_OUT ${CMAKE_CURRENT_BINARY_DIR}/codegen)
ExternalProject_Add(CodegenProject
	BUILD_ALWAYS ON
	SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/codegen
	# manually set configure command as to not inherit generator used by sapphire,
	# this should hopefully fix generator cache mismatch between different projects, however
	# it causes a warning to be shown every time. if you know a better solution please tell us ok thx
	CONFIGURE_COMMAND ${CMAKE_COMMAND} ${SAPPHIRE_CODEGEN_CMAKE_ARGS} -DCMAKE_INSTALL_PREFIX:STRING=${SAPPHIRE_CODEGEN_BINARY_OUT}
		-DCMAKE_C_COMPILER_LAUNCHER=${CMAKE_C_COMPILER_LAUNCHER} -DCMAKE_CXX_COMPILER_LAUNCHER=${CMAKE_CXX_COMPILER_LAUNCHER}
		-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
		-S <SOURCE_DIR> -B <BINARY_DIR>
	BUILD_COMMAND ${CMAKE_COMMAND} --build <BINARY_DIR> --config $<CONFIGURATION>
	INSTALL_COMMAND ${CMAKE_COMMAND} --install <BINARY_DIR> --config $<CONFIGURATION>
)


file(GLOB CODEGEN_DEPENDS CONFIGURE_DEPENDS 
	${CMAKE_CURRENT_SOURCE_DIR}/bindings/*.bro
	${CMAKE_CURRENT_SOURCE_DIR}/codegen/src/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/codegen/src/*.hpp
)

if (NOT SAPPHIRE_BINDINGS_PATH)
	set(SAPPHIRE_BINDINGS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bindings)
endif()

file(GLOB CODEGEN_OUTPUTS CONFIGURE_DEPENDS 
	${SAPPHIRE_CODEGEN_PATH}/Sapphire/binding/*.hpp
)

add_custom_command(
	DEPENDS ${CODEGEN_DEPENDS}
	DEPENDS CodegenProject
	COMMAND ${SAPPHIRE_CODEGEN_BINARY_OUT}/Codegen ${SAPPHIRE_TARGET_PLATFORM} ${SAPPHIRE_BINDINGS_PATH} ${SAPPHIRE_CODEGEN_PATH}
	COMMAND echo codegen > ${SAPPHIRE_CODEGEN_PATH}/.stamp
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	COMMENT "Run Codegen"
	OUTPUT ${SAPPHIRE_CODEGEN_PATH}/Sapphire/GeneratedSource.cpp ${SAPPHIRE_CODEGEN_PATH}/Sapphire/GeneratedAddress.cpp ${SAPPHIRE_CODEGEN_PATH}/.stamp ${CODEGEN_OUTPUTS}
)

add_custom_target(CodegenRun
	DEPENDS ${SAPPHIRE_CODEGEN_PATH}/.stamp
)

add_dependencies(${PROJECT_NAME} CodegenRun)

# Hacky way to supress the not generated error
if (NOT EXISTS ${SAPPHIRE_CODEGEN_PATH}/Sapphire/GeneratedSource.cpp)
	make_directory(${SAPPHIRE_CODEGEN_PATH})
	make_directory(${SAPPHIRE_CODEGEN_PATH}/Sapphire)
	file(TOUCH ${SAPPHIRE_CODEGEN_PATH}/Sapphire/GeneratedSource.cpp)
endif()

if (NOT EXISTS ${SAPPHIRE_CODEGEN_PATH}/Sapphire/GeneratedAddress.cpp)
	make_directory(${SAPPHIRE_CODEGEN_PATH})
	make_directory(${SAPPHIRE_CODEGEN_PATH}/Sapphire)
	file(TOUCH ${SAPPHIRE_CODEGEN_PATH}/Sapphire/GeneratedAddress.cpp)
endif()

add_library(SapphireCodegenSources ${SAPPHIRE_CODEGEN_PATH}/Sapphire/GeneratedSource.cpp ${SAPPHIRE_CODEGEN_PATH}/Sapphire/GeneratedAddress.cpp)
target_link_directories(SapphireCodegenSources PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/loader/include/link)
target_link_libraries(SapphireCodegenSources PRIVATE ghc_filesystem SapphireFilesystemImpl fmt TulipHookInclude mat-json)
target_include_directories(SapphireCodegenSources PRIVATE
	${SAPPHIRE_CODEGEN_PATH}
	${SAPPHIRE_LOADER_PATH}/include
	${SAPPHIRE_LOADER_PATH}/include/Sapphire/cocos/include
	${SAPPHIRE_LOADER_PATH}/include/Sapphire/cocos/extensions
	${SAPPHIRE_LOADER_PATH}/include/Sapphire/fmod
)
target_compile_features(SapphireCodegenSources PUBLIC cxx_std_20)
if (NOT SAPPHIRE_DISABLE_PRECOMPILED_HEADERS)
	target_precompile_headers(SapphireCodegenSources INTERFACE
		"${SAPPHIRE_LOADER_PATH}/include/Sapphire/Bindings.hpp"
	)
endif()

target_include_directories(${PROJECT_NAME} INTERFACE
	${SAPPHIRE_CODEGEN_PATH}
	${SAPPHIRE_LOADER_PATH}/include
	${SAPPHIRE_LOADER_PATH}/include/Sapphire/cocos/include
	${SAPPHIRE_LOADER_PATH}/include/Sapphire/cocos/extensions
	${SAPPHIRE_LOADER_PATH}/include/Sapphire/fmod
)
target_link_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/loader/include/link)

target_link_libraries(${PROJECT_NAME} INTERFACE ghc_filesystem fmt TulipHookInclude SapphireCodegenSources mat-json SapphireFilesystemImpl)


if (NOT EXISTS ${SAPPHIRE_BIN_PATH})
	make_directory(${SAPPHIRE_BIN_PATH})
endif()

if (NOT EXISTS ${SAPPHIRE_BIN_PATH}/${PROJECT_VERSION} AND EXISTS ${SAPPHIRE_BIN_PATH}/nightly/)
	set(SAPPHIRE_LINK_NIGHTLY 1)
endif()

if (${SAPPHIRE_LINK_NIGHTLY})
	set(SAPPHIRE_PLATFORM_BIN_PATH ${SAPPHIRE_BIN_PATH}/nightly/${SAPPHIRE_PLATFORM_BINARY})
else()
	set(SAPPHIRE_PLATFORM_BIN_PATH ${SAPPHIRE_BIN_PATH}/${PROJECT_VERSION}/${SAPPHIRE_PLATFORM_BINARY})
endif()


if (PROJECT_IS_TOP_LEVEL)
	add_subdirectory(loader)
	target_link_libraries(${PROJECT_NAME} INTERFACE sapphire-loader)
elseif(EXISTS ${SAPPHIRE_PLATFORM_BIN_PATH})
	target_link_libraries(${PROJECT_NAME} INTERFACE "${SAPPHIRE_PLATFORM_BIN_PATH}")
	target_precompile_headers(${PROJECT_NAME} INTERFACE
		"${SAPPHIRE_LOADER_PATH}/include/Sapphire/DefaultInclude.hpp"
		"${SAPPHIRE_LOADER_PATH}/include/Sapphire/Sapphire.hpp"
		# please stop adding modify here its not here because it makes windows compilation take longer than sapphire 1.0 release date
	)
else()
	message(FATAL_ERROR
		"No valid loader binary to link to! Install prebuilts with `sapphire sdk install-binaries`, "
		"or build Sapphire from source and add `set(SAPPHIRE_LINK_NIGHTLY ON)` to your CMakeLists.txt "
		"in the line before calling add_subdirectory for Sapphire."
	)
endif()
