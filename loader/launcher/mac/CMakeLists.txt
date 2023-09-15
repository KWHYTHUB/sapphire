cmake_minimum_required(VERSION 3.3.0)


add_library(SapphireBootstrapper SHARED Bootstrapper.cpp)
target_compile_features(SapphireBootstrapper PUBLIC cxx_std_17)
set_target_properties(SapphireBootstrapper PROPERTIES
	PREFIX "" 
	OUTPUT_NAME "SapphireBootstrapper"
	LIBRARY_OUTPUT_DIRECTORY "${GEODE_BIN_PATH}/nightly"
	RUNTIME_OUTPUT_DIRECTORY "${GEODE_BIN_PATH}/nightly"
)

target_link_libraries(SapphireBootstrapper PRIVATE)

add_library(FakeSapphire SHARED FakeSapphire.cpp)
target_compile_features(FakeSapphire PUBLIC cxx_std_17)
set_target_properties(FakeSapphire PROPERTIES
	PREFIX "" 
	OUTPUT_NAME "Sapphire"
)

set_target_properties(SapphireBootstrapper PROPERTIES BUILD_WITH_INSTALL_RPATH TRUE)
set_target_properties(SapphireBootstrapper PROPERTIES INSTALL_RPATH "./")

set_target_properties(FakeSapphire PROPERTIES BUILD_WITH_INSTALL_RPATH TRUE)
set_target_properties(FakeSapphire PROPERTIES INSTALL_RPATH "./")

target_link_libraries(SapphireBootstrapper PRIVATE FakeSapphire)
