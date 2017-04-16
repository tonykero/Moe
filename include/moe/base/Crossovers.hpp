#pragma once

#include <random>

#include "Moe.hpp"

namespace moe
{
namespace crx
{
    enum : const unsigned int
    {
        OnePoint = 0,
        TwoPoint,
        Uniform
    };
}

template <typename GenotypeType>
class Crossover
{
    public:
        Crossover(std::default_random_engine& _generator);

        virtual ~Crossover() = default;

        virtual std::pair<std::vector<GenotypeType>, std::vector<GenotypeType>> cross(const std::vector<GenotypeType>&, const std::vector<GenotypeType>&) const = 0;
    
    protected:
        std::default_random_engine& m_generator;
};

template <typename GenotypeType>
Crossover<GenotypeType>::Crossover(std::default_random_engine& _generator)
:m_generator(_generator)
{

}

template <typename GenotypeType>
class OnePoint : public Crossover<GenotypeType>
{
    public:
        OnePoint( std::default_random_engine& _generator )
        :Crossover<GenotypeType>(_generator)
        {

        }

        std::pair<std::vector<GenotypeType>, std::vector<GenotypeType>> cross(const std::vector<GenotypeType>& _genotype1, const std::vector<GenotypeType>& _genotype2) const override
        {
            unsigned int min    = std::min( _genotype1.size(), _genotype2.size() );
            std::uniform_int_distribution<unsigned int> distrib_index(0.45*min, 0.55*min);
            unsigned int index  = distrib_index( Crossover<GenotypeType>::m_generator );

            std::pair<std::vector<GenotypeType>, std::vector<GenotypeType>> ret;

            ret.first   = _genotype2;
            ret.second  = _genotype1;
            for(unsigned int i = 0; i < index; i++)
                std::swap(ret.first[i], ret.second[i]);
            
            return ret;
        }
};

template <typename GenotypeType>
class TwoPoint : public Crossover<GenotypeType>
{
    public:
        TwoPoint( std::default_random_engine& _generator )
        :Crossover<GenotypeType>(_generator)
        {

        }

        std::pair<std::vector<GenotypeType>, std::vector<GenotypeType>> cross(const std::vector<GenotypeType>& _genotype1, const std::vector<GenotypeType>& _genotype2) const override
        {
            std::uniform_int_distribution<unsigned int> distrib_index;
            unsigned int min    = std::min( _genotype1.size(), _genotype2.size() );

            distrib_index       = std::uniform_int_distribution<unsigned int>(min*0.05f, min*0.45f);
            unsigned int index1 = distrib_index( Crossover<GenotypeType>::m_generator );
            distrib_index       = std::uniform_int_distribution<unsigned int>(min*0.55f, min*0.95f);
            unsigned int index2 = distrib_index( Crossover<GenotypeType>::m_generator );

            std::pair<std::vector<GenotypeType>, std::vector<GenotypeType>> ret;
            ret.first   = _genotype1;
            ret.second  = _genotype2;

            for(unsigned int i = index1; i < index2; i++)
                std::swap(ret.first[i], ret.second[i]);
                
            return ret;
        }
};

template <typename GenotypeType>
class Uniform : public Crossover<GenotypeType>
{
    public:
        Uniform( std::default_random_engine& _generator, float _crossoverRate = 0.5f)
        :Crossover<GenotypeType>(_generator), m_crossoverRate(_crossoverRate)
        {

        }

        std::pair<std::vector<GenotypeType>, std::vector<GenotypeType>> cross(const std::vector<GenotypeType>& _genotype1, const std::vector<GenotypeType>& _genotype2) const override
        {
            std::bernoulli_distribution distrib_uniform(m_crossoverRate);
            unsigned int min = std::min( _genotype1.size(), _genotype2.size() );
            
            std::pair<std::vector<GenotypeType>, std::vector<GenotypeType>> ret;
            ret.first   = _genotype1;
            ret.second  = _genotype2;

            for(unsigned int i = 0; i < min; i++)
            {
                if(distrib_uniform( Crossover<GenotypeType>::m_generator ))
                    std::swap(ret.first[i], ret.second[i]);
            }
            return ret;
        }
    private:
        float m_crossoverRate;
};

}
