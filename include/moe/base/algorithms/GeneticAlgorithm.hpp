#pragma once

#include <memory> // std::unique_ptr
#include <unordered_map> // register functions

#include "AlgorithmImpl.hpp"

template <typename GenotypeType>
class GeneticAlgorithm : public Algorithm<GenotypeType>
{
    public:
        GeneticAlgorithm( unsigned int _moesPerGen, std::vector<GenotypeType> _dataset, unsigned int _eliteCopies = 0, float _mutationRate = 0.1f, float _crossoverRate = 0.5f );
        
        void                                run                 ( unsigned int _generations ) override;
        
        void                                setFixedSize        ( unsigned int _size );
        
        void                                setCrossover        ( unsigned int _crossoverID );
        void                                enableCrossover     ( bool );
        void                                enableMutation      ( bool );

        bool                                isCrossoverEnabled  () const;
        bool                                isMutationEnabled   () const;

        void                                registerCrossover   ( std::unique_ptr< Crossover<GenotypeType>  >, unsigned int _id);
        void                                registerMutation    ( std::unique_ptr< Mutation<GenotypeType> > , unsigned int _id);
        void                                unregisterCrossover ( unsigned int _id );
        void                                unregisterMutation  ( unsigned int _id );

        void                                setDataset          ( std::vector<GenotypeType> _dataset );
        const std::vector<GenotypeType>&    getDataset          () const;

        std::vector<GenotypeType>           getRandomGenotype   ();

    private:
        void                                            updateMutationsKeys ();
        std::pair<std::vector<GenotypeType>, std::vector<GenotypeType>> crossover           ( const Moe<GenotypeType>& _parent1, const Moe<GenotypeType>& _parent2 );
        void                                            mutate              ( Moe<GenotypeType>& _moe );
        
        unsigned int    m_generations,
                        m_moesPerGen,
                        m_eliteCopies;
        unsigned int    m_fixedSize             = 8;
        unsigned int    m_crossover             = moe::Crossover::OnePoint;

        float           m_mutationRate,
                        m_crossoverRate;

        bool            m_isCrossoverEnabled    = true,
                        m_isMutationEnabled     = true;

        std::unordered_map< unsigned int, std::unique_ptr< Mutation<GenotypeType> > > m_mutations;
        std::unordered_map< unsigned int, std::unique_ptr< Crossover<GenotypeType> >> m_crossovers;
        std::vector<unsigned int>   m_keys; // mutations IDs

        std::vector<GenotypeType>   m_dataset;
        std::uniform_int_distribution<unsigned int> dist_dataset;
};


template <typename GenotypeType>
GeneticAlgorithm<GenotypeType>::GeneticAlgorithm( unsigned int _moesPerGen, std::vector<GenotypeType> _dataset, unsigned int _eliteCopies, float _mutationRate, float _crossoverRate )
:Algorithm<GenotypeType>(),
m_moesPerGen    ( _moesPerGen       ),
m_dataset       ( _dataset          ),
m_eliteCopies   ( _eliteCopies      ),
m_mutationRate  ( _mutationRate     ),
m_crossoverRate ( _crossoverRate    )
{
    registerMutation( std::make_unique< Substitution<GenotypeType>  >(this->m_generator), moe::Mutation::Substitution );
    registerMutation( std::make_unique< Insertion<GenotypeType>     >(this->m_generator), moe::Mutation::Insertion    );
    registerMutation( std::make_unique< Deletion<GenotypeType>      >(this->m_generator), moe::Mutation::Deletion     );
    registerMutation( std::make_unique< Translocation<GenotypeType> >(this->m_generator), moe::Mutation::Translocation);
    registerCrossover(std::make_unique< OnePoint<GenotypeType>      >(this->m_generator), moe::Crossover::OnePoint    );
    registerCrossover(std::make_unique< TwoPoint<GenotypeType>      >(this->m_generator), moe::Crossover::TwoPoint    );
    registerCrossover(std::make_unique< Uniform<GenotypeType>       >(this->m_generator, m_crossoverRate), moe::Crossover::Uniform);
    
    dist_dataset = std::uniform_int_distribution<unsigned int>( 0, m_dataset.size()-1 );
}

template <typename GenotypeType>
void GeneticAlgorithm<GenotypeType>::run( unsigned int _generations )
{
    m_generations = _generations;
            
    std::vector< Moe<GenotypeType> >    population  ( m_moesPerGen );
    std::vector< double >               fitnesses   ( population.size() );
            
    // Init with a random population
    for( auto& moe : population )
        moe.genotype = getRandomGenotype();
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

            if(m_isCrossoverEnabled)
            {
                std::pair<std::vector<GenotypeType>, std::vector<GenotypeType>> pair;
                pair = crossover( selected1, selected2 );
                offspring1.genotype = pair.first;
                offspring2.genotype = pair.second;
            }

            if(m_isMutationEnabled)
            {
                std::bernoulli_distribution dist_mutation( m_mutationRate );
                        
                if(dist_mutation( this->m_generator ))
                    mutate(offspring1);
                        
                if(dist_mutation( this->m_generator ))
                    mutate(offspring2);
            }
                    
            new_population.push_back(offspring1);
            new_population.push_back(offspring2);
        }
        population = std::move(new_population);
    }
}

template <typename GenotypeType>
void GeneticAlgorithm<GenotypeType>::setFixedSize( unsigned int _size )
{
    m_fixedSize = _size;
    
    unregisterMutation( moe::Mutation::Insertion );
    unregisterMutation( moe::Mutation::Deletion );
}

template <typename GenotypeType>
void GeneticAlgorithm<GenotypeType>::setCrossover( unsigned int _crossoverID )
{
    m_crossover = _crossoverID;
}

template <typename GenotypeType>
void GeneticAlgorithm<GenotypeType>::enableCrossover( bool _enable )
{
    m_isCrossoverEnabled = _enable;
}

template <typename GenotypeType>
void GeneticAlgorithm<GenotypeType>::enableMutation( bool _enable )
{
    m_isMutationEnabled = _enable;
}

template <typename GenotypeType>
bool GeneticAlgorithm<GenotypeType>::isCrossoverEnabled() const
{
    return m_isCrossoverEnabled;
}

template <typename GenotypeType>
bool GeneticAlgorithm<GenotypeType>::isMutationEnabled() const
{
    return m_isMutationEnabled;
}

template <typename GenotypeType>
void GeneticAlgorithm<GenotypeType>::registerCrossover( std::unique_ptr< Crossover<GenotypeType> > _crossover, unsigned int _id )
{
    m_crossovers[_id] = std::move( _crossover );
}

template <typename GenotypeType>
void GeneticAlgorithm<GenotypeType>::registerMutation( std::unique_ptr< Mutation<GenotypeType> > _mutation, unsigned int _id )
{
    m_mutations[_id] = std::move( _mutation );
    updateMutationsKeys();
}

template <typename GenotypeType>
void GeneticAlgorithm<GenotypeType>::unregisterCrossover( unsigned int _id )
{
    m_crossovers.erase(_id);
}

template <typename GenotypeType>
void GeneticAlgorithm<GenotypeType>::unregisterMutation( unsigned int _id )
{
    m_mutations.erase(_id);
    updateMutationsKeys();
}

template <typename GenotypeType>
void GeneticAlgorithm<GenotypeType>::setDataset( std::vector<GenotypeType> _dataset )
{
    m_dataset = _dataset;
    dist_dataset = std::uniform_int_distribution<unsigned int>( 0, m_dataset.size()-1 );
}

template <typename GenotypeType>
const std::vector<GenotypeType>& GeneticAlgorithm<GenotypeType>::getDataset() const
{
    return m_dataset;
}

template <typename GenotypeType>
void GeneticAlgorithm<GenotypeType>::updateMutationsKeys()
{
    m_keys.clear();
    m_keys.reserve(m_mutations.size());
    for(const auto& i : m_mutations)
        m_keys.push_back(i.first);
}

template <typename GenotypeType>
std::pair<std::vector<GenotypeType>, std::vector<GenotypeType>> GeneticAlgorithm<GenotypeType>::crossover( const Moe<GenotypeType>& _parent1, const Moe<GenotypeType>& _parent2 )
{
    return m_crossovers[ m_crossover ]->cross(_parent1.genotype, _parent2.genotype);
    
}

template <typename GenotypeType>
void GeneticAlgorithm<GenotypeType>::mutate( Moe<GenotypeType>& _moe )
{
    std::uniform_int_distribution<unsigned int> dist_mutation(0, m_keys.size()-1);
    _moe.genotype = m_mutations[ m_keys[ dist_mutation( this->m_generator ) ] ]->mutate(_moe.genotype, m_dataset );
}

template <typename GenotypeType>
std::vector<GenotypeType> GeneticAlgorithm<GenotypeType>::getRandomGenotype()
{
    std::vector<GenotypeType> genotype;
    genotype.reserve( m_fixedSize );

    for( unsigned int i = 0; i < m_fixedSize; i++ )
        genotype.push_back( m_dataset[ dist_dataset( this->m_generator ) ] );

    return genotype;
}
