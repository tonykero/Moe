#pragma once
template <typename MoeType>
Moether<MoeType>::Moether()
{
    setAsciiRange(32, 255);

    registerMutation( std::make_unique< Substitution    >(gen));
    registerMutation( std::make_unique< Insertion       >(gen));
    registerMutation( std::make_unique< Deletion        >(gen));
    registerMutation( std::make_unique< Translocation   >(gen));
    registerCrossover(std::make_unique< OnePoint        >(gen));
    registerCrossover(std::make_unique< TwoPoint        >(gen));
    registerCrossover(std::make_unique< Uniform         >(gen, m_crossoverRate));
}

template <typename MoeType>
Moether<MoeType>::~Moether()
{

}

template <typename MoeType>
void Moether<MoeType>::init( unsigned int _moesPerGen, unsigned int _eliteCopies, float _mutationRate, float _crossoverRate )
{
    m_moesPerGen    = _moesPerGen;
    m_eliteCopies   = _eliteCopies;
    m_mutationRate  = _mutationRate;
    m_crossoverRate = _crossoverRate;
    
    if(m_eliteCopies & 1)
        m_eliteCopies += 1;

    
}

template <typename MoeType>
void Moether<MoeType>::run( unsigned int _generations )
{

    m_generations = _generations;

    std::vector<MoeType> population( m_moesPerGen );
    std::vector<double> fitnesses ( m_moesPerGen );
    // 1) Generate random population
    for(unsigned int i = 0; i < m_moesPerGen; i++)
    {
        population[i].setGenotype( randomizeGenotype() );
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

        // 3) keep the best as "elite"
        m_bestMoe = population[maxIndex];

        // 4) put m_eliteCopies times elite in the new generation
        std::vector<MoeType> new_population( m_eliteCopies, m_bestMoe );

        // 5) for each 2 free slots 
        while(new_population.size() + 2 <= m_moesPerGen)
        {
            // 6) Choose 2 candidates from past generation
            std::uniform_int_distribution<unsigned int> distrib_roulette(0, m_moesPerGen-1);
            
            unsigned int    a = distrib_roulette( gen ),
                            b = distrib_roulette( gen );
            MoeType selected1 = population[ a ],
                    selected2 = population[ b ];
            
            // 7) Generate 2 offsprings with crossover
            MoeType offspring1,
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
void Moether<MoeType>::setInitGenotypeSize(unsigned int _size)
{
    m_initGenotypeSize = _size;
}

template <typename MoeType>
void Moether<MoeType>::setCrossover(unsigned int _crossoverID)
{
    m_crossover = _crossoverID;
}

template <typename MoeType>
void Moether<MoeType>::setCrossoverEnabled(bool _crossoverEnabled)
{
    m_isCrossoverEnabled = _crossoverEnabled;
}

template <typename MoeType>
void Moether<MoeType>::setMutationEnabled(bool _mutationEnabled)
{
    m_isMutationsEnabled = _mutationEnabled;
}

template <typename MoeType>
const bool& Moether<MoeType>::isFitnessMode() const
{
    return m_mode;
}

template <typename MoeType>
const bool& Moether<MoeType>::isCrossoverEnabled() const
{
    return m_isCrossoverEnabled;
}

template <typename MoeType>
const bool& Moether<MoeType>::isMutationEnabled() const
{
    return m_isMutationsEnabled;
}

template <typename MoeType>
void Moether<MoeType>::registerCrossover( std::unique_ptr<Crossover> _crossover )
{
    m_crossovers.push_back( std::move(_crossover) );
}

template <typename MoeType>
void Moether<MoeType>::registerMutation( std::unique_ptr<Mutation> _mutation )
{
    m_mutations.push_back( std::move(_mutation) );
}

template <typename MoeType>
void Moether<MoeType>::setAsciiRange( unsigned int _a, unsigned int _b )
{
    m_charset = "";
    for(unsigned int i = _a; i < _b+1; i++)
    {
        m_charset += (char)i;
    }
    distrib_charset = std::uniform_int_distribution<unsigned int>(0, m_charset.size()-1);
}

template <typename MoeType>
void Moether<MoeType>::setCharset( const std::string& _charset )
{
    m_charset = _charset;
    distrib_charset = std::uniform_int_distribution<unsigned int>(0, m_charset.size()-1);
}

template <typename MoeType>
const std::string& Moether<MoeType>::getCharset() const
{
    return m_charset;
}

template <typename MoeType>
const MoeType& Moether<MoeType>::getBestMoe() const
{
    return m_bestMoe;
}

template <typename MoeType>
std::string Moether<MoeType>::randomizeGenotype()
{
    std::string randomized = "";

    for(unsigned int i = 0; i < m_initGenotypeSize; i++)
        randomized += m_charset[ distrib_charset(gen) ];

    return randomized;
}

template <typename MoeType>
void Moether<MoeType>::crossover( const MoeType& _parent1, const MoeType& _parent2, MoeType& _offspring1, MoeType& _offspring2 )
{
    std::string offspring1_genotype = _parent1.getGenotype(),
                offspring2_genotype = _parent2.getGenotype();

    std::pair<std::string, std::string> pair = m_crossovers[ m_crossover ]->cross(offspring1_genotype, offspring2_genotype);
    _offspring1.setGenotype( pair.first );
    _offspring2.setGenotype( pair.second);
}

template <typename MoeType>
void Moether<MoeType>::mutate(MoeType& _moe)
{
    std::uniform_int_distribution<unsigned int> distrib_mutations(0, m_mutations.size()-1);
    _moe.setGenotype( m_mutations[ distrib_mutations( gen ) ]->mutate( _moe.getGenotype(), m_charset ) );
}