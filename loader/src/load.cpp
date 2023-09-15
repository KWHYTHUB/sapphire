#include "loader/LoaderImpl.hpp"

#include <Sapphire/loader/IPC.hpp>
#include <Sapphire/loader/Loader.hpp>
#include <Sapphire/loader/Log.hpp>
#include <Sapphire/loader/Mod.hpp>
#include <Sapphire/loader/SettingEvent.hpp>
#include <Sapphire/loader/ModJsonTest.hpp>
#include <Sapphire/utils/JsonValidation.hpp>

#include <array>

using namespace sapphire::prelude;

#include "load.hpp"

$execute {
    listenForSettingChanges("show-platform-console", +[](bool value) {
        if (value) {
            Loader::get()->openPlatformConsole();
        }
        else {
            Loader::get()->closePlatformConsole();
        }
    });
    
    listenForIPC("ipc-test", [](IPCEvent* event) -> json::Value {
        return "Hello from Sapphire!";
    });

    listenForIPC("loader-info", [](IPCEvent* event) -> json::Value {
        return Mod::get()->getMetadata();
    });

    listenForIPC("list-mods", [](IPCEvent* event) -> json::Value {
        std::vector<json::Value> res;

        auto args = *event->messageData;
        JsonChecker checker(args);
        auto root = checker.root("").obj();

        auto includeRunTimeInfo = root.has("include-runtime-info").template get<bool>();
        auto dontIncludeLoader = root.has("dont-include-loader").template get<bool>();

        if (!dontIncludeLoader) {
            res.push_back(
                includeRunTimeInfo ? Mod::get()->getRuntimeInfo() :
                                     Mod::get()->getMetadata().toJSON()
            );
        }

        for (auto& mod : Loader::get()->getAllMods()) {
            res.push_back(includeRunTimeInfo ? mod->getRuntimeInfo() : mod->getMetadata().toJSON());
        }

        return res;
    });
}

int sapphireEntry(void* platformData) {
    // set up internal mod, settings and data
    auto internalSetupRes = LoaderImpl::get()->setupInternalMod();
    if (!internalSetupRes) {
        LoaderImpl::get()->platformMessageBox(
            "Unable to Load Sapphire!",
            "There was a fatal error setting up "
            "the internal mod and Sapphire can not be loaded: " + internalSetupRes.unwrapErr()
        );
        LoaderImpl::get()->forceReset();
        return 1;
    }

    // open console
    if (Mod::get()->getSettingValue<bool>("show-platform-console")) {
        Loader::get()->openPlatformConsole();
    }

    // set up loader, load mods, etc.
    auto setupRes = LoaderImpl::get()->setup();
    if (!setupRes) {
        LoaderImpl::get()->platformMessageBox(
            "Unable to Load Sapphire!",
            "There was an unknown fatal error setting up "
            "the loader and Sapphire can not be loaded. " 
            "(" + setupRes.unwrapErr() + ")"
        );
        LoaderImpl::get()->forceReset();
        return 1;
    }

    log::info("Set up loader");

    // download and install new loader update in the background
    if (Mod::get()->getSettingValue<bool>("auto-check-updates")) {
        LoaderImpl::get()->checkForLoaderUpdates();
    }

    log::debug("Entry done.");

    return 0;
}
