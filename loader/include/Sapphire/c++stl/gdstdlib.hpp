#pragma once

#include <Sapphire/platform/platform.hpp>

#if defined(GEODE_IS_WINDOWS)
#include "msvcstl.hpp"
#else
#include "gnustl.hpp"
#endif