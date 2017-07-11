#pragma once

#include "NumericAlgorithmImpl.hpp"
#include "../parameters/SAParameters.hpp"

namespace moe
{

template <typename GenotypeType>
class SimulatedAnnealing : public NumericAlgorithm<GenotypeType>
{
    public:
        SimulatedAnnealing( float _temperature, float _coolingRate, unsigned int _repetitions, unsigned int _dimensions = 1, std::vector<GenotypeType> _range = { std::numeric_limits<GenotypeType>::lowest() , std::numeric_limits<GenotypeType>::max() } );
        SimulatedAnnealing( const SAParameters<GenotypeType>& _parameters );

        void run( unsigned int _generations ) override;

    private:
        unsigned int    m_generations;
        
        float           m_temperature,
                        m_coolingRate,
                        m_absoluteZero = 1.0f;
        unsigned int    m_repetitions;
};

template <typename GenotypeType>
SimulatedAnnealing<GenotypeType>::SimulatedAnnealing( float _temperature, float _coolingRate, unsigned int _repetitions, unsigned int _dimensions, std::vector<GenotypeType> _range )
:NumericAlgorithm<GenotypeType>( 1 , _dimensions, _range),
m_temperature(_temperature),
m_coolingRate(_coolingRate),
m_repetitions(_repetitions)
{
}

template <typename GenotypeType>
SimulatedAnnealing<GenotypeType>::SimulatedAnnealing( const SAParameters<GenotypeType>& _parameters )
:SimulatedAnnealing<GenotypeType>(_parameters.temperature, _parameters.coolingRate, _parameters.repetitions, _parameters.dimensions, _parameters.range)
{
}

template <typename GenotypeType>
void SimulatedAnnealing<GenotypeType>::run( unsigned int _generations )
{
    m_generations = _generations;

    auto initial_candidate = NumericAlgorithm<GenotypeType>::getRandomGenotype();
    double initial_fitness, max;

    Moe<GenotypeType> bestMoe;
    bestMoe.genotype    = initial_candidate;
    bestMoe.fitness     = Algorithm<GenotypeType>::m_fitnessFunction( bestMoe );
    initial_fitness = max = bestMoe.fitness;

    for(unsigned int i = 0; i < m_generations; i++)
    {
        while(m_temperature > m_absoluteZero)
        {
            Moe<GenotypeType> candidate;
            candidate.genotype = NumericAlgorithm<GenotypeType>::getRandomGenotype();
            double fitness = Algorithm<GenotypeType>::m_fitnessFunction( candidate );

            double delta = fitness - initial_fitness;

            std::uniform_real_distribution<double> normal_dist(0, 1);
            double rdm_normal = normal_dist(Algorithm<GenotypeType>::m_generator);

            if( delta > 0 || (std::exp( delta/m_temperature )) > rdm_normal )
            {
                initial_fitness     = fitness;
                initial_candidate   = candidate.genotype;

                if(initial_fitness > max)
                {
                    bestMoe.genotype    = initial_candidate;
                    bestMoe.fitness     = initial_fitness;
                    max                 = initial_fitness;
                }
            }
            m_temperature *= m_coolingRate;
        }
    }
    Algorithm<GenotypeType>::m_bestMoe = bestMoe;
}

}
