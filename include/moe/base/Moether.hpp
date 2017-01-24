#pragma once

#include <random>

#include "Moe.hpp"

template <typename MoeType>
class Moether
{
    public:
        Moether();
        ~Moether();

        void init( unsigned int _moesPerGen, unsigned int _eliteCopies,
                    float _mutationRate, float _crossoverRate);

        void run( unsigned int _generations );

        void setFitnessFunction( std::function<double( const MoeType&)> _fitnessFunction);
        void setFitnessMode(bool _mode);
        void setMaxGenomeSize(int _size);
        
        const MoeType& getBestMoe() const;

    private:
        std::string randomizeGenome();

        std::function<double( const MoeType&) > m_fitnessFunction;

        unsigned int m_generations,
                    m_moesPerGen,
                    m_eliteCopies;

        int m_maxGenomeSize = -1;
        
        float m_mutationRate,
            m_crossoverRate;

        MoeType m_bestMoe;

        bool m_mode = true;

        std::default_random_engine gen;
        std::uniform_int_distribution<unsigned int> distrib_char;

};

#include "Moether_defs.hpp"