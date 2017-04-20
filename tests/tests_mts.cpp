namespace mts
{

TEST_CASE( "Mutations Tests", "[mutations]" )
{
    std::default_random_engine engine( std::chrono::high_resolution_clock::now().time_since_epoch().count() );
    auto dataset = moe::util::getAlphabet<char>();
    
    Substitution<char>  s(engine);
    Insertion<char>     i(engine);
    Deletion<char>      d(engine);
    Translocation<char> t(engine);

    const std::vector<char> sample = { 'a', 'b', 'c', 'd' };

    SECTION( "Substitution Mutation" )
    {

        auto post_sample = s.mutate(sample, dataset);

        unsigned int    pre_size    = sample.size(),
                        post_size   = post_sample.size();
        unsigned int    count = 0;
        
        for(unsigned int index = 0; index < pre_size-1; index++)
        {
            if( sample[index] != post_sample[index] )
                count++;
        }

        CAPTURE( sample );
        CAPTURE( post_sample );
        REQUIRE( pre_size == post_size );
        REQUIRE( count == 1 );
    }

    SECTION( "Insertion Mutation" )
    {
        unsigned int    pre_size    = sample.size(),
                        post_size   = i.mutate(sample, dataset).size();
    
        REQUIRE( pre_size+1 == post_size );
    }

    SECTION( "Deletion Mutation" )
    {
        unsigned int    pre_size    = sample.size(),
                        post_size   = d.mutate(sample, dataset).size();
    
        REQUIRE( pre_size-1 == post_size );
    }

    SECTION( "Translocation Mutation" )
    {
        auto post_sample = t.mutate(sample, dataset);

        unsigned int    pre_size    = sample.size(),
                        post_size   = post_sample.size();
        unsigned int    count = 0;
        
        for(unsigned int index = 0; index < pre_size; index++)
        {
            if( sample[index] != post_sample[index] )
                count++;
        }

        CAPTURE( sample );
        CAPTURE( post_sample );
        REQUIRE( pre_size == post_size );
        REQUIRE( count == 2 );
    }

}

}
