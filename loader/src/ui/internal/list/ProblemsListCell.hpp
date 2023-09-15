#pragma once

#include <Sapphire/binding/TableViewCell.hpp>
#include <Sapphire/binding/FLAlertLayerProtocol.hpp>
#include <Sapphire/loader/Loader.hpp>
#include <Sapphire/loader/ModMetadata.hpp>
#include <Sapphire/loader/Index.hpp>

using namespace sapphire::prelude;

class ProblemsListPopup;

class ProblemsListCell : public CCLayer {
protected:
    float m_width;
    float m_height;
    ProblemsListPopup* m_layer;
    CCMenu* m_menu;
    LoadProblem m_problem;
    std::string m_longMessage;

    bool init(LoadProblem problem, ProblemsListPopup* list, CCSize const& size);
    void draw() override;

    void onMore(CCObject*);

    float getLogoSize() const;

public:
    LoadProblem getProblem() const;

    static ProblemsListCell* create(LoadProblem problem, ProblemsListPopup* list, CCSize const& size);
};
