TEST_CASE( "Crossover Tests", "[crossovers]" )
{
    std::default_random_engine engine;
    
    OnePoint<char>  op(engine);
    TwoPoint<char>  tp(engine);
    Uniform<char>   uni(engine);
    
    const std::vector<char> sample1 = { 'a', 'a', 'a', 'a' },
                            sample2 = { 'b', 'b', 'b', 'b' };

    auto count_change = [](std::vector<char> vec) -> unsigned int
    {
        unsigned int count = 0;
        
        for(unsigned int i = 0; i < vec.size()-1; i++)
        {
            if( vec[i] != vec[i+1] )
                count++;
        }

        return count;
    };

    SECTION( "OnePoint Crossover" )
    {
        auto pair = op.cross(sample1, sample2);

        CAPTURE( pair.first );
        CAPTURE( pair.second );
        REQUIRE( pair.first.size() == pair.second.size() );
        REQUIRE( count_change( pair.first ) == 1 );
        REQUIRE( count_change( pair.second ) == 1 );
    }

    SECTION( "TwoPoint Crossover" )
    {
        auto pair = tp.cross( sample1, sample2 );

        CAPTURE( pair.first );
        CAPTURE( pair.second );
        REQUIRE( pair.first.size() == pair.second.size() );
        REQUIRE( count_change( pair.first ) <= 2 );
        REQUIRE( count_change( pair.second ) <= 2 );
    }

    SECTION( "Uniform Crossover" )
    {
        auto pair = uni.cross( sample1, sample2 );

        CAPTURE( pair.first );
        CAPTURE( pair.second );
        REQUIRE( pair.first.size() == pair.second.size() );
    }
}
