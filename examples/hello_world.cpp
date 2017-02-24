#include <moe/moe.hpp>
#include <iostream>

#ifdef _MSC_VER
    #include <string> // needed with MSVC 19.0 for overloaded << on std::string
#endif

#include <chrono>

int main()
{
    Moether<char> moether;
    std::vector<char> target = {'h','e','l','l','o',' ','w','o','r','l','d'};

    moether.setFitnessFunction( [target](const Moe<char>& moe) -> double
    {
        std::vector<char> genotype = moe.genotype;

        int             dSize   = target.size() - genotype.size();
        unsigned int    min     = std::min(target.size(), genotype.size()),
                        error   = std::abs(dSize) * 256; // add 256 error points for each extra or lack of char

        for(unsigned int i = 0; i < min; i++)
            error += std::abs( genotype[i] - target[i] );
        
        return error;
    });

    moether.setFitnessMode( false );                // fitness by scoring error
    moether.setCrossover( moe::Crossover::TwoPoint );

    std::vector<char> dataset = moe::alphabet;
    dataset.push_back(' '); // add space

    moether.setDataset(dataset);

    auto start = std::chrono::high_resolution_clock::now();

        moether.init( 200 , 100 );
        moether.run( 1500 );

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> diff = end-start;

    std::string genotype;
        for(char c : moether.getBestMoe().genotype)
            genotype += c;

    std::cout   << "genotype: " << genotype << "\n"
                << "fitness: " << moether.getBestMoe().fitness << "\n"
                << "time spent: " << diff.count() << " seconds" << std::endl;
}
