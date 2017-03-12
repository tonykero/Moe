#pragma once

#include <random>
#include <memory>
#include <unordered_map>
#include <functional>

#include "Moe.hpp"
#include "Mutations.hpp"
#include "Crossovers.hpp"

template <typename GenotypeType>
class Moether
{
    public:
        Moether ();
        ~Moether();

        void                                init   ( unsigned int _moesPerGen, unsigned int _eliteCopies, float _mutationRate = 0.1f, float _crossoverRate = 0.5f );
        void                                run    ( unsigned int _generations );

        void                                setFitnessFunction  ( std::function<double( const Moe<GenotypeType>& )> _fitnessFunction );
        void                                setFitnessMode      ( bool _mode);
        void                                setInitGenotypeSize ( unsigned int _size );

        void                                setCrossover        ( unsigned int _crossoverID );
        void                                setCrossoverEnabled ( bool _crossoverEnabled );
        void                                setMutationEnabled  ( bool _mutationEnabled );

        const bool&                         isFitnessMode       () const;
        const bool&                         isCrossoverEnabled  () const;
        const bool&                         isMutationEnabled   () const;

        void                                registerCrossover   ( std::unique_ptr< Crossover<GenotypeType>  >, unsigned int _id);
        void                                registerMutation    ( std::unique_ptr< Mutation<GenotypeType> > , unsigned int _id);
        void                                unregisterCrossover ( unsigned int _id );
        void                                unregisterMutation  ( unsigned int _id );

        void                                setDataset          ( const std::vector<GenotypeType>& _dataset );
        const std::vector<GenotypeType>&    getDataset          () const;

        const Moe<GenotypeType>&            getBestMoe          () const;

    private:
        /* private member functions */
        void                                updateKeys          ();
        std::vector<GenotypeType>           randomizeGenotype   ();
        void                                crossover           ( const Moe<GenotypeType>& _parent1, const Moe<GenotypeType>& _parent2, Moe<GenotypeType>& _offspring1, Moe<GenotypeType>& _offspring2 );
        void                                mutate              ( Moe<GenotypeType>& _moe );

        std::function< double( const Moe<GenotypeType>& ) > m_fitnessFunction;
        // ---

        /* standard types */
        unsigned int    m_generations,
                        m_moesPerGen,
                        m_eliteCopies;
        unsigned int    m_initGenotypeSize = 8;
        unsigned int    m_crossover = moe::Crossover::OnePoint;
        
        float           m_mutationRate  = 0.1f,
                        m_crossoverRate = 0.5f;

        bool            m_mode = true,
                        m_isCrossoverEnabled = true,
                        m_isMutationsEnabled = true;
        // ---

        std::unordered_map< unsigned int, std::unique_ptr< Mutation<GenotypeType> > > m_mutations;
        std::unordered_map< unsigned int, std::unique_ptr< Crossover<GenotypeType> >> m_crossovers;
        std::vector<unsigned int> m_keys;

        Moe<GenotypeType>           m_bestMoe;
        std::vector<GenotypeType>   m_dataset;

        std::uniform_int_distribution<unsigned int> distrib_dataset;
        std::default_random_engine gen;
};

#include "Moether_defs.hpp"
