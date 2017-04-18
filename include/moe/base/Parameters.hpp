#pragma once

#include <type_traits>
#include <limits>
#include <vector>

template <typename GenotypeType>
struct NAParameters
{
    NAParameters()
    {
        static_assert( std::is_arithmetic<GenotypeType>::value, "NAParameters only works with arithmetic types" );
    }

    unsigned int    moesPerGen,
                    dimensions = 1;

    std::vector<GenotypeType> range{    std::numeric_limits<GenotypeType>::lowest(),
                                        std::numeric_limits<GenotypeType>::max() };

};

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

template <typename GenotypeType>
struct GAParameters
{
    GAParameters&   with(  )
    {
        return *this;
    }

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
