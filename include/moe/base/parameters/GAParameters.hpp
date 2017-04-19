#pragma once

#include <vector>

namespace moe
{

template <typename GenotypeType>
struct GAParameters
{
    GAParameters&   withMoesPerGen( unsigned int _moesPerGen )
    {
        moesPerGen = _moesPerGen;
        return *this;
    }

    GAParameters&   withEliteCopies( unsigned int _eliteCopies )
    {
        eliteCopies = _eliteCopies;
        return *this;
    }

    GAParameters&   withMutationRate( float _mutationRate )
    {
        mutationRate = _mutationRate;
        return *this;
    }

    GAParameters&   withCrossoverRate( float _crossoverRate )
    {
        crossoverRate = _crossoverRate;
        return *this;
    }

    GAParameters&   withDataset( std::vector<GenotypeType> _dataset )
    {
        dataset = _dataset;
        return *this;
    }

    unsigned int    moesPerGen,
                    eliteCopies= 0;
    
    float           mutationRate = 0.1f,
                    crossoverRate = 0.5f;

    std::vector<GenotypeType>   dataset;
};

}
