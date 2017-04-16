#pragma once

#include <random>

#include "Moe.hpp"

namespace moe
{
namespace mtn
{
    enum : const unsigned int
    {
        Substitution = 0,
        Insertion,
        Deletion,
        Translocation
    };
}

template <typename GenotypeType>
class Mutation
{
    public:
        Mutation(std::default_random_engine& _generator);
        
        virtual ~Mutation() = default;

        virtual std::vector<GenotypeType> mutate ( const std::vector<GenotypeType>& _moeGenotype, const std::vector<GenotypeType>& _dataset ) const = 0;
    
    protected:
        std::default_random_engine& m_generator;
};

template <typename GenotypeType>
Mutation<GenotypeType>::Mutation(std::default_random_engine& _generator)
:m_generator(_generator)
{

}

template <typename GenotypeType>
class Substitution : public Mutation<GenotypeType>
{
    public:
        Substitution( std::default_random_engine& _generator )
        :Mutation<GenotypeType>(_generator)
        {

        }

        std::vector<GenotypeType> mutate ( const std::vector<GenotypeType>& _moeGenotype, const std::vector<GenotypeType>& _dataset ) const override
        {
            std::vector<GenotypeType> moe_genotype = _moeGenotype;
            auto dataset = _dataset;
            std::uniform_int_distribution<unsigned int> distrib;

            distrib = std::uniform_int_distribution<unsigned int>(0, moe_genotype.size()-1);
            unsigned int genotype_index = distrib( Mutation<GenotypeType>::m_generator );
            
            distrib = std::uniform_int_distribution<unsigned int>(0, _dataset.size()-1);
            unsigned int dataset_index  = distrib( Mutation<GenotypeType>::m_generator );

            while(moe_genotype[genotype_index] == dataset[dataset_index])
                dataset_index = distrib( Mutation<GenotypeType>::m_generator );

            GenotypeType mutation = dataset[ dataset_index ];
            moe_genotype[ genotype_index ] = mutation;

            return moe_genotype;
        }
};

template <typename GenotypeType>
class Insertion : public Mutation<GenotypeType>
{
    public:
        Insertion( std::default_random_engine& _generator )
        :Mutation<GenotypeType>(_generator)
        {

        }

        std::vector<GenotypeType> mutate ( const std::vector<GenotypeType>& _moeGenotype, const std::vector<GenotypeType>& _dataset ) const override
        {
            std::vector<GenotypeType> moe_genotype = _moeGenotype;
            std::uniform_int_distribution<unsigned int> distrib(0, _dataset.size()-1);

            GenotypeType mutation = _dataset[ distrib( Mutation<GenotypeType>::m_generator ) ];

            distrib = std::uniform_int_distribution<unsigned int>(0, moe_genotype.size()-1);
            moe_genotype.insert( moe_genotype.begin() + distrib( Mutation<GenotypeType>::m_generator ), mutation );

            return moe_genotype;
        }
};

template <typename GenotypeType>
class Deletion : public Mutation<GenotypeType>
{
    public:
        Deletion( std::default_random_engine& _generator )
        :Mutation<GenotypeType>(_generator)
        {

        }

        std::vector<GenotypeType> mutate ( const std::vector<GenotypeType>& _moeGenotype, const std::vector<GenotypeType>& _dataset ) const override
        {
            std::vector<GenotypeType> moe_genotype = _moeGenotype;
            if( moe_genotype.size() > 1 )
            {
                std::uniform_int_distribution<unsigned int> distrib(0, moe_genotype.size()-1);

                moe_genotype.erase( moe_genotype.begin() + distrib( Mutation<GenotypeType>::m_generator ));
            }

            return moe_genotype;
        }
};

template <typename GenotypeType>
class Translocation : public Mutation<GenotypeType>
{
    public:
        Translocation( std::default_random_engine& _generator )
        :Mutation<GenotypeType>(_generator)
        {

        }
        
        std::vector<GenotypeType> mutate ( const std::vector<GenotypeType>& _moeGenotype, const std::vector<GenotypeType>& _dataset ) const override
        {
            std::vector<GenotypeType> moe_genotype = _moeGenotype;
            std::uniform_int_distribution<unsigned int> distrib(0, moe_genotype.size()-1);

            unsigned int    a = distrib( Mutation<GenotypeType>::m_generator ),
                            b = distrib( Mutation<GenotypeType>::m_generator );
            
            // Ensure a & b are different
            while( a-b == 0 )
            {
                b = distrib( Mutation<GenotypeType>::m_generator );
            }
                            
            GenotypeType tmp = moe_genotype[a];
            moe_genotype[a] = moe_genotype[b];
            moe_genotype[b] = tmp;

            return moe_genotype;
        }
};

}
