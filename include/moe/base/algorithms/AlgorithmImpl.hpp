#pragma once

#include <random> // distributions & engine
#include <functional> // std::function

#ifdef USE_TIMESEED
#include <chrono>
#endif

#include <moe/base/Moe.hpp>
#include <moe/base/Mutations.hpp>
#include <moe/base/Crossovers.hpp>

template <typename GenotypeType>
class Algorithm
{
    public:
        Algorithm();
        virtual ~Algorithm();

        virtual void                        run                 ( unsigned int _iterations ) = 0;
        
        void                                setFitnessFunction  ( std::function< double( const Moe<GenotypeType>& ) > _fitnessFunction );

        virtual std::vector<GenotypeType>   getRandomGenotype   () = 0;

        const Moe<GenotypeType>&            getBestMoe          () const;
    
    protected:
        virtual void                        init                ( unsigned int _iterations ) = 0;

        std::function< double( const Moe<GenotypeType>& ) > m_fitnessFunction;
                        
        Moe<GenotypeType>           m_bestMoe;
        std::default_random_engine  m_generator;
        
};

template <typename GenotypeType>
Algorithm<GenotypeType>::Algorithm()
#ifdef USE_TIMESEED
:m_generator( std::chrono::high_resolution_clock::now().time_since_epoch().count() )
#endif
{
}

template <typename GenotypeType>
Algorithm<GenotypeType>::~Algorithm()
{
    
}

template <typename GenotypeType>
void Algorithm<GenotypeType>::setFitnessFunction( std::function< double(const Moe<GenotypeType>& )> _fitnessFunction )
{
    m_fitnessFunction = _fitnessFunction;
}

template <typename GenotypeType>
const Moe<GenotypeType>& Algorithm<GenotypeType>::getBestMoe() const
{
    return m_bestMoe;
}
