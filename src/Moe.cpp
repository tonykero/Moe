#include <moe/Moe.hpp>

Moe::Moe()
{

}

const GenomeType& Moe::getGenome() const
{
    return m_genome;
}

const double& Moe::getFitness() const
{
    return m_fitness;
}

void Moe::onGenomeGenerated( std::function<void( GenomeType& )> _callbackGenomeGeneration)
{
    _callbackGenomeGeneration( m_genome );
}

void Moe::setGenome( GenomeType _genome )
{
    m_genome = _genome;
}

void Moe::setFitness( double _fitness )
{
    m_fitness = _fitness;
}