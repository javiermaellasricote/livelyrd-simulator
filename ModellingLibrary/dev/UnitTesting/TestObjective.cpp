#include <vector>
#include <unordered_map>
#include "../thirdPartyLibraries/catch2/catch.hpp"
#include "../src/DataInfrastructure/Objective.h"

double calculateTotalProfits(const std::unordered_map<std::string, double>& profits)
{
    double sum = 0.0;
    for(const auto& profit : profits)
    {
        sum += profit.second;
    }
    return sum;
}

SCENARIO("Show name of Objective", "")
{
    GIVEN("An objective named 'Total Profits' owned by 'me' ")
    {
        std::string objectiveName = "Oil Fields Profits";
        std::string ownerName = "me";
        livelyRD::Objective totalProfits(objectiveName, ownerName, calculateTotalProfits);

        WHEN("The name is public variable")
        {
            totalProfits.setNamePublic("me");
            THEN("The name should be retrieved")
            {
                REQUIRE(totalProfits.getName("") == objectiveName);
            }
        }

        WHEN("The name is private variable")
        {
            totalProfits.setNamePrivate("me");

            AND_WHEN("It is called by owner")
            {
                THEN("The name should be retrieved")
                {
                    REQUIRE(totalProfits.getName("me") == objectiveName);
                }
            }

            AND_WHEN("It is called by other player")
            {
                THEN("The name should NOT be retrieved")
                {
                    REQUIRE(totalProfits.getName("") == "");
                }
            }
        }
    }
}

SCENARIO("Show current value of Objective", "")
{
    GIVEN("An objective named 'Total Profits' owned by 'me' ")
    {
        std::string objectiveName = "Oil Fields Profits";
        std::string ownerName = "me";
        livelyRD::Objective totalProfits(objectiveName, ownerName, calculateTotalProfits);

        WHEN("The current value is public variable")
        {
            totalProfits.setCurrentValuePublic("me");
            THEN("The Current Value should be retrieved")
            {
                REQUIRE(totalProfits.getCurrentValue("") == 0.0);
            }
        }

        WHEN("The CurrentValue is private variable")
        {
            totalProfits.setCurrentValuePrivate("me");

            AND_WHEN("It is called by owner")
            {
                THEN("The Current Value should be retrieved")
                {
                    REQUIRE(totalProfits.getCurrentValue("me") == 0.0);
                }
            }

            AND_WHEN("It is called by other player")
            {
                THEN("The Current Value should NOT be retrieved")
                {
                    REQUIRE(totalProfits.getCurrentValue("") == -1.0);
                }
            }
        }

        WHEN("The Current Value is calculated by owner")
        {
            std::unordered_map<std::string, double> inputs;
            inputs["1"] = 1.1;
            inputs["2"] = 2.0;
            inputs["3"] = 3.0;
            inputs["4"] = 4.0;
            inputs["5"] = 5.0;
            bool success = totalProfits.calculateObjective(inputs, ownerName);

            THEN("The current value should have changed")
            {
                REQUIRE(success);
                REQUIRE(totalProfits.getCurrentValue("me") == 15.1);
            }
        }

        WHEN("The Current Value is calculated by other")
        {
            std::unordered_map<std::string, double> inputs;
            inputs["1"] = 1.1;
            inputs["2"] = 2.0;
            inputs["3"] = 3.0;
            inputs["4"] = 4.0;
            inputs["5"] = 5.0;
            bool success = totalProfits.calculateObjective(inputs, "");

            THEN("The current value should have changed")
            {
                REQUIRE(!success);
                REQUIRE(totalProfits.getCurrentValue("me") == 0.0);
            }
        }
    }
}

