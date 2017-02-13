#pragma once
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

            crossover(selected1, selected2, offspring1, offspring2);

            // 8) Mutate offsprings

            std::bernoulli_distribution distrib_mutation( m_mutationRate );

            if( distrib_mutation( gen ) )
            {
                mutate(offspring1);
                mutate(offspring2);
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
void Moether<MoeType>::setGenotypeAscii( unsigned int _a, unsigned int _b )
{
    distrib_char = std::uniform_int_distribution<unsigned int>(_a, _b);
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

    for(unsigned int i = 0; i < m_maxGenotypeSize; i++)
        randomized += (unsigned char)distrib_char(gen);

    return randomized;
}

template <typename MoeType>
void Moether<MoeType>::crossover( MoeType& _parent1, MoeType& _parent2, MoeType& _offspring1, MoeType& _offspring2 )
{
    std::string offspring1_genotype = _parent1.getGenotype(),
                offspring2_genotype = _parent2.getGenotype();

    unsigned int min = std::min( offspring1_genotype.size(), offspring2_genotype.size() );

    std::uniform_real_distribution<float> distrib_index;
    float indexCoef;

    switch( m_crossover )
    {
        default:
        case moe::Crossover::NONE:
            // it does nothing, but NEEDED
            // in this case, _offspring1 becomes _parent1, same thing for _offspring2
            break;
        case moe::Crossover::OnePoint:
        {
            distrib_index = std::uniform_real_distribution<float>(0.05f, 0.95f);

            indexCoef = distrib_index( gen );
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
            std::bernoulli_distribution distrib_uniform = std::bernoulli_distribution( m_crossoverRate );
            for(unsigned int i = 0; i < min; i++)
            {
                if( distrib_uniform( gen ) )
                {
                    char cs = offspring1_genotype[i];
                    offspring1_genotype[i] = offspring2_genotype[i];
                    offspring2_genotype[i] = cs;
                }
            }
        }
        break;
    }

    _offspring1.setGenotype(offspring1_genotype);
    _offspring2.setGenotype(offspring2_genotype);
}

template <typename MoeType>
void Moether<MoeType>::mutate(MoeType& _moe)
{
    //TODO: assert to check m_mutation

    unsigned int choosenMutation;
    
    std::vector<unsigned int> available;
    if( m_mutation != moe::Mutation::NONE )
    {
        if( m_mutation & moe::Mutation::Substitution )
            available.push_back( moe::Mutation::Substitution );
        
        if( m_mutation & moe::Mutation::Insertion )
            available.push_back( moe::Mutation::Insertion );
        
        if( m_mutation & moe::Mutation::Deletion )
            available.push_back( moe::Mutation::Deletion );

        if( m_mutation & moe::Mutation::Translocation )
            available.push_back( moe::Mutation::Translocation );
    
        std::uniform_int_distribution<unsigned int> dist_mutation(0, available.size()-1);
        choosenMutation = available[ dist_mutation( gen ) ];
    }
    else
        choosenMutation = moe::Mutation::NONE;

    std::string moe_genotype = _moe.getGenotype();

    std::uniform_int_distribution<unsigned int> dist_genotype(0, moe_genotype.size()-1);
    switch( choosenMutation )
    {
        default:
        case moe::Mutation::NONE:
        {
            // needed
        }
        break;

        case moe::Mutation::Substitution:
        {
            char mutation = (unsigned char)distrib_char( gen );
            moe_genotype[ dist_genotype(gen) ] = mutation;   
        }
        break;
        
        case moe::Mutation::Insertion:
        {
            char mutation = (unsigned char)distrib_char( gen );
            moe_genotype.insert( moe_genotype.begin()+dist_genotype(gen), mutation );
        }
        break;
        
        case moe::Mutation::Deletion:
        {
            if(moe_genotype.size() > 2)
            {
                moe_genotype.erase( moe_genotype.begin()+ dist_genotype(gen) );
            }
        }
        break;
        
        case moe::Mutation::Translocation:
        {
            unsigned int    a = dist_genotype(gen),
                            b = dist_genotype(gen);
            char tmp = moe_genotype[a];
            moe_genotype[a] = moe_genotype[b];
            moe_genotype[b] = tmp;
        }
        break;
    }

    _moe.setGenotype( moe_genotype );
}