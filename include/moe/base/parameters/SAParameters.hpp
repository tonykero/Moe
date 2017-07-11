#pragma once

#include "NAParameters.hpp"

namespace moe
{

template <typename GenotypeType>
struct SAParameters : NAParameters<GenotypeType>
{
    SAParameters()
    :NAParameters<GenotypeType>()
    {
    }

    SAParameters&   withMoesPerGen( unsigned int _moesPerGen ) = delete;

    SAParameters&   withDimensions( unsigned int _dimensions )
    {
        NAParameters<GenotypeType>::dimensions = _dimensions;
        return *this;
    }

    SAParameters&   withRange( std::vector<GenotypeType> _range )
    {
        NAParameters<GenotypeType>::range = _range;
        return *this;
    }

    SAParameters&   withTemperature( float _temperature )
    {
        temperature = _temperature;
        return *this;
    }

    SAParameters&   withCoolingRate( float _coolingRate )
    {
        coolingRate = _coolingRate;
        return *this;
    }

    SAParameters&   withRepetitions( unsigned int _repetitions )
    {
        repetitions = _repetitions;
        return *this;
    }

    float           temperature = 100.0f,
                    coolingRate;
    unsigned int    repetitions = 10;
};

}
