#pragma once

#include <random>

#include "Moe.hpp"

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

class Crossover
{
    public:
        Crossover(std::default_random_engine& _generator)
        :m_generator(_generator)
        {

        };
        virtual ~Crossover(){};

        virtual std::pair<std::string, std::string> cross(const std::string&, const std::string&) const = 0;
    
    protected:
        std::default_random_engine& m_generator;
};

class OnePoint : public Crossover
{
    public:
        OnePoint( std::default_random_engine& _generator )
        :Crossover(_generator)
        {

        }

        std::pair<std::string, std::string> cross(const std::string& _genotype1, const std::string& _genotype2) const override
        {
            unsigned int min    = std::min( _genotype1.size(), _genotype2.size() );
            std::uniform_int_distribution<unsigned int> distrib_index(1, min-2);
            unsigned int index  = distrib_index( m_generator );

            std::pair<std::string, std::string> ret;
            ret.first   = _genotype1.substr(0, index)
                        + _genotype2.substr(index, _genotype2.size() - index);
            ret.second  = _genotype2.substr(0, index)
                        + _genotype1.substr(index, _genotype1.size() - index);

            return ret;
        }
};

class TwoPoint : public Crossover
{
    public:
        TwoPoint( std::default_random_engine& _generator )
        :Crossover(_generator)
        {

        }

        std::pair<std::string, std::string> cross(const std::string& _genotype1, const std::string& _genotype2) const override
        {
            std::uniform_int_distribution<unsigned int> distrib_index;
            unsigned int min    = std::min( _genotype1.size(), _genotype2.size() );

            distrib_index       = std::uniform_int_distribution<unsigned int>(min*0.05f, min*0.45f);
            unsigned int index1 = distrib_index( m_generator );
            distrib_index       = std::uniform_int_distribution<unsigned int>(min*0.55f, min*0.95f);
            unsigned int index2 = distrib_index( m_generator );

            std::pair<std::string, std::string> ret;
            ret.first   = _genotype1;
            ret.second  = _genotype2;

            for(unsigned int i = index1; i < index2; i++)
            {
                char cs = ret.first[i];
                ret.first[i] = ret.second[i];
                ret.second[i] = cs;
            }
            return ret;
        }
};

class Uniform : public Crossover
{
    public:
        Uniform( std::default_random_engine& _generator, float& _crossoverRate)
        :Crossover(_generator), m_crossoverRate(_crossoverRate)
        {

        }

        std::pair<std::string, std::string> cross(const std::string& _genotype1, const std::string& _genotype2) const override
        {
            std::bernoulli_distribution distrib_uniform(m_crossoverRate);
            unsigned int min = std::min( _genotype1.size(), _genotype2.size() );
            
            std::pair<std::string, std::string> ret;
            ret.first   = _genotype1;
            ret.second  = _genotype2;

            for(unsigned int i = 0; i < min; i++)
            {
                if(distrib_uniform( m_generator ))
                {
                    char cs = ret.first[i];
                    ret.first[i] = ret.second[i];
                    ret.second[i] = cs;
                }
            }
            return ret;
        }
    private:
        float& m_crossoverRate;
};