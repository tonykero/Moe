#include <moe/moe.hpp>
#include <iostream>
#include <chrono>

#ifdef _MSC_VER
    #include <string> // needed with MSVC 19.0 for overloaded << on std::string
#endif

long long translate(const std::vector<int>& _vec);

int main()
{
    Moether<int> moether;

    long long n = 2261953600;

    moether.setFitnessFunction( [n](const Moe<int>& moe) -> double
    {
        long long genotype = translate(moe.genotype);
                
        double error = std::abs(n - genotype*genotype);

        return error;
    });

    moether.setFitnessMode( false );    // fitness by scoring error
    moether.setDataset( moe::numbers );   //only numbers

    auto start = std::chrono::high_resolution_clock::now();

        moether.init( 400, 80 );
        moether.run( 600 );

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> diff = end-start;


    long long genotype = translate(moether.getBestMoe().genotype);

    std::cout   << "genotype: "     << genotype << "\n"
                << "fitness: "      << moether.getBestMoe().fitness << "\n"
                << "time spent: "   << diff.count() << " seconds" << std::endl;
}

long long translate(const std::vector<int>& _vec)
{
    long long value = 0;
    
    for( unsigned int i = 0; i < _vec.size(); i++ )
        value += std::round(std::pow(10, _vec.size()-i-1)*_vec[i]);
    
    return value;
}
