#include <loader/LoaderImpl.hpp>

using namespace sapphire::prelude;

#include <Sapphire/modify/CCScheduler.hpp>

struct FunctionQueue : Modify<FunctionQueue, CCScheduler> {
    void update(float dt) {
        LoaderImpl::get()->executeGDThreadQueue();
        return CCScheduler::update(dt);
    }
};
