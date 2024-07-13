#pragma once

#include <Sapphire/binding/CCContentLayer.hpp>
#include <Sapphire/binding/CCScrollLayerExt.hpp>

namespace sapphire {
    /**
     * CCContentLayer expects all of its children
     * to be TableViewCells, which is not ideal for
     * a generic content layer
     */
    class GEODE_DLL GenericContentLayer : public CCContentLayer {
    public:
        static GenericContentLayer* create(float width, float height);

        void setPosition(cocos2d::CCPoint const& pos) override;
    };

    class GEODE_DLL ScrollLayer : public CCScrollLayerExt {
    protected:
        bool m_scrollWheelEnabled;

        ScrollLayer(cocos2d::CCRect const& rect, bool scrollWheelEnabled, bool vertical);

        bool ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*) override;

    public:
        static ScrollLayer* create(
            cocos2d::CCRect const& rect, bool scrollWheelEnabled = true, bool vertical = true
        );
        static ScrollLayer* create(
            cocos2d::CCSize const& size, bool scrollWheelEnabled = true, bool vertical = true
        );

        void scrollWheel(float y, float) override;
        void enableScrollWheel(bool enable = true);
    };
}