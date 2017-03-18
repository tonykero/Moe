#pragma once

#include <random> // distributions & engine
#include <functional> // std::function

#include <moe/base/Moe.hpp>
#include <moe/base/Mutations.hpp>
#include <moe/base/Crossovers.hpp>

template <typename GenotypeType>
class Algorithm
{
    public:
        Algorithm();
        virtual ~Algorithm();

        virtual void                        run                 ( unsigned int _generations ) = 0;
        
        void                                setFitnessFunction  ( std::function< double( const Moe<GenotypeType>& ) > _fitnessFunction );

        virtual std::vector<GenotypeType>   getRandomGenotype   () = 0;

        const Moe<GenotypeType>&            getBestMoe          () const;
    
    protected:
        std::function< double( const Moe<GenotypeType>& ) > m_fitnessFunction;
                        
        Moe<GenotypeType>           m_bestMoe;
        std::default_random_engine  m_generator;
        
};

template <typename GenotypeType>
Algorithm<GenotypeType>::Algorithm()
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