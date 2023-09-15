#pragma once

// Because C++ doesn't like using
// namespaces that don't exist
namespace sapphire {
    namespace addresser {}
    namespace cast {}
    namespace cocos {}
    namespace utils {}
    namespace helper {}
    namespace op {}
    namespace stream {}
    namespace view {}
}

namespace cocos2d {
    namespace extension {}
}

namespace sapphire {
    namespace prelude {
        using namespace ::sapphire;
        using namespace ::sapphire::addresser;
        using namespace ::sapphire::cast;
        using namespace ::sapphire::cocos;
        using namespace ::sapphire::helper;
        using namespace ::sapphire::utils;
        using namespace ::sapphire::op;
        using namespace ::sapphire::stream;
        using namespace ::sapphire::view;
        using namespace ::cocos2d;
        using namespace ::cocos2d::extension;
    }

    namespace [[deprecated("Use `using namespace sapphire::prelude` instead!")]] old_prelude {
        using namespace ::sapphire::prelude;
    }
}

#define USE_GEODE_NAMESPACE() using namespace sapphire::old_prelude;