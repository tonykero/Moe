#include <moe/moe.hpp>

#include <iostream>

int main()
{
    Moether<Moe> moether;
    
    std::string target = "hello world";

    moether.setFitnessFunction( [target](const Moe& moe) -> double
    {
        std::string genome = moe.getGenome();
        int dSize = std::abs( genome.size() - target.size() );

        // add 256 error points for each extra or lack of char
        unsigned int error = dSize * 256;

        for(unsigned int i = 0; i < genome.size(); i++ )
        {
            if(i > target.size() - 1)
                break;

            error += std::abs( genome[i] - target[i] );

        }
        return error;
    });

    //moether.setMaxGenomeSize();
    moether.setFitnessMode( false );
    moether.setMaxGenomeSize(11);

    moether.init( 200 , 50, 0.1, 0.5);
    moether.run( 1000 );

    std::string genome = moether.getBestMoe().getGenome();

    std::cout << genome << std::endl;
    //std::cout << moether.getBestMoe().getFitness() << std::endl;
}