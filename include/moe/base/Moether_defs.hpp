#pragma once

template <typename GenotypeType>
Moether<GenotypeType>::Moether()
{
    registerMutation( std::make_unique< Substitution<GenotypeType>  >(gen), moe::Mutation::Substitution );
    registerMutation( std::make_unique< Insertion<GenotypeType>     >(gen), moe::Mutation::Insertion    );
    registerMutation( std::make_unique< Deletion<GenotypeType>      >(gen), moe::Mutation::Deletion     );
    registerMutation( std::make_unique< Translocation<GenotypeType> >(gen), moe::Mutation::Translocation);
    registerCrossover(std::make_unique< OnePoint<GenotypeType>      >(gen), moe::Crossover::OnePoint    );
    registerCrossover(std::make_unique< TwoPoint<GenotypeType>      >(gen), moe::Crossover::TwoPoint    );
    registerCrossover(std::make_unique< Uniform<GenotypeType>       >(gen, m_crossoverRate), moe::Crossover::Uniform);
}

template <typename GenotypeType>
Moether<GenotypeType>::~Moether()
{

}

template <typename GenotypeType>
void Moether<GenotypeType>::init( unsigned int _moesPerGen, unsigned int _eliteCopies, float _mutationRate, float _crossoverRate )
{
    m_moesPerGen    = _moesPerGen;
    m_eliteCopies   = _eliteCopies;
    m_mutationRate  = _mutationRate;
    m_crossoverRate = _crossoverRate;
    
    if(m_eliteCopies & 1)
        m_eliteCopies += 1;

    
}

template <typename GenotypeType>
void Moether<GenotypeType>::run( unsigned int _generations )
{

    m_generations = _generations;

    std::vector< Moe<GenotypeType> >    population( m_moesPerGen );
    std::vector<double>                     fitnesses ( m_moesPerGen );
    // 1) Generate random population
    for(unsigned int i = 0; i < m_moesPerGen; i++)
    {
        population[i].genotype = randomizeGenotype();
    }

    for(unsigned int i = 0; i < m_generations; i++)
    {
        // 2) Compute fitnesses

        double  max = 0.0,
                min = m_fitnessFunction(population[0]);
        
        unsigned int maxIndex = 0;
        
        for(unsigned int j = 0; j < m_moesPerGen; j++)
        {
            fitnesses[j] = m_fitnessFunction( population[j] );
            population[j].fitness = fitnesses[j];
            
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

        // 3) keep the best as "elite"
        m_bestMoe = population[maxIndex];

        // 4) put m_eliteCopies times elite in the new generation
        std::vector< Moe<GenotypeType> > new_population( m_eliteCopies, m_bestMoe );

        // 5) for each 2 free slots 
        while(new_population.size() + 2 <= m_moesPerGen)
        {
            // 6) Choose 2 candidates from past generation
            std::uniform_int_distribution<unsigned int> distrib_roulette(0, m_moesPerGen-1);
            
            unsigned int                    a = distrib_roulette( gen ),
                                            b = distrib_roulette( gen );
            Moe<GenotypeType>   selected1 = population[ a ],
                                    selected2 = population[ b ];
            
            // 7) Generate 2 offsprings with crossover
            Moe<GenotypeType>   offspring1,
                                    offspring2;

            if( m_isCrossoverEnabled )
                crossover(selected1, selected2, offspring1, offspring2);

            // 8) Mutate offsprings
            if( m_isMutationsEnabled )
            {
                std::bernoulli_distribution distrib_mutation( m_mutationRate );

                if( distrib_mutation( gen ) )
                {
                    mutate(offspring1);
                    mutate(offspring2);
                }
            }
            
            // 9) Put the 2 offsprings in the new generation
            new_population.push_back( offspring1 );
            new_population.push_back( offspring2 );
        }

        // 10) Set the new generation as actual
        population = new_population;
    }

}

template <typename GenotypeType>
void Moether<GenotypeType>::setFitnessFunction( std::function<double( const Moe<GenotypeType>&)> _fitnessFunction)
{
    m_fitnessFunction = _fitnessFunction;
}

template <typename GenotypeType>
void Moether<GenotypeType>::setFitnessMode(bool _mode)
{
    m_mode = _mode;
}

template <typename GenotypeType>
void Moether<GenotypeType>::setInitGenotypeSize(unsigned int _size)
{
    m_initGenotypeSize = _size;
}

template <typename GenotypeType>
void Moether<GenotypeType>::setCrossover(unsigned int _crossoverID)
{
    m_crossover = _crossoverID;
}

template <typename GenotypeType>
void Moether<GenotypeType>::setCrossoverEnabled(bool _crossoverEnabled)
{
    m_isCrossoverEnabled = _crossoverEnabled;
}

template <typename GenotypeType>
void Moether<GenotypeType>::setMutationEnabled(bool _mutationEnabled)
{
    m_isMutationsEnabled = _mutationEnabled;
}

template <typename GenotypeType>
const bool& Moether<GenotypeType>::isFitnessMode() const
{
    return m_mode;
}

template <typename GenotypeType>
const bool& Moether<GenotypeType>::isCrossoverEnabled() const
{
    return m_isCrossoverEnabled;
}

template <typename GenotypeType>
const bool& Moether<GenotypeType>::isMutationEnabled() const
{
    return m_isMutationsEnabled;
}

template <typename GenotypeType>
void Moether<GenotypeType>::registerCrossover( std::unique_ptr<Crossover<GenotypeType>> _crossover, unsigned int _id )
{
    m_crossovers[_id] = std::move(_crossover);
}

template <typename GenotypeType>
void Moether<GenotypeType>::registerMutation( std::unique_ptr<Mutation<GenotypeType>> _mutation, unsigned int _id )
{
    m_mutations[_id] = std::move(_mutation);
    updateKeys();
}

template <typename GenotypeType>
void Moether<GenotypeType>::unregisterCrossover( unsigned int _id )
{
    m_crossovers.erase(_id);
}

template <typename GenotypeType>
void Moether<GenotypeType>::unregisterMutation( unsigned int _id )
{
    m_mutations.erase(_id);
    updateKeys();
}

template <typename GenotypeType>
void Moether<GenotypeType>::updateKeys()
{
    m_keys.clear();
    m_keys.reserve(m_mutations.size());
    for(const auto& i : m_mutations)
        m_keys.push_back(i.first);
}

template <typename GenotypeType>
void Moether<GenotypeType>::setDataset( const std::vector<GenotypeType>& _dataset )
{
    m_dataset = _dataset;
    distrib_dataset = std::uniform_int_distribution<unsigned int>(0, m_dataset.size()-1);
}

template <typename GenotypeType>
const std::vector<GenotypeType>& Moether<GenotypeType>::getDataset() const
{
    return m_dataset;
}

template <typename GenotypeType>
const Moe<GenotypeType>& Moether<GenotypeType>::getBestMoe() const
{
    return m_bestMoe;
}

template <typename GenotypeType>
std::vector<GenotypeType> Moether<GenotypeType>::randomizeGenotype()
{
    std::vector<GenotypeType> randomized;

    for(unsigned int i = 0; i < m_initGenotypeSize; i++)
        randomized.push_back( m_dataset[ distrib_dataset(gen) ] );

    return randomized;
}

template <typename GenotypeType>
void Moether<GenotypeType>::crossover( const Moe<GenotypeType>& _parent1, const Moe<GenotypeType>& _parent2, Moe<GenotypeType>& _offspring1, Moe<GenotypeType>& _offspring2 )
{
    std::vector<GenotypeType>   offspring1_genotype = _parent1.genotype,
                                offspring2_genotype = _parent2.genotype;

    std::pair<std::vector<GenotypeType>, std::vector<GenotypeType>> pair = m_crossovers[ m_crossover ]->cross(offspring1_genotype, offspring2_genotype);
    _offspring1.genotype = pair.first;
    _offspring2.genotype = pair.second;
}

template <typename GenotypeType>
void Moether<GenotypeType>::mutate(Moe<GenotypeType>& _moe)
{
    std::uniform_int_distribution<unsigned int> distrib_mutations(0, m_keys.size()-1);
    _moe.genotype = m_mutations[ m_keys[distrib_mutations( gen )] ]->mutate( _moe.genotype, m_dataset );
}
