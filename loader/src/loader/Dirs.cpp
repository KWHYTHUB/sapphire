
#include <Sapphire/loader/Dirs.hpp>
#include <cocos2d.h>
#include <crashlog.hpp>
#include <filesystem>
#include "LoaderImpl.hpp"

using namespace sapphire::prelude;

ghc::filesystem::path dirs::getSapphireDir() {
    return dirs::getGameDir() / "sapphire";
}

ghc::filesystem::path dirs::getSapphireSaveDir() {
    return dirs::getSaveDir() / "sapphire";
}

ghc::filesystem::path dirs::getSapphireResourcesDir() {
    return dirs::getSapphireDir() / "resources";
}

ghc::filesystem::path dirs::getSapphireLogDir() {
    return dirs::getSapphireDir() / "logs";
}

ghc::filesystem::path dirs::getTempDir() {
    return getSapphireDir() / "temp";
}

ghc::filesystem::path dirs::getModsDir() {
    return getSapphireDir() / "mods";
}

ghc::filesystem::path dirs::getModsSaveDir() {
    return getSapphireSaveDir() / "mods";
}

ghc::filesystem::path dirs::getModRuntimeDir() {
    return dirs::getSapphireDir() / "unzipped";
}

ghc::filesystem::path dirs::getModConfigDir() {
    return dirs::getSapphireDir() / "config";
}

ghc::filesystem::path dirs::getIndexDir() {
    return dirs::getSapphireDir() / "index";
}

ghc::filesystem::path dirs::getCrashlogsDir() {
    return crashlog::getCrashLogDirectory();
}
