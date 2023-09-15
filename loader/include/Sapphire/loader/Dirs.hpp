#pragma once

#include <ghc/fs_fwd.hpp>
#include "../DefaultInclude.hpp"

namespace sapphire::dirs {
    /**
     * Directory where Geometry Dash is
     */
    GEODE_DLL ghc::filesystem::path getGameDir();
    /**
     * Directory where GD saves its files
     */
    GEODE_DLL ghc::filesystem::path getSaveDir();
    /**
     * Directory where Sapphire is
     */
    GEODE_DLL ghc::filesystem::path getSapphireDir();
    /**
     * Directory where Sapphire saves its files
     */
    GEODE_DLL ghc::filesystem::path getSapphireSaveDir();
    /**
     * Directory where Sapphire's resources are stored
     */
    GEODE_DLL ghc::filesystem::path getSapphireResourcesDir();
    /**
     * Directory where Sapphire's resources are stored
     */
    GEODE_DLL ghc::filesystem::path getSapphireLogDir();
    /**
     * Directory to store temporary files
     */
    GEODE_DLL ghc::filesystem::path getTempDir();
    /**
     * Directory where mods are stored by default
     */
    GEODE_DLL ghc::filesystem::path getModsDir();
    /**
     * Directory where mods' save data is stored
     */
    GEODE_DLL ghc::filesystem::path getModsSaveDir();
    /**
     * Directory where mods' unzipped packages are stored at runtime
     */
    GEODE_DLL ghc::filesystem::path getModRuntimeDir();
    /**
     * Directory where mods' config files lie
     */
    GEODE_DLL ghc::filesystem::path getModConfigDir();
    /**
     * Directory where Sapphire stores the cached index
     */
    GEODE_DLL ghc::filesystem::path getIndexDir();
    /**
     * Directory where crashlogs are stored
     */
    GEODE_DLL ghc::filesystem::path getCrashlogsDir();
}
