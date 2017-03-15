# Moe
GCC 4.9+/Clang 3.6+ (Linux) | MSVC 19.0 (Win 32/64)
--- | ---
[![Travis branch](https://img.shields.io/travis/tonykero/Moe/feature/algorithms.svg?style=flat-square)](https://travis-ci.org/tonykero/Moe) | [![AppVeyor branch](https://img.shields.io/appveyor/ci/tonykero/Moe/feature/algorithms.svg?style=flat-square)](https://ci.appveyor.com/project/tonykero/moe)

[![license](https://img.shields.io/github/license/tonykero/Moe.svg?style=flat-square)](https://github.com/tonykero/Moe/blob/master/LICENSE)

Moe is a C++14 header-only dependency-free library providing generic implementations of some metaheuristic algorithms

## Quick Overview

The main goal of Moe is to provide a generic and easy way to implement fast solving systems
from any complexity with C++.

Moe stands for Maybe Overpowered Entity, but isn't limited to it.

Moe gives a way for the user to completely define the behavior of algorithms, all parameters are
adjustable, Mutations & Crossovers can be defined and added to Moether in addition to those given by default.

Take a look at a base sample using Moe ([examples/hello_world.cpp](https://github.com/tonykero/Moe/blob/master/examples/hello_world.cpp)):

```cpp
#include <moe/moe.hpp>
#include <iostream>
#include <string>

int main()
{
    GeneticAlgorithm<char> moether(200, 100); // char will be the Base Type for genotype creations
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

    auto dataset = moe::util::getAlphabet<char>();
    dataset.push_back(' '); // add space
    moether.setDataset(dataset);

    moether.run( 1500 );

    // converts std::vector<char> to std::string
    std::string genotype;
        for(char c : moether.getBestMoe().genotype)
            genotype += c;
    //
    
    std::cout   << "genotype: " << genotype << "\n"
                << "fitness: " << moether.getBestMoe().fitness << std::endl;
}

```

To understand how registering Mutations and Crossovers works check files 
[Mutations.hpp](https://github.com/tonykero/Moe/blob/master/include/moe/base/Mutations.hpp)
& [Crossovers.hpp](https://github.com/tonykero/Moe/blob/master/include/moe/base/Crossovers.hpp)
and [AlgorithmImpl.hpp Line 7 - 13](https://github.com/tonykero/Moe/blob/master/include/moe/base/algorithms/AlgorithmImpl.hpp#L77)

### Examples

Examples can be found [here](https://github.com/tonykero/Moe/tree/master/examples)

## Features

Moe contains the following features:

* Algorithms:
    * Genetic Algorithm
    * + Abstract Class

* Crossovers:
    * One Point
    * Two Point
    * Uniform

* Mutations:
    * Substitution
    * Insertion
    * Deletion
    * Translocation

* Planned:
    * Performance:
        * Parallel Implementation
    * Algorithms:
        * Differential Evolution
        * Particle Swarm Optimization

## Building

Moe uses CMake, options are available:

Options         | Description                   | Default Value |
--------------- | ----------------------------- | ------------- |
BUILD_EXAMPLES  | build examples                | ON            |
DEBUG           | Enable debugging symbols      | OFF           |

Moe was successfully tested against:
* GCC:
    * 4.9 (4.9.4)
    * 5 (5.4.1)
    * 6 (6.2)
    * MinGW:
        * GCC 5.3
* Clang:
    * 3.6 (3.6.2)
    * 3.7 (3.7.1)
    * 3.8 (3.8.0)
    * 3.9 (3.9.1)
* MSVC:
    * 19.0 ( Visual Studio 14 2015 )

Compiling:

```
git clone https://github.com/tonykero/Moe.git
cd Moe
mkdir build && cd build && cmake .. && cmake --build .
```
builds Moe with examples

## License

[The MIT License](https://opensource.org/licenses/MIT)
