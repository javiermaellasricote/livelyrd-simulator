#include <vector>
#include <string>
#include <unordered_map>
#include "../thirdPartyLibraries/catch2/catch.hpp"
#include "../src/DataInfrastructure/UnilateralAction.h"

bool decideIfBuyingApples(const std::unordered_map<std::string, double>& inputs)
{
    return (inputs.find("0")->second <= 30.0);
}

bool buyApples(const std::unordered_map<std::string, double>& inputs,
        std::unordered_map<std::string, double>& outputs)
{
    outputs["0"] += inputs.find("0")->second;
    return true;
}

SCENARIO("Decide if action should be taken", "")
{
    GIVEN("A unilateral action to buy apples owned by 'me' ")
    {
        livelyRD::UnilateralAction toBuyApples("Buy Apples", "me", buyApples, decideIfBuyingApples);

        WHEN("The level of apples is less than 30.0")
        {
            std::unordered_map<std::string, double> currentNumberOfApples;
            currentNumberOfApples["0"] = 10.3;

            AND_WHEN("Is called by owner")
            {
                THEN("Apples should be bought")
                {
                    REQUIRE(toBuyApples.decideIfTakingAction(currentNumberOfApples, "me"));
                    REQUIRE(toBuyApples.getIsToBePerformed("me"));
                }
            }

            AND_WHEN("Is called by other")
            {
                THEN("Apples should NOT be bought")
                {
                    REQUIRE(!toBuyApples.decideIfTakingAction(currentNumberOfApples, ""));
                    REQUIRE(!toBuyApples.getIsToBePerformed("me"));
                }
            }

            AND_WHEN("Function is called by owner but get() is called by other")
            {
                THEN("Apples should be bought, but won-t be shown")
                {
                    REQUIRE(toBuyApples.decideIfTakingAction(currentNumberOfApples, "me"));
                    REQUIRE(!toBuyApples.getIsToBePerformed(""));
                }
            }
        }

        WHEN("The level of apples is more than 30.0")
        {
            std::unordered_map<std::string, double> currentNumberOfApples;
            currentNumberOfApples["0"] = 30.3;

            AND_WHEN("Is called by owner")
            {
                THEN("Apples should NOT be bought")
                {
                    REQUIRE(!toBuyApples.decideIfTakingAction(currentNumberOfApples, "me"));
                    REQUIRE(!toBuyApples.getIsToBePerformed("me"));
                }
            }

            AND_WHEN("Is called by other")
            {
                THEN("Apples should NOT be bought")
                {
                    REQUIRE(!toBuyApples.decideIfTakingAction(currentNumberOfApples, ""));
                    REQUIRE(!toBuyApples.getIsToBePerformed("me"));
                }
            }
        }
    }
}

SCENARIO("Reset value of isToBePerformed", "")
{
    GIVEN("A unilateral action to buy apples owned by 'me' ")
    {
        livelyRD::UnilateralAction toBuyApples("Buy Apples", "me", buyApples, decideIfBuyingApples);

        WHEN("The action is to be performed.")
        {
            std::unordered_map<std::string, double> currentNumberOfApples;
            currentNumberOfApples["0"] = 10.3;
            toBuyApples.decideIfTakingAction(currentNumberOfApples, "me");

            AND_WHEN("The value of isToBePerformed has been reset by owner")
            {
                toBuyApples.resetIsToBePerformed("me");

                THEN("The action should NOT be performed")
                {
                    REQUIRE(!toBuyApples.getIsToBePerformed("me"));
                }
            }

            AND_WHEN("The value of isToBePerformed has been reset by other")
            {
                toBuyApples.resetIsToBePerformed("");

                THEN("The action should be performed")
                {
                    REQUIRE(toBuyApples.getIsToBePerformed("me"));
                }
            }
        }
    }
}

SCENARIO("Perform action", "")
{
    GIVEN("A unilateral action to buy apples owned by 'me' ")
    {
        livelyRD::UnilateralAction toBuyApples("Buy Apples", "me", buyApples, decideIfBuyingApples);

        WHEN("The action is to be performed.")
        {
            std::unordered_map<std::string, double> currentNumberOfApples;
            currentNumberOfApples["0"] = 10.3;
            toBuyApples.decideIfTakingAction(currentNumberOfApples, "me");

            AND_WHEN("Is called by owner")
            {
                std::unordered_map<std::string, double> applesToBuy;
                applesToBuy["0"] = 4.2;
                REQUIRE(toBuyApples.performAction(applesToBuy, currentNumberOfApples, "me"));

                THEN("The current number of apples must be the sum")
                {
                    REQUIRE(currentNumberOfApples["0"] == 14.5);

                    AND_THEN("isToBePerformed must be false")
                    {
                        REQUIRE(!toBuyApples.getIsToBePerformed("me"));
                    }
                }
            }

            AND_WHEN("Is called by other")
            {
                std::unordered_map<std::string, double> applesToBuy;
                applesToBuy["0"] = 4.2;
                REQUIRE(!toBuyApples.performAction(applesToBuy, currentNumberOfApples, ""));

                THEN("The current number of apples must NOT have change")
                {
                    REQUIRE(currentNumberOfApples["0"] == 10.3);
                    AND_THEN("isToBePerformed must be true")
                    {
                        REQUIRE(toBuyApples.getIsToBePerformed("me"));
                    }
                }
            }
        }
    }
}