
#include <Sapphire/DefaultInclude.hpp>

#ifdef GEODE_IS_WINDOWS

    #include <Sapphire/loader/Mod.hpp>
    #include <Sapphire/modify/Modify.hpp>

using namespace sapphire::prelude;

// for some reason RobTop uses MessageBoxW in his GLFW error handler.
// no one knows how this is possible (he passes char* to wchar_t*).
// so anyway, here's a fix for it

static void __cdecl fixedErrorHandler(int code, char const* description) {
    log::error("GLFW Error {}: {}", code, description);
    MessageBoxA(
        nullptr,
        fmt::format(
            "GLFWError #{}: {}\nPlease contact the "
            "Sapphire Development Team for more information.",
            code,
            description
        )
            .c_str(),
        "OpenGL Error",
        MB_ICONERROR
    );
}

$execute {
    (void)Mod::get()->patch(
        reinterpret_cast<void*>(sapphire::base::getCocos() + 0x19feec), toByteArray(&fixedErrorHandler)
    );
}

#endif
