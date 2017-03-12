#pragma once
#include <vector>

namespace moe
{
namespace util
{
    template<typename T>
    const std::vector<T> getAlphabet()
    {
        return {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
    }

    template<typename T>
    const std::vector<T> getDigits()
    {
        return {0,1,2,3,4,5,6,7,8,9};
    }
}
}
