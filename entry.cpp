
#include <Sapphire/loader/Loader.hpp>
#include <Sapphire/loader/Mod.hpp>

namespace sapphire {
    /**
     * To bypass the need for cyclic dependencies,
     * this function does the exact same as Mod::get()
     * However, it can be externed, unlike Mod::get()
     * @returns Same thing Mod::get() returns
     */
    Mod* getMod() {
        return Mod::get();
    }
}

namespace {
    // to make sure the instance is set into the sharedMod<> in load time
    static auto mod = sapphire::getMod(); 
}
