#pragma once

#include "NumericAlgorithmImpl.hpp"
#include "../parameters/PSParameters.hpp"

namespace moe
{

template <typename GenotypeType>
class ParticleSwarm : public NumericAlgorithm<GenotypeType>
{
    public:
        ParticleSwarm( unsigned int _moesPerGen, float _weight, float _coef1, float _coef2, unsigned int _dimensions = 1, std::vector<GenotypeType> _range = { std::numeric_limits<GenotypeType>::lowest() , std::numeric_limits<GenotypeType>::max() });
        ParticleSwarm( const PSParameters<GenotypeType>& _parameters );

        void run( unsigned int _iterations ) override;

    protected:
        void init( unsigned int _iterations ) override;

    private:
        unsigned int    m_iterations;
        
        float           m_weight,
                        m_coef1,
                        m_coef2;

        std::vector< Moe<GenotypeType> >            m_population;
        std::vector< Moe<GenotypeType> >            m_best_genotypes;
        std::vector< std::vector<GenotypeType> >    m_velocities;
        std::uniform_real_distribution<float>       m_dist_coef;
};

template <typename GenotypeType>
ParticleSwarm<GenotypeType>::ParticleSwarm( unsigned int _moesPerGen, float _weight, float _coef1, float _coef2, unsigned int _dimensions, std::vector<GenotypeType> _range )
:NumericAlgorithm<GenotypeType>( _moesPerGen, _dimensions, _range ),
m_weight        ( _weight       ),
m_coef1         ( _coef1        ),
m_coef2         ( _coef2        ),
m_population    ( _moesPerGen   ),
m_best_genotypes(_moesPerGen    ),
m_velocities    (_moesPerGen    ),
m_dist_coef     ( 0.0f, 1.0f    )
{
}

template <typename GenotypeType>
ParticleSwarm<GenotypeType>::ParticleSwarm( const PSParameters<GenotypeType>& _parameters )
:ParticleSwarm<GenotypeType>( _parameters.moesPerGen, _parameters.inertia, _parameters.coef1, _parameters.coef2, _parameters.dimensions, _parameters.range )
{
    
}

template <typename GenotypeType>
void ParticleSwarm<GenotypeType>::init( unsigned int _iterations )
{
    m_iterations = _iterations;

    double max = 0.0;
    unsigned int    index = 0,
                    count = 0;
    
    for( auto& moe : m_population )
    {
        moe.genotype            = NumericAlgorithm<GenotypeType>::getRandomGenotype();
        moe.fitness             = Algorithm<GenotypeType>::m_fitnessFunction( moe );
        
        m_velocities[count]       = NumericAlgorithm<GenotypeType>::getRandomGenotype();
        
        m_best_genotypes[count].genotype  = moe.genotype;
        m_best_genotypes[count].fitness   = moe.fitness;
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
void ParticleSwarm<GenotypeType>::run( unsigned int _iterations )
{
    this->init( _iterations );

    for( unsigned int i = 0; i < m_iterations; i++ )
    {
        for( unsigned int j = 0; j < m_population.size(); j++ )
        {
            for( unsigned int k = 0; k < NumericAlgorithm<GenotypeType>::m_dimensions; k++ )
            {
                float   r1 = m_dist_coef( Algorithm<GenotypeType>::m_generator ),
                        r2 = m_dist_coef( Algorithm<GenotypeType>::m_generator );

                m_velocities[j][k] *= m_weight;
                m_velocities[j][k] += m_coef1*r1*( m_best_genotypes[j].genotype[k] - m_population[j].genotype[k] );
                m_velocities[j][k] += m_coef2*r2*( Algorithm<GenotypeType>::m_bestMoe.genotype[k] - m_population[j].genotype[k] );
                
                m_population[j].genotype[k] += m_velocities[j][k];
                
                //checks if genotype actual dimension is still in provided search space
                if( m_population[j].genotype[k] != std::max( std::min( m_population[j].genotype[k], NumericAlgorithm<GenotypeType>::m_range[1]), NumericAlgorithm<GenotypeType>::m_range[0] ) )
                {
                    m_population[j].genotype = NumericAlgorithm<GenotypeType>::getRandomGenotype();
                    m_velocities[j] = NumericAlgorithm<GenotypeType>::getRandomGenotype();
                    break;
                }
            }

            m_population[j].fitness = Algorithm<GenotypeType>::m_fitnessFunction( m_population[j] );
            if( m_population[j].fitness > m_best_genotypes[j].fitness )
            {
                m_best_genotypes[j] = m_population[j];
                if( m_best_genotypes[j].fitness > Algorithm<GenotypeType>::m_bestMoe.fitness )
                    Algorithm<GenotypeType>::m_bestMoe = m_best_genotypes[j];
            }
        }
    }

}

}
