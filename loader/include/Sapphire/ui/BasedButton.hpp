#pragma once

#include "BasedButtonSprite.hpp"

#include <Sapphire/binding/CCMenuItemToggler.hpp>

#pragma warning(disable : 4275)

namespace sapphire {
    class GEODE_DLL TabButton : public CCMenuItemToggler {
    public:
        static TabButton* create(
            TabBaseColor unselected, TabBaseColor selected, char const* text,
            cocos2d::CCObject* target, cocos2d::SEL_MenuHandler callback
        );

        static TabButton* create(
            char const* text, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler callback
        );
    };
}
