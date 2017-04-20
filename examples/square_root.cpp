#include <moe/moe.hpp>
#include <iostream>
#include <chrono>

#ifdef _MSC_VER
    #include <string> // needed with MSVC 19.0 for overloaded << on std::string
#endif

int main()
{
    moe::DifferentialEvolution<int> moether( moe::DEParameters<int>()
                                                .withMoesPerGen(20) );

    //moe::DifferentialEvolution<int> moether(20);

    long long n = 2261953600;

    moether.setFitnessFunction( [n](auto moe) -> double
    {
        long long genotype = moe.genotype[0];
                
        double error = std::abs(n - genotype*genotype);

        return 1/(error+1);
    });

    auto start = std::chrono::high_resolution_clock::now();
    
        moether.run( 50 );

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> diff = end-start;


    long long genotype = moether.getBestMoe().genotype[0];

    std::cout   << "genotype: "     << genotype << "\n"
                << "fitness: "      << moether.getBestMoe().fitness << "\n"
                << "time spent: "   << diff.count() << " seconds" << std::endl;
}
