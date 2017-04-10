# Moe
GCC/Clang (Linux) | VS14 (Win 32/64)
 :---: | :---:
[![Travis branch](https://img.shields.io/travis/tonykero/Moe/master.svg?style=flat-square)](https://travis-ci.org/tonykero/Moe) | [![AppVeyor branch](https://img.shields.io/appveyor/ci/tonykero/Moe/master.svg?style=flat-square)](https://ci.appveyor.com/project/tonykero/moe)

[![license](https://img.shields.io/github/license/tonykero/Moe.svg?style=flat-square)](https://github.com/tonykero/Moe/blob/master/LICENSE)

Moe is a C++14 header-only dependency-free library providing generic implementations of some metaheuristic algorithms

## Quick Overview

The main goal of Moe is to provide a generic and easy way to implement fast solving systems
from any complexity with C++.

Moe stands for Maybe Overpowered Entity, but isn't limited to it.

Moe gives a way for the user to completely define the behavior of algorithms, all parameters are
adjustable, and the structure allows to easily add features or custom Mutations/Crossover/Algorithms.

Take a look at a base sample using Moe ([examples/square_root.cpp](https://github.com/tonykero/Moe/blob/master/examples/square_root.cpp)):

This example illustrates how Differential Evolution can be used to search for the square root of a number (in this case 2261953600), the known answer is 47560.

```cpp
#include <moe/moe.hpp>
#include <iostream>
#include <chrono> // optional
#include <string> // needed with MSVC 19.0 for overloaded << on std::string

int main()
{
    DifferentialEvolution<int> moether(20);

    long long n = 2261953600;

    moether.setFitnessFunction( [n](auto moe) -> double
    {
        long long genotype = moe.genotype[0];
                
        double error = std::abs(n - genotype*genotype);

        return 1/(error+1);
    });

    auto start = std::chrono::high_resolution_clock::now(); // optional
    
        moether.run( 50 );

    auto end = std::chrono::high_resolution_clock::now();   // optional
    std::chrono::duration<float> diff = end-start;          //      //


    long long genotype = moether.getBestMoe().genotype[0];

    std::cout   << "genotype: "     << genotype << "\n"
                << "fitness: "      << moether.getBestMoe().fitness << "\n"
                << "time spent: "   << diff.count() << " seconds" << std::endl;
}
```

### Examples

Examples can be found [here](https://github.com/tonykero/Moe/tree/master/examples)

## Features

Moe contains the following features:

* Algorithms:
    * Genetic Algorithm
    * Differential Evolution
    * Particle Swarm Optimization
    * + Abstract Classes

* Genetic Algorithm features:
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
    * Better Parameters handling
        * Serialization

## Building

Moe uses CMake, options are available:

Options         | Description                   | Default Value |
--------------- | ----------------------------- | ------------- |
BUILD_EXAMPLES  | Builds Examples               | ON            |
BUILD_TESTS     | Builds Catch Unit Tests       | OFF           |
DEBUG           | Enables debugging symbols     | OFF           |

Moe was(and still) successfully tested against:
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
    * Visual Studio 14 2015

Compiling:

```
git clone https://github.com/tonykero/Moe.git
cd Moe
mkdir build && cd build && cmake .. && cmake --build .
```
builds Moe with examples

## License

[The MIT License](https://opensource.org/licenses/MIT)
