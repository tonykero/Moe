#pragma once

#include <type_traits> // std::is_arithmetic
#include <algorithm> // std::transform
#include <limits> // std::numeric_limits
#include <numeric> // std::iota

#include "AlgorithmImpl.hpp"

template <typename GenotypeType>
class DifferentialEvolution : public Algorithm<GenotypeType>
{
    public:
        DifferentialEvolution(unsigned int _moesPerGen, float _differentiation = 0.9f ,float _crossoverRate = 0.5f, unsigned int _dimensions = 1 );
        
        void run( unsigned int _generations ) override;

        std::vector<GenotypeType> getRandomGenotype() override;

    private:
        unsigned int    m_generations,
                        m_moesPerGen,
                        m_dimensions;
                        
        float           m_differentiation,
                        m_crossoverRate;

        std::uniform_real_distribution<>    dist_genotype;
};

template <typename GenotypeType>
DifferentialEvolution<GenotypeType>::DifferentialEvolution(unsigned int _moesPerGen, float _differentiation, float _crossoverRate, unsigned int _dimensions )
:Algorithm<GenotypeType>(),
m_moesPerGen        ( _moesPerGen       ),
m_differentiation   ( _differentiation  ),
m_crossoverRate     ( _crossoverRate    ),
m_dimensions        ( _dimensions       )
{
    static_assert( std::is_arithmetic<GenotypeType>::value, "DifferentialEvolution only works with artihmetic types" );
    dist_genotype = std::uniform_real_distribution<>( std::numeric_limits<GenotypeType>::lowest(), std::numeric_limits<GenotypeType>::max() );
}

template <typename GenotypeType>
void DifferentialEvolution<GenotypeType>::run( unsigned int _generations )
{
    m_generations = _generations;
    
    std::vector< Moe<GenotypeType> >    population  ( m_moesPerGen );
    
    double max = 0.0;
    unsigned int    index = 0,
                    count = 0;

    for( auto& moe : population )
    {
        moe.genotype = getRandomGenotype();
        moe.fitness = this->m_fitnessFunction( moe );
        if( max < moe.fitness )
        {
            max = moe.fitness;
            index = count;
        }
        count++;
    }

    for( unsigned int i = 0; i < m_generations; i++ )
    {
        for(unsigned int j = 0; j < population.size(); j++ )
        {
            std::vector<unsigned int> pickableIDs(population.size());
            std::iota(pickableIDs.begin(), pickableIDs.end(), 0); // fills from 0 to end() 0,1,2...,n
            
            // we remove the ID of the actual element so
            // actual != a != b != c
            pickableIDs.erase( pickableIDs.begin() + j );
            
            unsigned int    max = pickableIDs.size()-1;
            
            // returns a random index, which won't be able to be choosen next time
            auto random_index = [this, &max, &pickableIDs]() -> unsigned int
            {
                std::uniform_int_distribution<unsigned int> dist( 0, max );
                
                unsigned int    pick = dist( this->m_generator ),
                                ret = pickableIDs[pick];
                
                pickableIDs.erase( pickableIDs.begin() + pick );
                max--;
                
                return ret;
            };
            
            Moe<GenotypeType>   actual  = population[j],
                                a       = population[ random_index() ],
                                b       = population[ random_index() ],
                                c       = population[ random_index() ],
                                candidate = actual;
                                
            std::uniform_int_distribution<unsigned int> dist_dim( 1, m_dimensions );
            unsigned int r = dist_dim( this->m_generator );

            for( unsigned int k = 0; k <= m_dimensions-1; k++ )
            {
                std::bernoulli_distribution dist_crossover( m_crossoverRate );

                if( dist_crossover( this->m_generator ) || ( k +1 == r ) )
                    candidate.genotype[k] = a.genotype[k] + m_differentiation * ( b.genotype[k] - c.genotype[k] );
                else
                    candidate.genotype[k] = actual.genotype[k];
            }
            candidate.fitness   = this->m_fitnessFunction( candidate );
            
            if( candidate.fitness > actual.fitness )
                population[j] = std::move( candidate );
        
        }
    }

    max = 0.0;
    index = 0;
    count = 0;

    for(const auto& moe : population )
    {
        if( max < moe.fitness )
        {
            max = moe.fitness;
            index = count;
        }
        count++;
    }

    this->m_bestMoe = population[index];
}

template <typename GenotypeType>
std::vector<GenotypeType> DifferentialEvolution<GenotypeType>::getRandomGenotype()
{
    std::vector<GenotypeType> genotype; 
    genotype.reserve( m_dimensions );

    while( genotype.size() < m_dimensions )
        genotype.push_back( dist_genotype( this->m_generator ) );

    return genotype;
}
