
#include <Sapphire/platform/platform.hpp>
#include <Sapphire/utils/general.hpp>

using namespace sapphire::prelude;

PlatformID PlatformID::from(const char* str) {
    switch (hash(str)) {
        default:
        case hash("unknown"): return PlatformID::Unknown;
        case hash("windows"): return PlatformID::Windows;
        case hash("macos"): return PlatformID::MacOS;
        case hash("ios"): return PlatformID::iOS;
        case hash("android"): return PlatformID::Android;
        case hash("linux"): return PlatformID::Linux;
    }
}

PlatformID PlatformID::from(std::string const& str) {
    return PlatformID::from(str.c_str());
}

