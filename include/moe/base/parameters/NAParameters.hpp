#pragma once

#include <type_traits>
#include <limits>
#include <vector>

namespace moe
{

template <typename GenotypeType>
struct NAParameters
{
    NAParameters()
    {
        static_assert( std::is_arithmetic<GenotypeType>::value, "NAParameters only works with arithmetic types" );
    }

    unsigned int    moesPerGen,
                    dimensions = 1;

    std::vector<GenotypeType> range{    std::numeric_limits<GenotypeType>::lowest(),
                                        std::numeric_limits<GenotypeType>::max() };

};

}
