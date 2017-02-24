#pragma once

#include <functional>
#include <vector>

template <typename GenotypeType>
struct Moe
{
    std::vector<GenotypeType>   genotype;
    double                      fitness;
};
