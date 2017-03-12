#pragma once

#ifdef _MSC_VER
    #include <algorithm> // triggers errors for std::min if not included with MSVC (19.0)
#endif

#include "base/Moether.hpp" // Moe.hpp, Mutations.hpp, Crossovers.hpp
#include "base/Moether_defs.hpp"
#include "base/util.hpp"
