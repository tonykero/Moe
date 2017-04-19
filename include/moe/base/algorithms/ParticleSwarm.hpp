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

        void run( unsigned int _generations ) override;

    private:
        unsigned int    m_generations;
        
        float           m_weight,
                        m_coef1,
                        m_coef2;
};

template <typename GenotypeType>
ParticleSwarm<GenotypeType>::ParticleSwarm( unsigned int _moesPerGen, float _weight, float _coef1, float _coef2, unsigned int _dimensions, std::vector<GenotypeType> _range )
:NumericAlgorithm<GenotypeType>( _moesPerGen, _dimensions, _range ),
m_weight    ( _weight ),
m_coef1     ( _coef1 ),
m_coef2     ( _coef2 )
{
}

template <typename GenotypeType>
ParticleSwarm<GenotypeType>::ParticleSwarm( const PSParameters<GenotypeType>& _parameters )
:ParticleSwarm<GenotypeType>( _parameters.moesPerGen, _parameters.inertia, _parameters.coef1, _parameters.coef2, _parameters.dimensions, _parameters.range )
{
    
}

template <typename GenotypeType>
void ParticleSwarm<GenotypeType>::run( unsigned int _generations )
{
    m_generations = _generations;

    std::vector< Moe<GenotypeType> >            population      ( NumericAlgorithm<GenotypeType>::m_moesPerGen );
    std::vector< Moe<GenotypeType> >            best_genotypes  ( NumericAlgorithm<GenotypeType>::m_moesPerGen );
    std::vector< std::vector<GenotypeType> >    velocities      ( NumericAlgorithm<GenotypeType>::m_moesPerGen );

    std::uniform_real_distribution<float> dist_coef     ( 0.0f, 1.0f );

    //TODO: getRandomGenotype() -> getRandomVector()
    //TODO: getRandomVector( range );

    double max = 0.0;
    unsigned int    index = 0,
                    count = 0;
    
    for( auto& moe : population )
    {
        moe.genotype            = NumericAlgorithm<GenotypeType>::getRandomGenotype();
        moe.fitness             = Algorithm<GenotypeType>::m_fitnessFunction( moe );
        
        velocities[count]       = NumericAlgorithm<GenotypeType>::getRandomGenotype();
        
        best_genotypes[count].genotype  = moe.genotype;
        best_genotypes[count].fitness   = moe.fitness;
        if( max < moe.fitness )
        {
            max = moe.fitness;
            index = count;
        }
        count++;
    }
    
    Algorithm<GenotypeType>::m_bestMoe = population[index];

    for( unsigned int i = 0; i < m_generations; i++ )
    {
        for( unsigned int j = 0; j < population.size(); j++ )
        {
            for( unsigned int k = 0; k < NumericAlgorithm<GenotypeType>::m_dimensions; k++ )
            {
                float   r1 = dist_coef( Algorithm<GenotypeType>::m_generator ),
                        r2 = dist_coef( Algorithm<GenotypeType>::m_generator );

                velocities[j][k] *= m_weight;
                velocities[j][k] += m_coef1*r1*( best_genotypes[j].genotype[k] - population[j].genotype[k] );
                velocities[j][k] += m_coef2*r2*( Algorithm<GenotypeType>::m_bestMoe.genotype[k] - population[j].genotype[k] );
                
                population[j].genotype[k] += velocities[j][k];
            }
            population[j].fitness = Algorithm<GenotypeType>::m_fitnessFunction( population[j] );
            if( population[j].fitness > best_genotypes[j].fitness )
            {
                best_genotypes[j] = population[j];
                if( best_genotypes[j].fitness > Algorithm<GenotypeType>::m_bestMoe.fitness )
                    Algorithm<GenotypeType>::m_bestMoe = best_genotypes[j];
            }
        }
    }

}

}
