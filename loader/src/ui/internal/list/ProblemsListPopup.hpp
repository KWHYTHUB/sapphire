#pragma once

#include <Sapphire/ui/Popup.hpp>
#include <Sapphire/loader/Loader.hpp>

using namespace sapphire::prelude;

class ProblemsListPopup : public Popup<Mod*> {
protected:
    CCNode* m_listParent;
    ListView* m_list;

    bool setup(Mod* scrollTo) override;

    void createList(Mod* scrollTo);
    CCArray* createCells(Mod* scrollTo, float& scrollValue);
    CCSize getCellSize() const;
    CCSize getListSize() const;

public:
    static ProblemsListPopup* create(Mod* scrollTo);
};
