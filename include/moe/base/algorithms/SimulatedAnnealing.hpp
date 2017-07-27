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

        void run( unsigned int _iterations ) override;

    protected:
        void init( unsigned int _iterations ) override;

    private:
        unsigned int    m_iterations;
        
        float           m_temperature,
                        m_coolingRate,
                        m_absoluteZero = 1.0f;
        unsigned int    m_repetitions;

        std::vector<GenotypeType>   m_initial_candidate;
        double                      m_initial_fitness;
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
void SimulatedAnnealing<GenotypeType>::init( unsigned int _iterations )
{
    m_iterations = _iterations;

    m_initial_candidate = NumericAlgorithm<GenotypeType>::getRandomGenotype();

    Algorithm<GenotypeType>::m_bestMoe.genotype    = m_initial_candidate;
    Algorithm<GenotypeType>::m_bestMoe.fitness     = Algorithm<GenotypeType>::m_fitnessFunction( Algorithm<GenotypeType>::m_bestMoe );
    m_initial_fitness = Algorithm<GenotypeType>::m_bestMoe.fitness;
}

template <typename GenotypeType>
void SimulatedAnnealing<GenotypeType>::run( unsigned int _iterations )
{

    this->init(_iterations);

    for(unsigned int i = 0; i < m_iterations; i++)
    {
        Moe<GenotypeType> candidate;
        candidate.genotype = NumericAlgorithm<GenotypeType>::getRandomGenotype();
        
        while(m_temperature > m_absoluteZero)
        {
            double fitness = Algorithm<GenotypeType>::m_fitnessFunction( candidate );

            double delta = fitness - m_initial_fitness;

            std::uniform_real_distribution<double> dist(0, 1);
            double rdm_normal = dist(Algorithm<GenotypeType>::m_generator);

            if( delta > 0 || (std::exp( delta/m_temperature )) > rdm_normal )
            {
                m_initial_fitness     = fitness;
                m_initial_candidate   = candidate.genotype;

                if(m_initial_fitness > Algorithm<GenotypeType>::m_bestMoe.fitness)
                {
                    Algorithm<GenotypeType>::m_bestMoe.genotype    = m_initial_candidate;
                    Algorithm<GenotypeType>::m_bestMoe.fitness     = m_initial_fitness;
                }
            }

            for(unsigned int j = 0; j < NumericAlgorithm<GenotypeType>::m_dimensions; j++)
            {
                dist = std::uniform_real_distribution<double>(-1, 1);
                candidate.genotype[j] += dist(Algorithm<GenotypeType>::m_generator);
            }
            m_temperature *= m_coolingRate;
        }
    }
}

}
