#pragma once

#include <algorithm> // std::transform
#include <numeric> // std::iota

#include "NumericAlgorithmImpl.hpp"
#include "../parameters/DEParameters.hpp"

namespace moe
{

template <typename GenotypeType>
class DifferentialEvolution : public NumericAlgorithm<GenotypeType>
{
    public:
        DifferentialEvolution(unsigned int _moesPerGen, float _differentiation = 0.9f ,float _crossoverRate = 0.5f, unsigned int _dimensions = 1, std::vector<GenotypeType> _range = { std::numeric_limits<GenotypeType>::lowest() , std::numeric_limits<GenotypeType>::max() } );
        DifferentialEvolution( const DEParameters<GenotypeType>& _parameters );


        void    run( unsigned int _iterations ) override;

    protected:
        void    init(unsigned int _iterations) override;

    private:
        unsigned int    m_iterations;
                        
        float           m_differentiation,
                        m_crossoverRate;

        std::vector< Moe<GenotypeType> >    m_population;

        std::uniform_real_distribution<>    dist_genotype;
};

template <typename GenotypeType>
DifferentialEvolution<GenotypeType>::DifferentialEvolution(unsigned int _moesPerGen, float _differentiation, float _crossoverRate, unsigned int _dimensions, std::vector<GenotypeType> _range )
:NumericAlgorithm<GenotypeType>( _moesPerGen, _dimensions, _range ),
m_differentiation   ( _differentiation  ),
m_crossoverRate     ( _crossoverRate    ),
m_population(_moesPerGen)
{

}

template <typename GenotypeType>
DifferentialEvolution<GenotypeType>::DifferentialEvolution( const DEParameters<GenotypeType>& _parameters )
:DifferentialEvolution<GenotypeType>( _parameters.moesPerGen, _parameters.differentiation, _parameters.crossoverRate, _parameters.dimensions, _parameters.range )
{
}

template<typename GenotypeType>
void DifferentialEvolution<GenotypeType>::init( unsigned int _iterations )
{
    m_iterations = _iterations;

    double          max = 0.0;
    unsigned int    index = 0,
                    count = 0;

    for( auto& moe : m_population )
    {
        moe.genotype = NumericAlgorithm<GenotypeType>::getRandomGenotype();
        moe.fitness = NumericAlgorithm<GenotypeType>::m_fitnessFunction( moe );
        if( max < moe.fitness )
        {
            max = moe.fitness;
            index = count;
        }
        count++;
    }

    Algorithm<GenotypeType>::m_bestMoe = m_population[index];

}

template <typename GenotypeType>
void DifferentialEvolution<GenotypeType>::run( unsigned int _iterations )
{
    this->init(_iterations);

    for( unsigned int i = 0; i < m_iterations; i++ )
    {
        for(unsigned int j = 0; j < m_population.size(); j++ )
        {
            /// -- Algorithm initialization
            std::vector<unsigned int> pickableIDs(m_population.size());
            std::iota(pickableIDs.begin(), pickableIDs.end(), 0); // fills from 0 to end() 0,1,2...,n
            
            // we remove the ID of the actual element so
            // actual != a != b != c
            pickableIDs.erase( pickableIDs.begin() + j );
            
            unsigned int    max = pickableIDs.size()-1;
            
            // returns a random index, which won't be able to be choosen next time
            auto random_index = [this, &max, &pickableIDs]() -> unsigned int
            {
                std::uniform_int_distribution<unsigned int> dist( 0, max );
                
                unsigned int    pick = dist( Algorithm<GenotypeType>::m_generator ),
                                ret = pickableIDs[pick];
                
                pickableIDs.erase( pickableIDs.begin() + pick );
                max--;
                
                return ret;
            };
            
            Moe<GenotypeType>   actual  = m_population[j],
                                a       = m_population[ random_index() ],
                                b       = m_population[ random_index() ],
                                c       = m_population[ random_index() ],
                                candidate = actual;
            /// --

            /// -- Beginning of Algorithm
            std::uniform_int_distribution<unsigned int> dist_dim( 1, NumericAlgorithm<GenotypeType>::m_dimensions );
            unsigned int r = dist_dim( Algorithm<GenotypeType>::m_generator );

            for( unsigned int k = 0; k < NumericAlgorithm<GenotypeType>::m_dimensions; k++ )
            {
                std::bernoulli_distribution dist_crossover( m_crossoverRate );

                if( dist_crossover( Algorithm<GenotypeType>::m_generator ) || ( k +1 == r ) )
                    candidate.genotype[k] = a.genotype[k] + m_differentiation * ( b.genotype[k] - c.genotype[k] );
                else
                    candidate.genotype[k] = actual.genotype[k];
                
                // checks if genotype is still in provided search space
                if( candidate.genotype[k] != std::max( std::min( candidate.genotype[k], NumericAlgorithm<GenotypeType>::m_range[1]), NumericAlgorithm<GenotypeType>::m_range[0] ) )
                {
                    candidate.genotype = NumericAlgorithm<GenotypeType>::getRandomGenotype();
                    break;
                }
            }
            /// --

            /// -- Selection
            candidate.fitness   = Algorithm<GenotypeType>::m_fitnessFunction( candidate );
            
            // local selection
            if( candidate.fitness > actual.fitness )
            {
                m_population[j] = std::move( candidate );
                
                // global selection
                if( Algorithm<GenotypeType>::m_bestMoe.fitness < m_population[j].fitness )
                    Algorithm<GenotypeType>::m_bestMoe = m_population[j];
                /// --
            }
        }
    }
}

}
