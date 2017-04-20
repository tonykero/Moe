#include <moe/moe.hpp>
#include <iostream>
#include <chrono>
#include <cmath>

#ifdef _MSC_VER
    #include <string>
#endif

#define f(x, y) himmelblau(x, y)
#define pi ((float)std::acos(-1))

float booth(float x, float y)
{
    float  op1 = x + 2*y - 7,
            op2 = 2*x + y - 5;

    return op1*op1 + op2*op2;
}

float himmelblau(float x, float y)
{
    float  op1 = x*x + y - 11,
            op2 = x + y*y - 7;
    
    return op1*op1 + op2*op2;
}

float easom(float x, float y)
{
    float  opcos = -std::cos(x) * std::cos(y),
            xpart = (x-pi)*(x-pi),
            ypart = (y-pi)*(y-pi),
            opexp = std::exp( -( xpart + ypart ) );

    return opcos*opexp;
}

float holdertable(float x, float y)
{
    float  optrigo = std::sin(x) * std::cos(y),
            opexp   = std::exp( std::abs( 1 - std::sqrt( x*x + y*y )/pi ) );
    
    return -std::abs( optrigo*opexp );
}

float ackley(float x, float y)
{
    float  op1 = -20 * std::exp( 0.2 * std::sqrt( 0.5 * (x*x + y*y) ) ),
            op2 = std::exp( 0.5 * ( std::cos( 2*pi*x ) + std::cos( 2*pi*y ) ) );
    
    return op1 - op2 + std::exp(1)+20;
}

int main()
{

    moe::ParticleSwarm<float> moether( moe::PSParameters<float>() 
                                            .withMoesPerGen(100)
                                            .withDimensions(2)
                                            .withRange({-10, 10})
                                            );
    //moe::ParticleSwarm<float> moether(50, 0.5f, 0.8f, 1.2f, 2, {-10, 10});

    moether.setFitnessFunction( [](auto moe) -> double
    {
        float   x = moe.genotype[0],
                y = moe.genotype[1];

        float   result = f(x, y);

        return -result;
    });

    auto start = std::chrono::high_resolution_clock::now();
    
        moether.run( 1000 );

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> diff = end-start;

    auto best_moe = moether.getBestMoe();
    

    std::cout   << "genotype: "     << best_moe.genotype[0] << "\t" << best_moe.genotype[1] << "\n"
                << "fitness: "      << best_moe.fitness << "\n"
                << "time spent: "   << diff.count() << " seconds" << std::endl;

}
