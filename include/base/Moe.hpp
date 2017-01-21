#pragma once

#include <functional>

class Moether;

template <typename GenomeType>
class Moe
{
    public:
        Moe();
        virtual ~Moe() = 0;

        const GenomeType& getGenome() const;
        const double& getFitness() const;
        
        void onGenomeGenerated( std::function<void( GenomeType& )> _callbackGenomeGeneration);

    private:

        void setGenome( GenomeType _genome );
        void setFitness( double _fitness );

        GenomeType m_genome;
        double m_fitness;

    friend class Moether; // allow access to setters

};