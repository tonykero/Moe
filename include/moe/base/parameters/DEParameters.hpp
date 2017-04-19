#pragma once

#include "NAParameters.hpp"

namespace moe
{

template <typename GenotypeType>
struct DEParameters : NAParameters<GenotypeType>
{
    DEParameters()
    :NAParameters<GenotypeType>()
    {
    }

    DEParameters&   withMoesPerGen( unsigned int _moesPerGen )
    {
        NAParameters<GenotypeType>::moesPerGen = _moesPerGen;
        return *this;
    }

    DEParameters&   withDimensions( unsigned int _dimensions )
    {
        NAParameters<GenotypeType>::dimensions = _dimensions;
        return *this;
    }

    DEParameters&   withRange( std::vector<GenotypeType> _range )
    {
        NAParameters<GenotypeType>::range = _range;
        return *this;
    }

    DEParameters&   withDifferentiation( float _differentiation )
    {
        differentiation = _differentiation;
        return *this;
    }

    DEParameters&   withCrossoverRate( float _crossoverRate )
    {
        crossoverRate = _crossoverRate;
        return *this;
    }

    float   differentiation = 0.9f,
            crossoverRate   = 0.5f;

};

}
