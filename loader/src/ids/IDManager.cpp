#include <Sapphire/modify/IDManager.hpp>

using namespace sapphire;

NodeIDs* NodeIDs::get() {
    static auto inst = new NodeIDs;
    return inst;
}
