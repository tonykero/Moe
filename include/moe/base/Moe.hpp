#pragma once

#include <functional>

class Moe
{
    public:
        Moe();
        ~Moe();

        void setGenome( std::string _genome );
        void setFitness( double _fitness );

        const std::string& getGenome() const;
        const double& getFitness() const;

    private:

        std::string m_genome;
        double m_fitness;

};