#pragma once

#include <Sapphire/loader/SettingNode.hpp>
#include <Sapphire/ui/Popup.hpp>
#include <Sapphire/utils/cocos.hpp>

using namespace sapphire::prelude;

class ModSettingsPopup : public Popup<Mod*>, public SettingNodeDelegate {
protected:
    Mod* m_mod;
    std::vector<SettingNode*> m_settings;
    CCMenuItemSpriteExtra* m_applyBtn;
    ButtonSprite* m_applyBtnSpr;

    void settingValueChanged(SettingNode*) override;
    void settingValueCommitted(SettingNode*) override;

    bool setup(Mod* mod) override;
    bool hasUncommitted() const;
    void onClose(CCObject*) override;
    void onApply(CCObject*);
    void onResetAll(CCObject*);
    void onOpenSaveDirectory(CCObject*);

public:
    static ModSettingsPopup* create(Mod* mod);
};
