
#include <Sapphire/loader/Tulip.hpp>

tulip::hook::Result<void*> sapphire::hook::createWrapper(
    void* address,
    tulip::hook::WrapperMetadata const& metadata
) noexcept {
    return tulip::hook::createWrapper(address, metadata);
}

std::shared_ptr<tulip::hook::CallingConvention> sapphire::hook::createConvention(
    tulip::hook::TulipConvention convention
) noexcept {
    return tulip::hook::createConvention(convention);
}
