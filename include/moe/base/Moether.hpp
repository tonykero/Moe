#pragma once

#include <random>
#include <memory>
#include <unordered_map>

#include "Moe.hpp"
#include "Mutations.hpp"
#include "Crossovers.hpp"

namespace moe
{
    namespace Crossover
    {
        enum : const unsigned int
        {
            OnePoint = 0,
            TwoPoint,
            Uniform
        };
    }
    namespace Mutation
    {
        enum : const unsigned int
        {
            Substitution = 0,
            Insertion,
            Deletion,
            Translocation
        };
    }
}

template <typename MoeType>
class Moether
{
    public:
        Moether ();
        ~Moether();

        void                init   ( unsigned int _moesPerGen, unsigned int _eliteCopies, float _mutationRate = 0.1f, float _crossoverRate = 0.5f );
        void                run    ( unsigned int _generations );

        void                setFitnessFunction  ( std::function<double( const MoeType& )> _fitnessFunction );
        void                setFitnessMode      ( bool _mode);
        void                setInitGenotypeSize ( unsigned int _size );

        void                setCrossover        ( unsigned int _crossoverID );
        void                setCrossoverEnabled ( bool _crossoverEnabled );
        void                setMutationEnabled  ( bool _mutationEnabled );

        const bool&         isFitnessMode       () const;
        const bool&         isCrossoverEnabled  () const;
        const bool&         isMutationEnabled   () const;

        void                registerCrossover   ( std::unique_ptr<Crossover>, unsigned int _id);
        void                registerMutation    ( std::unique_ptr<Mutation> , unsigned int _id);
        void                unregisterCrossover ( unsigned int _id );
        void                unregisterMutation  ( unsigned int _id );

        void                setAsciiRange       ( unsigned int _a, unsigned int _b );
        void                setCharset          ( const std::string& _charset );
        const std::string&  getCharset          () const;

        const MoeType&      getBestMoe          () const;

    private:
        /* private member functions */
        void                updateKeys          ();
        std::string         randomizeGenotype   ();
        void                crossover           ( const MoeType& _parent1, const MoeType& _parent2, MoeType& _offspring1, MoeType& _offspring2 );
        void                mutate              ( MoeType& _moe );

        std::function< double( const MoeType& ) > m_fitnessFunction;
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

        std::unordered_map< unsigned int, std::unique_ptr<Mutation> > m_mutations;
        std::unordered_map< unsigned int, std::unique_ptr<Crossover>> m_crossovers;
        std::vector<unsigned int> m_keys;

        MoeType         m_bestMoe;
        std::string     m_charset;

        std::uniform_int_distribution<unsigned int> distrib_charset;
        std::default_random_engine gen;
};

#include "Moether_defs.hpp"