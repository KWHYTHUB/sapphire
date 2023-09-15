#include <Sapphire/loader/Loader.hpp>
#include <Sapphire/modify/LoadingLayer.hpp>
#include <Sapphire/modify/GameManager.hpp>

using namespace sapphire::prelude;

struct ResourcesUpdate : Modify<ResourcesUpdate, LoadingLayer> {
    void loadAssets() {
        LoadingLayer::loadAssets();
        // this is in case the user refreshes texture quality at runtime
        if (m_loadStep == 10) {
            Loader::get()->updateResources(true);
        }
    }
};
