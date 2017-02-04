#pragma once

#include <functional>

class Moe
{
    public:
        Moe();
        ~Moe();

        void setGenotype( std::string _genotype );
        void setFitness( double _fitness );

        const std::string& getGenotype() const;
        const double& getFitness() const;

    private:

        std::string m_genotype;
        double m_fitness;

};