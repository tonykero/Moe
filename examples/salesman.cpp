#include <moe/moe.hpp>
#include <iostream>
#include <chrono>

#ifdef _MSC_VER
    #include <string> // needed with MSVC 19.0 for overloaded << on std::string
#endif

#include <algorithm>
#include <unordered_map>

void remove_duplicates(std::vector<char> &str);

int main()
{
    moe::GeneticAlgorithm<char> moether( moe::GAParameters<char>()
                                            .withMoesPerGen(30)
                                            .withDataset({'B', 'C', 'D', 'E'})
                                            .withEliteCopies(5)
                                        );

    //moe::GeneticAlgorithm<char> moether(30, {'B', 'C', 'D', 'E'}, 5);
    
    // we create a 5-node graph, or 5 city map
    // as described here: http://computing.dcu.ie/~humphrys/Notes/Morelli.images/2a.gif
    std::unordered_map< std::string, unsigned int > distances;
    distances["AB"] = 100;
    distances["AC"] = 300;
    distances["AD"] = 100;
    distances["AE"] = 75;
    
    distances["BA"] = distances["AB"];
    distances["BC"] = 50;
    distances["BD"] = 75;
    distances["BE"] = 125;

    distances["CA"] = distances["AC"];
    distances["CB"] = distances["BC"];
    distances["CD"] = 100;
    distances["CE"] = 125;

    distances["DA"] = distances["AD"];
    distances["DB"] = distances["BD"];
    distances["DC"] = distances["CD"];
    distances["DE"] = 50;

    distances["EA"] = distances["AE"];
    distances["EB"] = distances["BE"];
    distances["EC"] = distances["CE"];
    distances["ED"] = distances["DE"];

    moether.setFitnessFunction( [&distances](auto moe) -> double
    {
        std::vector<char> genotype = moe.genotype;
        unsigned int fitness = 0;

        // we first validate the genotype by checking if is has any duplicates
        // if it has any, it would mean that we travel multiples time to the same city/node
        std::vector<char> test_string = genotype;
        remove_duplicates(test_string);

        if( test_string.size() == 4 )
        {
            int error = -1000;
            genotype.insert(genotype.begin(), 'A');
            genotype.push_back('A');

            // we process endpoints by pair
            while( genotype.size() >= 2 )
            {
                std::string pair = "";
                pair += genotype[0];
                pair += genotype[1];

                error += distances[ pair ];
                
                genotype.erase( genotype.begin() );
            }
            fitness -= error; // error is the distance traveled
        }
        return fitness;
    });

    moether.setFixedSize( 4 );
    moether.setCrossover( moe::crx::Uniform);

    auto start = std::chrono::high_resolution_clock::now();
        moether.run( 100 );

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> diff = end-start;

    // converts std::vector<char> to std::string
    std::string genotype;
        for(char c : moether.getBestMoe().genotype)
            genotype += c;
    //

    std::cout   << "genotype: " << genotype << "\n"
                << "fitness: " << moether.getBestMoe().fitness << "\n"
                << "time spent: " << diff.count() << " seconds" << "\n";

    std::cout   << "path = A" << genotype + "A\n"
                << "distance = " << std::abs(moether.getBestMoe().fitness - 1000) << std::endl;
}

void remove_duplicates(std::vector<char> &str)
{
    std::vector<char> unique;
    std::vector<char>::iterator it;
    unique.push_back(str[0]);
    for(unsigned int i = 1; i < str.size(); i++)
    {
        it = std::find(unique.begin(), unique.end(), str[i]);
        if( it == unique.end() )
            unique.push_back(str[i]);
    }
    str = unique;
}
