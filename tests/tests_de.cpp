namespace de
{
    
TEST_CASE( "Differential Evolution Tests", "[differential_evolution]" )
{
    DifferentialEvolution<int> de(20);

    long long n = 2261953600;
    
    de.setFitnessFunction( [n](auto moe) -> double
    {
        long long genotype = moe.genotype[0];
        double error = std::abs(n - genotype*genotype);
        return 1/(error+1);
    });

    SECTION( "Should return an optimal solution" )
    {
        de.run(50);

        CAPTURE( de.getBestMoe().genotype[0] );
        CAPTURE( de.getBestMoe().fitness );
        REQUIRE( de.getBestMoe().fitness == Approx(1).margin(1));
    }
}

}
