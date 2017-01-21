#pragma once

#include "Moe.hpp"

template <typename GenomeType>
class Moether
{
    public:
        Moether();
        ~Moether();

        void init( unsigned int _moesPerGen, unsigned int _eliteCopies,
                    float _mutationRate, float _crossoverRate);

        void run( unsigned int _generations );

        void setFitnessFunction( std::function<const double&( const Moe& )> _fitnessFunction);
        void setGenomeGeneration( std::function<const GenomeType&()> _genomeGenerationFunction );

        const Moe& getBestMoe() const;

    private:
        std::function< const double&( const Moe&) > m_fitnessFunction;
        std::function< const GenomeType&() > m_genomeGeneration();

        unsigned int m_generations,
                    m_moesPerGen,
                    m_eliteCopies;
        
        float m_mutationRate,
            m_crossoverRate;

        Moe<GenomeType> m_bestMoe;
};