#pragma once

#include <Sapphire/utils/Result.hpp>
#include <tulip/TulipHook.hpp>

namespace sapphire::modifier {
    template <uint32_t Id>
    uintptr_t address();

    Result<tulip::hook::HandlerMetadata> handlerMetadataForAddress(uintptr_t address);
}
