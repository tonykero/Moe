#include <moe/moe.hpp>
#include <iostream>

int main()
{
    Moether<Moe> moether;
    std::string target = "hello world";

    moether.setFitnessFunction( [target](const Moe& moe) -> double
    {
        std::string genotype = moe.getGenotype();

        int             dSize   = target.size() - genotype.size();
        unsigned int    min     = std::min(target.size(), genotype.size()),
                        error   = std::abs(dSize) * 256; // add 256 error points for each extra or lack of char

        for(unsigned int i = 0; i < min; i++)
            error += std::abs( genotype[i] - target[i] );
        
        return error;
    });

    moether.setFitnessMode( false );
    moether.setMaxGenotypeSize( target.size() );
    moether.setCrossover( moe::Crossover::Uniform );

    moether.init( 200 , 40 );
    moether.run( 500 );

    std::cout   << "genotype: " << moether.getBestMoe().getGenotype() << "\n"
                << "fitness: " << moether.getBestMoe().getFitness() << std::endl;
}