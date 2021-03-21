#include <vector>
#include "../thirdPartyLibraries/catch2/catch.hpp"
#include "../src/Engines/LivelyMath.h"
#define MARGIN_OF_ERROR_FOR_DOUBLE 0.00001

TEST_CASE("linSpace testing", "[linSpace]")
{
    std::vector<double> solution;

    SECTION("Create one vector of length 0")
    {
        solution = {0};
        REQUIRE(livelyRD::linSpace(0,0,1) == solution);
    }

    SECTION("Create one vector of length 1")
    {
        solution = {1};
        REQUIRE(livelyRD::linSpace(1,0,1) == solution);
    }

    SECTION("Create one vector of length 2")
    {
        solution = {0,1};
        REQUIRE(livelyRD::linSpace(2,0,1) == solution);
    }

    SECTION("Create one vector of length 5")
    {
        solution = {0.0,0.2,0.4,0.6,0.8};
        std::vector<double> obtainedSolution = livelyRD::linSpace(5,0,0.8);
        for(std::size_t i = 0; i<solution.size(); ++i)
        {
            REQUIRE(obtainedSolution[i] < solution[i] + MARGIN_OF_ERROR_FOR_DOUBLE);
            REQUIRE(obtainedSolution[i] > solution[i] - MARGIN_OF_ERROR_FOR_DOUBLE);
        }
    }
}