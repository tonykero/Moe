namespace ps
{
    
TEST_CASE( "Particle Swarm Tests", "[particle_swarm]" )
{
    ParticleSwarm<double> ps( moe::PSParameters<double>()
                                        .withMoesPerGen(50)
                                        .withInertia(0.5f)
                                        .withCoef1(0.8f)
                                        .withCoef2(1.2f)
                                        .withDimensions(2)
                                        .withRange({-10, 10})
                                        );

    auto booth = [](double x, double y) -> double
    {
        double  op1 = x + 2*y - 7,
                op2 = 2*x + y - 5;
        return op1*op1 + op2*op2;
    };

    ps.setFitnessFunction( [booth](auto moe) -> double
    {
        double  x = moe.genotype[0],
                y = moe.genotype[1],
                r = booth(x, y);

        return 1/(r+1);
    });
    
    SECTION("Should return an optimal solution")
    {
        ps.run(75);

        double  x = ps.getBestMoe().genotype[0],
                y = ps.getBestMoe().genotype[1];

        CAPTURE( x );
        CAPTURE( y );
        REQUIRE( ps.getBestMoe().fitness == Approx(1.0).epsilon(0.00000001) );
        REQUIRE( x == Approx(1.0).epsilon(0.00000001) );
        REQUIRE( y == Approx(3.0).epsilon(0.00000001) );
    }
}

}
