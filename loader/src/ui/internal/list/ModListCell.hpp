#pragma once

#include <Sapphire/binding/TableViewCell.hpp>
#include <Sapphire/binding/FLAlertLayerProtocol.hpp>
#include <Sapphire/loader/Loader.hpp>
#include <Sapphire/loader/ModMetadata.hpp>
#include <Sapphire/loader/Index.hpp>

using namespace sapphire::prelude;

class ModListLayer;
enum class ModListDisplay;

/**
 * Base class for mod list items
 */
class ModListCell : public CCLayer {
protected:
    float m_width;
    float m_height;
    ModListLayer* m_layer;
    CCMenu* m_menu;
    CCLabelBMFont* m_description;
    CCMenuItemToggler* m_enableToggle = nullptr;
    CCMenuItemSpriteExtra* m_unresolvedExMark;
    CCMenuItemSpriteExtra* m_developerBtn;

    bool init(ModListLayer* list, CCSize const& size);
    void setupInfo(ModMetadata const& metadata, bool spaceForTags, ModListDisplay display, bool inactive);
    void draw() override;

    float getLogoSize() const;
    void onViewDev(CCObject*);

public:
    virtual void updateState() = 0;
    virtual CCNode* createLogo(CCSize const& size) = 0;
    virtual std::string getDeveloper() const = 0;
    
    void disableDeveloperButton();
};

/**
 * Mod list item for a mod
 */
class ModCell : public ModListCell {
protected:
    Mod* m_mod;

    bool init(
        Mod* mod,
        ModListLayer* list,
        ModListDisplay display,
        CCSize const& size
    );

    void onInfo(CCObject*);
    void onRestart(CCObject*);
    void onEnable(CCObject*);
    void onUnresolvedInfo(CCObject*);

public:
    static ModCell* create(
        Mod* mod,
        ModListLayer* list,
        ModListDisplay display,
        CCSize const& size
    );

    void updateState() override;
    CCNode* createLogo(CCSize const& size) override;
    std::string getDeveloper() const override;
};

/**
 * Mod list item for an index item
 */
class IndexItemCell : public ModListCell {
protected:
    IndexItemHandle m_item;

    bool init(
        IndexItemHandle item,
        ModListLayer* list,
        ModListDisplay display,
        CCSize const& size
    );

    void onInfo(CCObject*);
    void onRestart(CCObject*);

public:
    static IndexItemCell* create(
        IndexItemHandle item,
        ModListLayer* list,
        ModListDisplay display,
        CCSize const& size
    );

    void updateState() override;
    CCNode* createLogo(CCSize const& size) override;
    std::string getDeveloper() const override;
};

/**
 * Mod list item for an invalid Sapphire package
 */
class InvalidSapphireFileCell : public ModListCell, public FLAlertLayerProtocol {
protected:
    InvalidSapphireFile m_info;

    bool init(
        InvalidSapphireFile const& file,
        ModListLayer* list,
        ModListDisplay display,
        CCSize const& size
    );

    void onInfo(CCObject*);
    void FLAlert_Clicked(FLAlertLayer*, bool btn2) override;

public:
    static InvalidSapphireFileCell* create(
        InvalidSapphireFile const& file,
        ModListLayer* list,
        ModListDisplay display,
        CCSize const& size
    );

    void updateState() override;
    CCNode* createLogo(CCSize const& size) override;
    std::string getDeveloper() const override;
};

/**
 * Mod list item for an invalid Sapphire package
 */
class ProblemsCell : public ModListCell {
protected:
    std::optional<ccColor3B> m_color;

    bool init(
        ModListLayer* list,
        ModListDisplay display,
        CCSize const& size
    );

    void onInfo(CCObject*);

public:
    static ProblemsCell* create(
        ModListLayer* list,
        ModListDisplay display,
        CCSize const& size
    );

    std::optional<ccColor3B> getColor();

    void updateState() override;
    CCNode* createLogo(CCSize const& size) override;
    std::string getDeveloper() const override;
};
