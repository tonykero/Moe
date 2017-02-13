#pragma once

#include <random>
#include <memory>

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
}

template <typename MoeType>
class Moether
{
    public:
        Moether ();
        ~Moether();

        void            init   ( unsigned int _moesPerGen, unsigned int _eliteCopies, float _mutationRate = 0.1f, float _crossoverRate = 0.5f );
        void            run    ( unsigned int _generations );

        void            setFitnessFunction  ( std::function<double( const MoeType& )> _fitnessFunction );
        void            setFitnessMode      ( bool _mode);
        void            setInitGenotypeSize ( unsigned int _size );

        void            setCrossover        ( unsigned int _crossoverID );
        void            setCrossoverEnabled ( bool _crossoverEnabled );
        void            setMutationEnabled  ( bool _mutationEnabled );

        void            registerCrossover   ( std::unique_ptr<Crossover>);
        void            registerMutation    ( std::unique_ptr<Mutation> );

        void            setAsciiRange       ( unsigned int _a, unsigned int _b );
        void            setCharset          ( const std::string& _charset );
        
        const MoeType&  getBestMoe          () const;

    private:
        /* private member functions */
        std::string     randomizeGenotype   ();
        void            crossover           ( const MoeType& _parent1, const MoeType& _parent2, MoeType& _offspring1, MoeType& _offspring2 );
        void            mutate              ( MoeType& _moe );

        std::function< double( const MoeType& ) > m_fitnessFunction;
        // ---

        /* standard types */
        unsigned int    m_generations,
                        m_moesPerGen,
                        m_eliteCopies;
        unsigned int    m_initGenotypeSize = 8;
        unsigned int    m_crossover = moe::Crossover::OnePoint;
        
        float           m_mutationRate,
                        m_crossoverRate;

        bool            m_mode = true,
                        m_isCrossoverEnabled = true,
                        m_isMutationsEnabled = true;
        // ---

        std::vector< std::unique_ptr<Mutation> > m_mutations;
        std::vector< std::unique_ptr<Crossover>> m_crossovers;

        MoeType         m_bestMoe;
        std::string     m_charset;

        std::uniform_int_distribution<unsigned int> distrib_charset;
        std::default_random_engine gen;
};

#include "Moether_defs.hpp"