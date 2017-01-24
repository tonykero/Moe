
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
                    float _mutationRate, float _crossoverRate )
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
    
    //TODO: Moether::run()

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
        population[i].setGenome( randomizeGenome() );
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
            //One point crossover
            MoeType offspring1;
            offspring1.setGenome(selected1.getGenome().substr(0, selected1.getGenome().size()/2)
                                +selected2.getGenome().substr(selected1.getGenome().size()/2, selected2.getGenome().size() - selected1.getGenome().size()/2));
            
            MoeType offspring2;
            offspring2.setGenome(selected2.getGenome().substr(0, selected2.getGenome().size()/2)
                                +selected1.getGenome().substr(selected2.getGenome().size()/2, selected1.getGenome().size() - selected2.getGenome().size()/2));
                                
            // 8)
            std::uniform_real_distribution<float> distrib_mutation(0.0f, 1.0f);

            if( distrib_mutation( gen ) <= m_mutationRate )
            {
                std::uniform_int_distribution<unsigned int> distrib_offspring1(0, offspring1.getGenome().size());
                std::uniform_int_distribution<unsigned int> distrib_offspring2(0, offspring2.getGenome().size());

                char mutation;

                mutation = (unsigned char)distrib_char( gen );

                std::string offspring1_genome = offspring1.getGenome();
                offspring1_genome[ distrib_offspring1(gen) ] = mutation;
                offspring1.setGenome(offspring1_genome);


                mutation = (unsigned char)distrib_char( gen );

                std::string offspring2_genome = offspring2.getGenome();
                offspring2_genome[ distrib_offspring2(gen) ] = mutation;
                offspring2.setGenome(offspring2_genome);



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
void Moether<MoeType>::setMaxGenomeSize(int _size)
{
    m_maxGenomeSize = _size;
}

template <typename MoeType>
const MoeType& Moether<MoeType>::getBestMoe() const
{
    return m_bestMoe;
}

template <typename MoeType>
std::string Moether<MoeType>::randomizeGenome()
{
    unsigned int genomeSize = m_maxGenomeSize;

    std::string randomized = "";

    for(unsigned int i = 0; i < genomeSize; i++)
    {
        randomized += (unsigned char)distrib_char(gen);
    }

    return randomized;
}