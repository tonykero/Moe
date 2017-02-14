#pragma once

#include <random>

#include "Moe.hpp"

class Mutation
{
    public:
        Mutation(std::default_random_engine& _generator)
        :m_generator(_generator)
        {

        }
        
        virtual ~Mutation(){};

        virtual std::string mutate ( const std::string& _moeGenotype, const std::string& _charset ) const = 0;
    
    protected:
        std::default_random_engine& m_generator;
};

class Substitution : public Mutation
{
    public:
        Substitution( std::default_random_engine& _generator )
        :Mutation(_generator)
        {

        }

        std::string mutate( const std::string& _moeGenotype, const std::string& _charset ) const override
        {
            std::string moe_genotype = _moeGenotype;
            std::uniform_int_distribution<unsigned int> distrib(0, _charset.size()-1);

            char mutation = _charset[ distrib( m_generator ) ];
            
            distrib = std::uniform_int_distribution<unsigned int>(0, moe_genotype.size()-1);
            moe_genotype[ distrib( m_generator ) ] = mutation;

            return moe_genotype;
        }
};

class Insertion : public Mutation
{
    public:
        Insertion( std::default_random_engine& _generator )
        :Mutation(_generator)
        {

        }

        std::string mutate( const std::string& _moeGenotype, const std::string& _charset ) const override
        {
            std::string moe_genotype = _moeGenotype;
            std::uniform_int_distribution<unsigned int> distrib(0, _charset.size()-1);

            char mutation = _charset[ distrib( m_generator ) ];

            distrib = std::uniform_int_distribution<unsigned int>(0, moe_genotype.size()-1);
            moe_genotype.insert( moe_genotype.begin() + distrib( m_generator ), mutation );

            return moe_genotype;
        }
};

class Deletion : public Mutation
{
    public:
        Deletion( std::default_random_engine& _generator )
        :Mutation(_generator)
        {

        }

        std::string mutate( const std::string& _moeGenotype, const std::string& _charset ) const override
        {
            std::string moe_genotype = _moeGenotype;
            if( moe_genotype.size() > 1 )
            {
                std::uniform_int_distribution<unsigned int> distrib(0, moe_genotype.size()-1);

                moe_genotype.erase( moe_genotype.begin() + distrib( m_generator ));
            }

            return moe_genotype;
        }
};

class Translocation : public Mutation
{
    public:
        Translocation( std::default_random_engine& _generator )
        :Mutation(_generator)
        {

        }
        
        std::string mutate( const std::string& _moeGenotype, const std::string& _charset ) const override
        {
            std::string moe_genotype = _moeGenotype;
            std::uniform_int_distribution<unsigned int> distrib(0, moe_genotype.size()-1);

            unsigned int    a = distrib( m_generator ),
                            b = distrib( m_generator );
            char tmp = moe_genotype[a];
            moe_genotype[a] = moe_genotype[b];
            moe_genotype[b] = tmp;

            return moe_genotype;
        }
};