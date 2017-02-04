
#include <vector>

template <typename MoeType>
Moether<MoeType>::Moether()
{
    distrib_char = std::uniform_int_distribution<unsigned int>(32, 255);
}

template <typename MoeType>
Moether<MoeType>::~Moether()
{

}

template <typename MoeType>
void Moether<MoeType>::init( unsigned int _moesPerGen, unsigned int _eliteCopies,
                    float _mutationRate, float _crossoverRate)
{
    m_moesPerGen = _moesPerGen;
    m_eliteCopies = _eliteCopies;
    m_mutationRate = _mutationRate;
    m_crossoverRate = _crossoverRate;
}

template <typename MoeType>
void Moether<MoeType>::run( unsigned int _generations )
{

    m_generations = _generations;

    // Algorithm:

    // 1) Generate random population
    //      for each generation
    // 2) Compute fitnesses
    // 3) Keep the best as "elite"
    // 4) put m_eliteCopies times elite in the new generation
    // 5) for each 2 free slots 
    // 6) Choose 2 candidates from past generation
    // 7) Generate 2 offsprings with crossover
    // 8) Mutate offsprings
    // 9) Put the 2 offsprings in the new generation
    // 10) Set the new generation as actual
    // 11) Step 2

    std::vector<MoeType> population( m_moesPerGen );
    std::vector<double> fitnesses ( m_moesPerGen );
    // 1)
    for(unsigned int i = 0; i < m_moesPerGen; i++)
    {
        population[i].setGenotype( randomizeGenotype() );
    }

    

    for(unsigned int i = 0; i < m_generations; i++)
    {
        // 2)

        double max = 0.0;
        double min = m_fitnessFunction(population[0]);
        unsigned int maxIndex = 0;
        for(unsigned int j = 0; j < m_moesPerGen; j++)
        {
            fitnesses[j] = m_fitnessFunction( population[j] );
            population[j].setFitness( fitnesses[j] );
            
            if(m_mode)
            {
                if(max < fitnesses[j])
                {
                    max = fitnesses[j];
                    maxIndex = j;
                }
            }
            else
            {
                if(min > fitnesses[j])
                {
                    min = fitnesses[j];
                    maxIndex = j;
                }
            }
                

        }

        // 3)
        m_bestMoe = population[maxIndex];


        // 4)
        std::vector<MoeType> new_population( m_eliteCopies, m_bestMoe );

        // 5)
        while(new_population.size() + 2 <= m_moesPerGen)
        {
            // 6)
            std::uniform_int_distribution<unsigned int> distrib_roulette(0, m_moesPerGen-1);
            unsigned int a = distrib_roulette( gen );
            unsigned int b = distrib_roulette( gen );
            MoeType selected1 = population[ a ];
            MoeType selected2 = population[ b ];
            
            // 7)
            // One Point, Two point and Uniform crossover done
            MoeType offspring1;
            MoeType offspring2;

            std::string offspring1_genotype = selected1.getGenotype();
            std::string offspring2_genotype = selected2.getGenotype();

            unsigned int min = std::min( offspring1_genotype.size(), offspring2_genotype.size() );

            std::uniform_real_distribution<float> distrib_index;
            float indexCoef;

            switch( m_crossover )
            {
                default:
                case moe::Crossover::OnePoint:
                    {
                        distrib_index = std::uniform_real_distribution<float>(0.05f, 0.95f);

                        indexCoef =  distrib_index( gen );
                        unsigned int index = min * indexCoef;
                    
                        offspring1_genotype = offspring1_genotype.substr(0, index)
                                        + offspring2_genotype.substr(index, offspring2_genotype.size() - index);
                    
                        offspring2_genotype = offspring2_genotype.substr(0, index)
                                        + offspring1_genotype.substr(index, offspring1_genotype.size() - index);
                    }
                    break;
                
                case moe::Crossover::TwoPoint:
                    {
                        distrib_index = std::uniform_real_distribution<float>(0.05f, 0.45f);

                        indexCoef = distrib_index( gen );
                        unsigned int index1 = min * indexCoef;
                    
                        distrib_index = std::uniform_real_distribution<float>(0.55f, 0.95f);
                        indexCoef = distrib_index( gen );

                        unsigned int index2 = min * indexCoef;

                        for(unsigned int i = index1; i < index2; i++)
                        {
                            char cs = offspring1_genotype[i];
                            offspring1_genotype[i] = offspring2_genotype[i];
                            offspring2_genotype[i] = cs;
                        }
                    }
                    break;

                case moe::Crossover::Uniform:
                    {
                        distrib_index = std::uniform_real_distribution<float>(0.0f, 1.0f);
                        
                        for(unsigned int i = 0; i < min; i++)
                        {
                            float prob = distrib_index( gen );
                            if( prob <= m_crossoverRate )
                            {
                                char cs = offspring1_genotype[i];
                                offspring1_genotype[i] = offspring2_genotype[i];
                                offspring2_genotype[i] = cs;
                            }
                        }
                    }
                    break;
            }

            offspring1.setGenotype(offspring1_genotype);
            offspring2.setGenotype(offspring2_genotype);

            // 8)
            // Mutation by substitution
            
            // TODO: Implement Insertion, Deletion, Translocation
            std::uniform_real_distribution<float> distrib_mutation(0.0f, 1.0f);

            if( distrib_mutation( gen ) <= m_mutationRate )
            {
                std::uniform_int_distribution<unsigned int> distrib_offspring1(0, offspring1.getGenotype().size());
                std::uniform_int_distribution<unsigned int> distrib_offspring2(0, offspring2.getGenotype().size());

                char mutation;

                mutation = (unsigned char)distrib_char( gen );

                offspring1_genotype[ distrib_offspring1(gen) ] = mutation;
                offspring1.setGenotype(offspring1_genotype);

                mutation = (unsigned char)distrib_char( gen );

                offspring2_genotype[ distrib_offspring2(gen) ] = mutation;
                offspring2.setGenotype(offspring2_genotype);



            }

            // 9)
            new_population.push_back( offspring1 );
            new_population.push_back( offspring2 );
        }

        // 10)
        population = new_population;
    }

}

template <typename MoeType>
void Moether<MoeType>::setFitnessFunction( std::function<double( const MoeType&)> _fitnessFunction)
{
    m_fitnessFunction = _fitnessFunction;
}

template <typename MoeType>
void Moether<MoeType>::setFitnessMode(bool _mode)
{
    m_mode = _mode;
}

template <typename MoeType>
void Moether<MoeType>::setMaxGenotypeSize(unsigned int _size)
{
    m_maxGenotypeSize = _size;
}

template <typename MoeType>
void Moether<MoeType>::setCrossover(unsigned int _type)
{
    m_crossover = _type;
}

template <typename MoeType>
void Moether<MoeType>::setMutation(unsigned int _type)
{
    m_mutation = _type;
}

template <typename MoeType>
const MoeType& Moether<MoeType>::getBestMoe() const
{
    return m_bestMoe;
}

template <typename MoeType>
std::string Moether<MoeType>::randomizeGenotype()
{
    unsigned int genotypeSize = m_maxGenotypeSize;

    std::string randomized = "";

    for(unsigned int i = 0; i < genotypeSize; i++)
    {
        randomized += (unsigned char)distrib_char(gen);
    }

    return randomized;
}