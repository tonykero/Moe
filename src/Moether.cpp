#include <moe/Moether.hpp>

#include <vector>

Moether::Moether()
{

}

Moether::~Moether()
{

}

void Moether::init( unsigned int _moesPerGen, unsigned int _eliteCopies,
                    float _mutationRate, float _crossoverRate )
{
    m_moesPerGen = _moesPerGen;
    m_eliteCopies = m_eliteCopies;
    m_mutationRate = _mutationRate;
    m_crossoverRate = _crossoverRate;
}

void Moether::run( unsigned int _generations )
{
    //TODO: Moether::run()

    // Algorithm:

    // 1) Generate random population
    //      for each generation
    // 2) Compute fitnesses
    // 3) Keep the best as "elite"
    // 4) put m_eliteCopies times elite in the new generation
    // 5) for each 2 free slots 
    // 6) Choose 2 candidates from past generation
    // 7) Generate 2 offsprings with crossover
    // 8) Mutate offsprings
    // 9) Put the 2 offsprings in the new generation
    // 10) Set the new generation as actual
    // 11) Step 2

    

}

void Moether::setFitnessFunction( std::function<const double&( const Moe& )> _fitnessFunction)
{
    m_fitness = _fitnessFunction;
}

void setGenomeGeneration( std::function<const GenomeType&()> _genomeGenerationFunction )
{
    m_genomeGeneration = _genomeGenerationFunction;
}

const Moe& getBestMoe() const
{
    return m_bestMoe;
}