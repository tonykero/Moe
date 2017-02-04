#include <moe/moe.hpp>
#include <iostream>
#include <chrono>

int main()
{
    Moether<Moe> moether;

    unsigned int n = 2261953600;

    moether.setFitnessFunction( [n](const Moe& moe) -> double
    {
        std::string genotype = moe.getGenotype();
        unsigned long gen = std::stoul(genotype);

        double error = std::abs(n - gen*gen);

        return error;
    });

    moether.setFitnessMode( false );    // fitness by scoring error
    moether.setMaxGenotypeSize( 5 );
    moether.setGenotypeAscii(48, 57);   //only numbers
    moether.setCrossover( moe::Crossover::Uniform );

    auto start = std::chrono::high_resolution_clock::now();

        moether.init( 250, 60 );
        moether.run( 50 );

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> diff = end-start;

    std::cout   << "genotype: " << moether.getBestMoe().getGenotype() << "\n"
                << "fitness: " << moether.getBestMoe().getFitness() << "\n"
                << "time spent: " << diff.count() << " seconds" << std::endl;
}