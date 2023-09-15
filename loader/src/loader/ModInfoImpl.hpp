#pragma once

#include "ModMetadataImpl.hpp"

#include <Sapphire/loader/Loader.hpp>
#include <Sapphire/loader/Mod.hpp>
#include <Sapphire/utils/JsonValidation.hpp>
#include <Sapphire/utils/VersionInfo.hpp>

#pragma warning(disable : 4996) // deprecation

using namespace sapphire::prelude;

namespace sapphire {
    class [[deprecated]] ModInfo::Impl {
    public:
        ModMetadataImpl m_metadata;
        std::optional<IssuesInfo> m_issues;
        std::vector<Dependency> m_dependencies;
        bool m_supportsDisabling = true;
        bool m_supportsUnloading = false;

        bool operator==(ModInfo::Impl const& other) const;
    };

    class [[deprecated]] ModInfoImpl : public ModInfo::Impl {
    public:
        static ModInfo::Impl& getImpl(ModInfo& info);
    };
}
