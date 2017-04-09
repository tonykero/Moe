#pragma once

#ifdef _MSC_VER
    #include <algorithm> // triggers errors for std::min if not included with MSVC (19.0)
#endif

#include "base/util.hpp"
#include "base/algorithms/GeneticAlgorithm.hpp" // Moe.hpp, Mutations.hpp, Crossovers.hpp
#include "base/algorithms/DifferentialEvolution.hpp"
#include "base/algorithms/ParticleSwarm.hpp"