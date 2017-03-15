#pragma once

#include <random> // distributions & engine
#include <memory> // std::unique_ptr
#include <unordered_map> // register functions
#include <functional> // std::function

#include <moe/base/Moe.hpp>
#include <moe/base/Mutations.hpp>
#include <moe/base/Crossovers.hpp>

template <typename GenotypeType>
class Algorithm
{
    public:
        Algorithm();
        virtual ~Algorithm();

        virtual void                                run                 ( unsigned int _generations ) = 0;
        
        virtual void                                setFitnessFunction  ( std::function< double( const Moe<GenotypeType>& ) > _fitnessFunction );
        virtual void                                setFixedSize        ( unsigned int _size );
        
        virtual void                                setCrossover        ( unsigned int _crossoverID );
        virtual void                                enableCrossover     ( bool );
        virtual void                                enableMutation      ( bool );

        virtual bool                                isCrossoverEnabled  () const;
        virtual bool                                isMutationEnabled   () const;

        virtual void                                registerCrossover   ( std::unique_ptr< Crossover<GenotypeType>  >, unsigned int _id);
        virtual void                                registerMutation    ( std::unique_ptr< Mutation<GenotypeType> > , unsigned int _id);
        virtual void                                unregisterCrossover ( unsigned int _id );
        virtual void                                unregisterMutation  ( unsigned int _id );

        virtual void                                setDataset          ( std::vector<GenotypeType> _dataset );
        virtual const std::vector<GenotypeType>&    getDataset          () const;

        virtual std::vector<GenotypeType>           getRandomGenotype   ();

        virtual const Moe<GenotypeType>&            getBestMoe          () const;
    
    protected:
        // private member functions
        virtual void                                            updateMutationsKeys ();
        virtual std::pair<std::vector<GenotypeType>, std::vector<GenotypeType>> crossover           ( const Moe<GenotypeType>& _parent1, const Moe<GenotypeType>& _parent2 );
        virtual void                                            mutate              ( Moe<GenotypeType>& _moe );

        std::function< double( const Moe<GenotypeType>& ) >     m_fitnessFunction;
        // --

        // standard types
        unsigned int    m_fixedSize             = 8;
        unsigned int    m_crossover             = moe::Crossover::OnePoint;

        float           m_crossoverRate         = 0.5f; // used to init Uniform Crossover
        // NOTE: m_mutationRate implemented in derived classes

        bool            m_isCrossoverEnabled    = true,
                        m_isMutationEnabled     = true;
        //--
        
        std::unordered_map< unsigned int, std::unique_ptr< Mutation<GenotypeType> > > m_mutations;
        std::unordered_map< unsigned int, std::unique_ptr< Crossover<GenotypeType> >> m_crossovers;
        std::vector<unsigned int>   m_keys; // mutations IDs

        std::vector<GenotypeType>   m_dataset;
        Moe<GenotypeType>           m_bestMoe;

        std::default_random_engine  m_generator;
        
};

template <typename GenotypeType>
Algorithm<GenotypeType>::Algorithm()
{
    registerMutation( std::make_unique< Substitution<GenotypeType>  >(m_generator), moe::Mutation::Substitution );
    registerMutation( std::make_unique< Insertion<GenotypeType>     >(m_generator), moe::Mutation::Insertion    );
    registerMutation( std::make_unique< Deletion<GenotypeType>      >(m_generator), moe::Mutation::Deletion     );
    registerMutation( std::make_unique< Translocation<GenotypeType> >(m_generator), moe::Mutation::Translocation);
    registerCrossover(std::make_unique< OnePoint<GenotypeType>      >(m_generator), moe::Crossover::OnePoint    );
    registerCrossover(std::make_unique< TwoPoint<GenotypeType>      >(m_generator), moe::Crossover::TwoPoint    );
    registerCrossover(std::make_unique< Uniform<GenotypeType>       >(m_generator, m_crossoverRate), moe::Crossover::Uniform);
}

template <typename GenotypeType>
Algorithm<GenotypeType>::~Algorithm()
{
    
}

template <typename GenotypeType>
void Algorithm<GenotypeType>::setFitnessFunction( std::function< double(const Moe<GenotypeType>& )> _fitnessFunction )
{
    m_fitnessFunction = _fitnessFunction;
}

template <typename GenotypeType>
void Algorithm<GenotypeType>::setFixedSize( unsigned int _size )
{
    m_fixedSize = _size;
    
    unregisterMutation( moe::Mutation::Insertion );
    unregisterMutation( moe::Mutation::Deletion );
}

template <typename GenotypeType>
void Algorithm<GenotypeType>::setCrossover( unsigned int _crossoverID )
{
    m_crossover = _crossoverID;
}

template <typename GenotypeType>
void Algorithm<GenotypeType>::enableCrossover( bool _enable )
{
    m_isCrossoverEnabled = _enable;
}

template <typename GenotypeType>
void Algorithm<GenotypeType>::enableMutation( bool _enable )
{
    m_isMutationEnabled = _enable;
}

template <typename GenotypeType>
bool Algorithm<GenotypeType>::isCrossoverEnabled() const
{
    return m_isCrossoverEnabled;
}

template <typename GenotypeType>
bool Algorithm<GenotypeType>::isMutationEnabled() const
{
    return m_isMutationEnabled;
}

template <typename GenotypeType>
void Algorithm<GenotypeType>::registerCrossover( std::unique_ptr< Crossover<GenotypeType> > _crossover, unsigned int _id )
{
    m_crossovers[_id] = std::move( _crossover );
}

template <typename GenotypeType>
void Algorithm<GenotypeType>::registerMutation( std::unique_ptr< Mutation<GenotypeType> > _mutation, unsigned int _id )
{
    m_mutations[_id] = std::move( _mutation );
    updateMutationsKeys();
}

template <typename GenotypeType>
void Algorithm<GenotypeType>::unregisterCrossover( unsigned int _id )
{
    m_crossovers.erase(_id);
}

template <typename GenotypeType>
void Algorithm<GenotypeType>::unregisterMutation( unsigned int _id )
{
    m_mutations.erase(_id);
    updateMutationsKeys();
}

template <typename GenotypeType>
void Algorithm<GenotypeType>::setDataset( std::vector<GenotypeType> _dataset )
{
    m_dataset = _dataset;
}

template <typename GenotypeType>
const std::vector<GenotypeType>& Algorithm<GenotypeType>::getDataset() const
{
    return m_dataset;
}

template <typename GenotypeType>
std::vector<GenotypeType> Algorithm<GenotypeType>::getRandomGenotype()
{
    std::vector<GenotypeType> genotype;
    genotype.reserve( m_fixedSize );

    std::uniform_int_distribution<unsigned int> dist_dataset(0, m_dataset.size()-1);

    for( unsigned int i = 0; i < m_fixedSize; i++ )
        genotype.push_back( m_dataset[ dist_dataset( m_generator ) ] );

    return genotype;
}

template <typename GenotypeType>
const Moe<GenotypeType>& Algorithm<GenotypeType>::getBestMoe() const
{
    return m_bestMoe;
}

template <typename GenotypeType>
void Algorithm<GenotypeType>::updateMutationsKeys()
{
    m_keys.clear();
    m_keys.reserve(m_mutations.size());
    for(const auto& i : m_mutations)
        m_keys.push_back(i.first);
}

template <typename GenotypeType>
std::pair<std::vector<GenotypeType>, std::vector<GenotypeType>> Algorithm<GenotypeType>::crossover( const Moe<GenotypeType>& _parent1, const Moe<GenotypeType>& _parent2 )
{
    return m_crossovers[ m_crossover ]->cross(_parent1.genotype, _parent2.genotype);
    
}

template <typename GenotypeType>
void Algorithm<GenotypeType>::mutate( Moe<GenotypeType>& _moe )
{
    std::uniform_int_distribution<unsigned int> dist_mutation(0, m_keys.size()-1);
    _moe.genotype = m_mutations[ m_keys[ dist_mutation( m_generator ) ] ]->mutate(_moe.genotype, m_dataset );
}
