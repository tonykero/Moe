#pragma once

#include <random>

#include "Moe.hpp"

namespace moe
{
    enum Crossover : unsigned int
    {
        OnePoint = 0,
        TwoPoint,
        Uniform
    };

    enum Mutation : unsigned int
    {
        NONE = 0,
        Substitution =  1 << 0,
        Insertion =     1 << 1,
        Deletion =      1 << 2,
        Translocation = 1 << 3,

        ALL = Substitution | Insertion | Deletion | Translocation
    };
};

template <typename MoeType>
class Moether
{
    public:
        Moether();
        ~Moether();

        void            init   ( unsigned int _moesPerGen, unsigned int _eliteCopies, float _mutationRate = 0.1f, float _crossoverRate = 0.5f );
        void            run    ( unsigned int _generations );

        void            setFitnessFunction ( std::function<double( const MoeType& )> _fitnessFunction );
        void            setFitnessMode     ( bool _mode);
        void            setMaxGenotypeSize ( unsigned int _size );
        void            setCrossover       ( unsigned int _type );
        void            setMutation        ( unsigned int _type );

        void            setGenotypeAscii   ( unsigned int _a, unsigned int _b );

        void            setCrossoverRate   ( float _rate );
        void            setMutationRate    ( float _rate );
        
        const MoeType&  getBestMoe() const;

    private:
        std::string     randomizeGenotype   ();
        void            crossover           ( MoeType& _parent1, MoeType& _parent2, MoeType& _offspring1, MoeType& _offspring2 );

        std::function< double( const MoeType& ) > m_fitnessFunction;

        unsigned int    m_generations,
                        m_moesPerGen,
                        m_eliteCopies;
        unsigned int    m_crossover = moe::Crossover::OnePoint,
                        m_mutation = moe::Mutation::ALL;
        unsigned int    m_maxGenotypeSize = 64;
        
        float           m_mutationRate,
                        m_crossoverRate;

        MoeType         m_bestMoe;

        bool            m_mode = true;


        std::default_random_engine gen;
        std::uniform_int_distribution<unsigned int> distrib_char;

};

#include "Moether_defs.hpp"