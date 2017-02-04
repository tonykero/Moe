#include <moe/moe.hpp>

#include <iostream>

int main()
{
    Moether<Moe> moether;
    
    std::string target = "hello world";

    moether.setFitnessFunction( [target](const Moe& moe) -> double
    {
        std::string genotype = moe.getGenotype();
        int dSize = target.size() - genotype.size();

        // add 256 error points for each extra or lack of char
        unsigned int error = std::abs(dSize) * 256;

        for(unsigned int i = 0; i < genotype.size(); i++ )
        {
            if(i > target.size() - 1)
                break;

            error += std::abs( genotype[i] - target[i] );

        }
        return error;
    });

    moether.setFitnessMode( false );
    moether.setMaxGenotypeSize( target.size() );
    moether.setCrossover( moe::Crossover::Uniform );

    moether.init( 200 , 40);
    moether.run( 500 );

    std::string genotype = moether.getBestMoe().getGenotype();

    std::cout << "genotype: " << genotype << std::endl;
    std::cout << "fitness: " << moether.getBestMoe().getFitness() << std::endl;
}