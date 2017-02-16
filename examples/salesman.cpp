#include <moe/moe.hpp>
#include <iostream>

#ifdef _MSC_VER
    #include <string> // needed with MSVC 19.0 for overloaded << on std::string
#endif

#include <chrono>
#include <unordered_map>

void remove_duplicates(std::string &str)
{
    std::string unique = "";
    unique += str[0];
    for(unsigned int i = 1; i < str.size(); i++)
    {
        bool found = false;
        for(unsigned int j = 0; j < unique.size(); j++)
        {
            if(str[i] == unique[j])
                found = true;
        }
        if(found == false)
            unique += str[i];
    }
    str = unique;
}

int main()
{
    Moether<Moe> moether;
    
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

    moether.setFitnessFunction( [&distances](const Moe& moe) -> double
    {
        std::string genotype = moe.getGenotype();

        unsigned int fitness = 0;

        std::string test_string = genotype;
        remove_duplicates(test_string);

        if( test_string.size() == genotype.size() && genotype.size() == 4 )
        {

            int error = -1000;
            genotype.insert(0, "A");
            genotype += "A";

            while( genotype.size() >= 2 )
            {
                std::string pair = "";
                pair += genotype[0];
                pair += genotype[1];

                error += distances[ pair ];
                
                genotype.erase( genotype.begin() );
            }
            fitness += -error;
        }
        
        return fitness;
    });

    moether.setInitGenotypeSize( 5 );
    moether.setCharset("BCDE");
    moether.setCrossover( moe::Crossover::Uniform );

    auto start = std::chrono::high_resolution_clock::now();

        moether.init( 30 , 5 );
        moether.run( 100 );

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> diff = end-start;

    std::cout   << "genotype: " << moether.getBestMoe().getGenotype() << "\n"
                << "fitness: " << moether.getBestMoe().getFitness() << "\n"
                << "time spent: " << diff.count() << " seconds" << "\n";

    std::cout   << "path = A" << moether.getBestMoe().getGenotype() + "A\n"
                << "distance = " << std::abs(moether.getBestMoe().getFitness() - 1000) << std::endl;
}