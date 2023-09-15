#include <Sapphire/ui/SceneManager.hpp>

using namespace sapphire::prelude;

#include <Sapphire/modify/AchievementNotifier.hpp>

struct SceneSwitch : Modify<SceneSwitch, AchievementNotifier> {
    void willSwitchToScene(CCScene* scene) {
        AchievementNotifier::willSwitchToScene(scene);
        SceneManager::get()->willSwitchToScene(scene);
    }
};
