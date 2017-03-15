#pragma once

#include "AlgorithmImpl.hpp"

template <typename GenotypeType>
class GeneticAlgorithm : public Algorithm<GenotypeType>
{
    public:
        GeneticAlgorithm( unsigned int _moesPerGen, unsigned int _eliteCopies, float _mutationRate = 0.1f, float _crossoverRate = 0.5f )
        :Algorithm<GenotypeType>(),
        m_moesPerGen(_moesPerGen),
        m_eliteCopies(_eliteCopies),
        m_mutationRate(_mutationRate),
        m_crossoverRate(_crossoverRate)
        {
            
        }
        
        void run(unsigned int _generations) override
        {
            m_generations = _generations;
            
            std::vector< Moe<GenotypeType> >    population  ( m_moesPerGen );
            std::vector< double >               fitnesses   ( population.size() );
            
            // Init with a random population
            for( auto& moe : population )
                moe.genotype = this->getRandomGenotype();
            // --

            for( unsigned int i = 0; i < m_generations; i++ )
            {
                // Compute fitnesses
                double          max = 0.0;
                unsigned int    index = 0;
                
                for( unsigned int j = 0; j < population.size(); j++ )
                {
                    fitnesses[j] = this->m_fitnessFunction( population[j] );
                    population[j].fitness = fitnesses[j];
                    
                    if(max < fitnesses[j])
                    {
                        max = fitnesses[j];
                        index = j;
                    }
                }
                // --
                
                this->m_bestMoe = population[index];
                std::vector< Moe<GenotypeType> > new_population(m_eliteCopies, this->m_bestMoe);

                while(new_population.size() + 2 < m_moesPerGen)
                {
                    // random selection
                    std::uniform_int_distribution<unsigned int> dist_rnd(0, population.size()-1);
                    
                    unsigned int    a = dist_rnd( this->m_generator ),
                                    b = dist_rnd( this->m_generator );

                    Moe<GenotypeType>   selected1 = population[a],
                                        selected2 = population[b],
                                        offspring1,
                                        offspring2;

                    if(this->m_isCrossoverEnabled)
                    {
                        std::pair<std::vector<GenotypeType>, std::vector<GenotypeType>> pair;
                        pair = this->crossover( selected1, selected2 );
                        offspring1.genotype = pair.first;
                        offspring2.genotype = pair.second;
                    }

                    if(this->m_isMutationEnabled)
                    {
                        std::bernoulli_distribution dist_mutation( m_mutationRate );
                        
                        if(dist_mutation( this->m_generator ))
                            this->mutate(offspring1);
                        
                        if(dist_mutation( this->m_generator ))
                            this->mutate(offspring2);
                    }
                    
                    new_population.push_back(offspring1);
                    new_population.push_back(offspring2);
                }
                population = std::move(new_population);
            }
        }
    
    private:
        unsigned int    m_generations,
                        m_moesPerGen,
                        m_eliteCopies;

        float           m_mutationRate,
                        m_crossoverRate;
};
