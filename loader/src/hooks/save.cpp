#include <Sapphire/loader/Loader.hpp>

using namespace sapphire::prelude;

#include <Sapphire/modify/AppDelegate.hpp>

struct SaveLoader : Modify<SaveLoader, AppDelegate> {
    void trySaveGame() {
        log::info("Saving...");

        auto r = Loader::get()->saveData();
        if (!r) {
            log::info("{}", r.unwrapErr());
        }

        log::info("Saved");

        return AppDelegate::trySaveGame();
    }
};
