#include <moe/moe.hpp>
#include <iostream>
#include <chrono>
#include <cstdlib> //atoi

int main()
{
    Moether<Moe> moether;

    unsigned int n = 2261953600;

    moether.setFitnessFunction( [n](const Moe& moe) -> double
    {
        std::string genotype = moe.getGenotype();
        unsigned long long gen = atoi(genotype.c_str()); //std::stoul not supported by MinGW :c

        double error = std::abs(n - gen*gen);

        return error;
    });

    moether.setFitnessMode( false );    // fitness by scoring error
    moether.setGenotypeAscii(48, 57);   //only numbers
    moether.setCrossover( moe::Crossover::TwoPoint );

    auto start = std::chrono::high_resolution_clock::now();

        moether.init( 300, 80 );
        moether.run( 1500 );

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> diff = end-start;

    std::cout   << "genotype: " << moether.getBestMoe().getGenotype() << "\n"
                << "fitness: " << moether.getBestMoe().getFitness() << "\n"
                << "time spent: " << diff.count() << " seconds" << std::endl;
}