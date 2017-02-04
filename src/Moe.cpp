#include <moe/base/Moe.hpp>

Moe::Moe()
{

}

Moe::~Moe()
{
    
}

const std::string& Moe::getGenotype() const
{
    return m_genotype;
}

const double& Moe::getFitness() const
{
    return m_fitness;
}

void Moe::setGenotype( std::string _genotype )
{
    m_genotype = _genotype;
}

void Moe::setFitness( double _fitness )
{
    m_fitness = _fitness;
}