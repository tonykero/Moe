# Moe 

Moe is a cross-platform C++14 dependency-free [ implementation / framework ] of a Generic Genetic Algorithm

## Quick Overview

The main goal of Moe is to provide a generic and easy way to implement fast solving systems
from any complexity by using C++ features like templates & lambda functions.

### Moether

Moether is the object that will execute and train Moes
basically with those 2 functions

```cpp
void            init   ( unsigned int _moesPerGen, unsigned int _eliteCopies, float _mutationRate = 0.1f, float _crossoverRate = 0.5f );
void            run    ( unsigned int _generations );
```

A lot of setters are present in Moether in order to change parameters of the Genetic Algorithm

Once you've trained a certain amount of Moes you retrieve the best element with the following function:
```cpp
const MoeType&  getBestMoe() const;
```

### Moe

Moe is an object supposed to be a trained model, it is a very simple class with few functions, Moether is written with templates to handle inheritage from the class Moe

Simple problems can be solved with this class, but in all other cases, it is necessary to make a class
that inherits from Moe.

In some examples, genotype is basically containing the solution explicitly, but when problems are more complex, genotype needs a generation function and an interpretation function.

### Examples

Examples can be found [here](https://github.com/tonykero/Moe/tree/master/examples)

## Features

This contains the following features:

* Crossovers:
    * One Point ( randomly chosen )
    * Two Point ( randomly chosen )
    * Uniform ( adjustable rate )

* Mutations: ( ajustable rate )
    * Substitution
    * Insertion ( not yet )
    * Deletion  ( not yet )
    * Translocation ( not yet)

* Elitism ( ajustable number )

* Genetic Representation: ( enhancement needed )
    * Explicit representation
    * User-defined

* Performance:
    * Parallel Implementation ( not yet )

* Adaptive GA Variant ( not yet )

## Building

Moe uses CMake, options are available:

Options         | Description                   | Default Value |
--------------- | ----------------------------- | ------------- |
BUILD_SHARED    | builds Moe as SHARED if ON    | OFF           |
BUILD_EXAMPLES  | build examples                | ON            |

Compiling:

```
git clone https://github.com/tonykero/Moe.git
cd Moe
mkdir build && cd build && cmake .. && cmake --build .
```
builds Moe as STATIC with examples


## License

The MIT License