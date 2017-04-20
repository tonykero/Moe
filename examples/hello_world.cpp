#include <moe/moe.hpp>
#include <iostream>
#include <chrono>

#ifdef _MSC_VER
    #include <string> // needed with MSVC 19.0 for overloaded << on std::string
#endif

int main()
{
    auto dataset = moe::util::getAlphabet<char>();
    dataset.push_back(' '); // add space


    moe::GeneticAlgorithm<char> moether( moe::GAParameters<char>() 
                                            .withMoesPerGen(200)
                                            .withDataset(dataset)
                                            .withEliteCopies(100));

    //moe::GeneticAlgorithm<char> moether(200, dataset, 100); // char will be the Base Type for genotype creations
    
    std::vector<char> target = {'h','e','l','l','o',' ','w','o','r','l','d'};

    moether.setFitnessFunction( [target](auto moe) -> double
    {
        std::vector<char> genotype = moe.genotype;

        int             dSize   = target.size() - genotype.size(); // get difference of number of characters
        unsigned int    min     = std::min(target.size(), genotype.size());
        double          error   = std::abs(dSize) * 256; // add 256 error points for each extra or lack of char

        for(unsigned int i = 0; i < min; i++)
            error += std::abs( genotype[i] - target[i] ); // each difference of character is added to error score
        
        return 1/(error+1);
    });

    auto start = std::chrono::high_resolution_clock::now();
    
    moether.run( 1500 );

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> diff = end-start;

    // converts std::vector<char> to std::string
    std::string genotype;
        for(char c : moether.getBestMoe().genotype)
            genotype += c;
    //
    
    std::cout   << "genotype: " << genotype << "\n"
                << "fitness: " << moether.getBestMoe().fitness << "\n"
                << "time spent: " << diff.count() << " seconds" << std::endl;
}
