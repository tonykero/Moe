#pragma once

#include "NAParameters.hpp"

namespace moe
{

template <typename GenotypeType>
struct PSParameters : NAParameters<GenotypeType>
{
    PSParameters()
    :NAParameters<GenotypeType>()
    {
    }

    PSParameters&   withMoesPerGen( unsigned int _moesPerGen )
    {
        NAParameters<GenotypeType>::moesPerGen = _moesPerGen;
        return *this;
    }

    PSParameters&   withDimensions( unsigned int _dimensions )
    {
        NAParameters<GenotypeType>::dimensions = _dimensions;
        return *this;
    }

    PSParameters&   withRange( std::vector<GenotypeType> _range )
    {
        NAParameters<GenotypeType>::range = _range;
        return *this;
    }

    PSParameters&   withInertia( float _inertia )
    {
        inertia = _inertia;
        return *this;
    }

    PSParameters&   withCoef1( float _coef1 )
    {
        coef1 = _coef1;
        return *this;
    }

    PSParameters&   withCoef2( float _coef2 )
    {
        coef2 = _coef2;
        return *this;
    }

    float   inertia = 0.6f,
            coef1   = 0.8f,
            coef2   = 1.2f;
};

}
