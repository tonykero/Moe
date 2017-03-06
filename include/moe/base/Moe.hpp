#pragma once

#include <vector>

template <typename GenotypeType>
struct Moe
{
    std::vector<GenotypeType>   genotype;
    double                      fitness;
};
