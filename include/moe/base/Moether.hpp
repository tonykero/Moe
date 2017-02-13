#pragma once

#include <random>
#include <memory>

#include "Moe.hpp"
#include "Mutations.hpp"

namespace moe
{
    namespace Crossover
    {
        enum : unsigned int
        {
            NONE = 0,
            OnePoint,
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
        void            setCrossover        ( unsigned int _type );

        void            registerMutation    ( std::unique_ptr<Mutation> );

        void            setAsciiRange       ( unsigned int _a, unsigned int _b );
        void            setCharset          ( const std::string& _charset );

        void            setCrossoverRate    ( float _rate );
        void            setMutationRate     ( float _rate );
        
        const MoeType&  getBestMoe          () const;

    private:
        std::string     randomizeGenotype   ();
        void            crossover           ( MoeType& _parent1, MoeType& _parent2, MoeType& _offspring1, MoeType& _offspring2 );
        void            mutate              ( MoeType& _moe );

        std::function< double( const MoeType& ) > m_fitnessFunction;

        unsigned int    m_generations,
                        m_moesPerGen,
                        m_eliteCopies;
        unsigned int    m_initGenotypeSize = 8;
        unsigned int    m_crossover = moe::Crossover::OnePoint;
        
        std::vector< std::unique_ptr<Mutation> > m_mutations;

        std::string     m_charset;
        
        float           m_mutationRate,
                        m_crossoverRate;

        MoeType         m_bestMoe;

        bool            m_mode = true,
                        m_isCrossoverEnabled = true,
                        m_isMutationsEnabled = true;

        std::default_random_engine gen;
        std::uniform_int_distribution<unsigned int> distrib_charset;

};

#include "Moether_defs.hpp"