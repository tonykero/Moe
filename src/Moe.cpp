#include <moe/base/Moe.hpp>

Moe::Moe()
{

}

Moe::~Moe()
{
    
}

const std::string& Moe::getGenome() const
{
    return m_genome;
}

const double& Moe::getFitness() const
{
    return m_fitness;
}

void Moe::setGenome( std::string _genome )
{
    m_genome = _genome;
}

void Moe::setFitness( double _fitness )
{
    m_fitness = _fitness;
}