namespace ParticleSwarm
{
    
TEST_CASE( "Simulated Annealing Tests", "[simulated_annealing]" )
{
    SimulatedAnnealing<float> sa( moe::SAParameters<float>() 
                                            .withTemperature(100000)
                                            .withCoolingRate(0.95f)
                                            .withDimensions(2)
                                            .withRange({-10, 10})
                                            );

    auto booth = [](double x, double y) -> double
    {
        double  op1 = x + 2*y - 7,
                op2 = 2*x + y - 5;
        return op1*op1 + op2*op2;
    };

    sa.setFitnessFunction( [booth](auto moe) -> double
    {
        double  x = moe.genotype[0],
                y = moe.genotype[1],
                r = booth(x, y);

        return 1/(r+1);
    });
    
    SECTION("Should return an optimal solution")
    {
        sa.run(1000);

        double  x = sa.getBestMoe().genotype[0],
                y = sa.getBestMoe().genotype[1];

        CAPTURE( x );
        CAPTURE( y );
        REQUIRE( sa.getBestMoe().fitness == Approx(100.0).epsilon(1) );
    }
}

}
