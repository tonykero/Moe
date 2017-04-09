#include <moe/moe.hpp>
#include <iostream>
#include <chrono>
#include <cmath>

#ifdef _MSC_VER
    #include <string>
#endif

//#define BOOTH
#define HIMMELBLAU

double f(double x, double y)
{
    double result;

    #if defined BOOTH
        double  op1 = x + 2*y - 7,
                op2 = 2*x + y - 5;

        result = op1*op1 + op2*op2;
    #elif defined HIMMELBLAU
        double  op1 = x*x + y - 11,
                op2 = x + y*y - 7;
    
        result = op1*op1 + op2*op2;
    #endif

    return result;
}

int main()
{
    ParticleSwarm<double> moether(50, 0.5f, 0.8f, 1.2f, 2, {-10, 10});

    moether.setFitnessFunction( [](auto moe) -> double
    {
        double  x = moe.genotype[0],
                y = moe.genotype[1];

        double  result = f(x, y);

        return 1/(result+1);
    });

    auto start = std::chrono::high_resolution_clock::now();
    
        moether.run( 75 );

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> diff = end-start;

    auto best_moe = moether.getBestMoe();
    

    std::cout   << "genotype: "     << best_moe.genotype[0] << "\t" << best_moe.genotype[1] << "\n"
                << "fitness: "      << best_moe.fitness << "\n"
                << "time spent: "   << diff.count() << " seconds" << std::endl;

}
