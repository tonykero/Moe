namespace ga
{
    
TEST_CASE( "Genetic Algorithm tests", "[genetic_algorithm]" )
{
    auto dataset = moe::util::getAlphabet<char>();
    dataset.push_back(' ');

    global::GeneticAlgorithm<char> ga( 200, dataset, 100 );

    SECTION( "Getters tests" )
    {
        SECTION( "Dataset check" )
        {
             std::vector<char> model = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',' '};

        REQUIRE( ga.getDataset() == model );

        }
        SECTION( "Default Crossover is OnePoint" )
        {
            REQUIRE( ga.getCrossover() == moe::crx::OnePoint );
        }

        SECTION( "Crossover & Mutation is enabled by default" )
        {
            REQUIRE( ga.isCrossoverEnabled() );
            REQUIRE( ga.isMutationEnabled() );
        }

        SECTION( "random genotype should as long as m_fixedSize" )
        {
            REQUIRE( ga.getRandomGenotype().size() == ga.getFixedSize() );
        }
    }

    SECTION( "Should return an optimal solution" )
    {
        std::vector<char> target = {'h','e','l','l','o',' ','w','o','r','l','d'};
        
        ga.setFitnessFunction( [target](auto moe) -> double
        {
            std::vector<char> genotype = moe.genotype;

            int             dSize   = target.size() - genotype.size(); // get difference of number of characters
            unsigned int    min     = std::min(target.size(), genotype.size());
            double          error   = std::abs(dSize) * 256; // add 256 error points for each extra or lack of char

            for(unsigned int i = 0; i < min; i++)
                error += std::abs( genotype[i] - target[i] ); // each difference of character is added to error score
            
            return 1/(error+1);
        });

        ga.run( 1500 );

        // converts std::vector<char> to std::string
        std::string genotype;
            for(char c : ga.getBestMoe().genotype)
                genotype += c;
        //

        CAPTURE( genotype );
        CAPTURE( ga.getBestMoe().fitness );
        REQUIRE( ga.getBestMoe().fitness <= 4 );
    }
}

}
