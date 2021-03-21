#include <vector>
#include <string>
#include <unordered_map>
#include "../thirdPartyLibraries/catch2/catch.hpp"
#include "../src/DataInfrastructure/UnilateralAction.h"
#include "../src/DataInfrastructure/Environment.h"
#define private public

#include <memory>
#include "../src/DataInfrastructure/MultilateralAction.h"

namespace Test_MultilateralAction
{
    bool decideIfBuyingApples(const std::unordered_map<std::string, double>& inputs,
            std::unordered_map<std::string, std::vector<std::shared_ptr<livelyRD::MessageMultilateralAction>>>&
                    messages)
    {
        return (inputs.find("0")->second <= 30.0);
    }

    bool buyApples(const std::unordered_map<std::string, double>& inputs,
            std::unordered_map<std::string, double>& outputs, std::unordered_map<std::string,
            std::vector<std::shared_ptr<livelyRD::MessageMultilateralAction>>>& messages,
            std::unordered_map<std::string, std::unique_ptr<livelyRD::UnilateralAction>>& availableActions)
    {
        outputs["0"] += inputs.find("0")->second;
        return true;
    }

    bool decideIfPerformTransaction(const std::unordered_map<std::string, double>& inputs)
    {
        return (inputs.find("0")->second <= 30.0);
    }

    bool performTransaction(const std::unordered_map<std::string, double>& inputs,
            std::unordered_map<std::string, double>& outputs)
    {
        outputs["0"] += inputs.find("0")->second;
        return true;
    }
}

SCENARIO("Use of a multilateral action object", "")
{
    GIVEN("A multilateral action named: 'Trade for apples', owned by 'me' ")
    {
        std::string multilateralActionName = "Trade for apples";
        livelyRD::MultilateralAction tradeForApples
        (
            "Trade for apples",
            "me",
            Test_MultilateralAction::buyApples,
            Test_MultilateralAction::decideIfBuyingApples
        );

        std::shared_ptr<livelyRD::Environment> currentEnvironment;
        currentEnvironment = std::make_shared<livelyRD::Environment>("myEnvironment");
        tradeForApples.setCurrentEnvironment(currentEnvironment);

        AND_GIVEN("A unilateral action to perform the transaction owned by the multilateral action")
        {
            std::unique_ptr<livelyRD::UnilateralAction> performTransaction;
            performTransaction = std::make_unique<livelyRD::UnilateralAction>(
                "Perform transaction",
                "Trade for apples",
                Test_MultilateralAction::performTransaction,
                Test_MultilateralAction::decideIfPerformTransaction
            );

            WHEN("The unilateral action is added to the multilateral list of actions by owner")
            {
                bool success;
                success = tradeForApples.addActionToList(
                        "Make transaction",performTransaction, "me");

                THEN("The action should be inside the list of actions.")
                {
                    REQUIRE(success);
                    REQUIRE("Perform transaction" == tradeForApples.actionsToPerform_.value["Make transaction"]->
                    getName("Trade for apples"));
                }

                AND_WHEN("The action is removed from the list.")
                {
                    success = tradeForApples.eliminateActionFromList("Make transaction",
                            "me");

                    THEN("The action should NOT exist inside the multilateral action")
                    {
                        REQUIRE(success);
                        REQUIRE(tradeForApples.actionsToPerform_.value["Make transaction"] == nullptr);
                    }
                }
            }

            WHEN("The unilateral action is added to the multilateral list of actions by other")
            {
                bool success;
                success = tradeForApples.addActionToList(
                        "Make transaction",performTransaction, "you");

                THEN("The action should NOT be inside the list of actions.")
                {
                    REQUIRE(!success);
                    REQUIRE(tradeForApples.actionsToPerform_.value["Make transaction"] == nullptr);
                }

                AND_WHEN("The action is removed from the list.")
                {
                    success = tradeForApples.eliminateActionFromList("Make transaction",
                            "me");

                    THEN("The action should NOT exist inside the multilateral action")
                    {
                        REQUIRE(!success);
                        REQUIRE(tradeForApples.actionsToPerform_.value["Make transaction"] == nullptr);
                    }
                }
            }
        }

        WHEN("The level of apples is less than 30.0")
        {
            std::unordered_map<std::string, double> currentNumberOfApples;
            currentNumberOfApples["0"] = 10.3;

            AND_WHEN("Is called by owner")
            {
                THEN("Apples should be bought")
                {
                    REQUIRE(tradeForApples.decideIfTakingAction(currentNumberOfApples, "me"));
                    REQUIRE(tradeForApples.getIsToBePerformed("me"));
                }
            }

            AND_WHEN("Is called by other")
            {
                THEN("Apples should NOT be bought")
                {
                    REQUIRE(!tradeForApples.decideIfTakingAction(currentNumberOfApples, ""));
                    REQUIRE(!tradeForApples.getIsToBePerformed("me"));
                }
            }

            AND_WHEN("Function is called by owner but get() is called by other")
            {
                THEN("Apples should be bought, but won-t be shown")
                {
                    REQUIRE(tradeForApples.decideIfTakingAction(currentNumberOfApples, "me"));
                    REQUIRE(!tradeForApples.getIsToBePerformed(""));
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
                    REQUIRE(!tradeForApples.decideIfTakingAction(currentNumberOfApples, "me"));
                    REQUIRE(!tradeForApples.getIsToBePerformed("me"));
                }
            }

            AND_WHEN("Is called by other")
            {
                THEN("Apples should NOT be bought")
                {
                    REQUIRE(!tradeForApples.decideIfTakingAction(currentNumberOfApples, ""));
                    REQUIRE(!tradeForApples.getIsToBePerformed("me"));
                }
            }
        }

        WHEN("The action is to be performed.")
        {
            std::unordered_map<std::string, double> currentNumberOfApples;
            currentNumberOfApples["0"] = 10.3;
            tradeForApples.decideIfTakingAction(currentNumberOfApples, "me");

            AND_WHEN("The value of isToBePerformed has been reset by owner")
            {
                tradeForApples.resetIsToBePerformed("me");

                THEN("The action should NOT be performed")
                {
                    REQUIRE(!tradeForApples.getIsToBePerformed("me"));
                }
            }

            AND_WHEN("The value of isToBePerformed has been reset by other")
            {
                tradeForApples.resetIsToBePerformed("");

                THEN("The action should be performed")
                {
                    REQUIRE(tradeForApples.getIsToBePerformed("me"));
                }
            }
        }

        WHEN("The action is performed.")
        {
            std::unordered_map<std::string, double> currentNumberOfApples;
            currentNumberOfApples["0"] = 10.3;
            tradeForApples.decideIfTakingAction(currentNumberOfApples, "me");

            AND_WHEN("Is called by owner")
            {
                std::unordered_map<std::string, double> applesToBuy;
                applesToBuy["0"] = 4.2;
                REQUIRE(tradeForApples.performAction(applesToBuy, currentNumberOfApples, "me"));

                THEN("The current number of apples must be the sum")
                {
                    REQUIRE(currentNumberOfApples["0"] == 14.5);

                    AND_THEN("isToBePerformed must be false")
                    {
                        REQUIRE(!tradeForApples.getIsToBePerformed("me"));
                    }
                }
            }

            AND_WHEN("Is called by other")
            {
                std::unordered_map<std::string, double> applesToBuy;
                applesToBuy["0"] = 4.3;
                REQUIRE(!tradeForApples.performAction(applesToBuy, currentNumberOfApples, ""));

                THEN("The current number of apples must NOT have change")
                {
                    REQUIRE(currentNumberOfApples["0"] == 10.3);
                    AND_THEN("isToBePerformed must be true")
                    {
                        REQUIRE(tradeForApples.getIsToBePerformed("me"));
                    }
                }
            }
        }

        WHEN("A subscriber is added.")
        {
            bool success;
            std::string otherPlayerName = "Other player";
            success = tradeForApples.addSubscriber(otherPlayerName, "me");

            THEN("The subscriber should be in the list")
            {
                REQUIRE(success);
            }

            AND_WHEN("The value of the subscriber is changed by owner.")
            {
                success = tradeForApples.changeSubscriberValue(5.4,otherPlayerName, "me");

                THEN("The value should have changed")
                {
                    REQUIRE(success);
                }
            }

            AND_WHEN("The value of the subscriber is changed by other.")
            {
                success = tradeForApples.changeSubscriberValue(5.4,otherPlayerName, "you");

                THEN("The value should NOT have changed")
                {
                    REQUIRE(!success);
                }
            }

            AND_WHEN("The subscriber is eliminated by owner.")
            {
                success = tradeForApples.eliminateSubscriber(otherPlayerName, "me");

                THEN("The value should have changed")
                {
                    REQUIRE(success);
                }
            }

            AND_WHEN("The subscriber is eliminated by other.")
            {
                success = tradeForApples.eliminateSubscriber(otherPlayerName, "you");

                THEN("The value should have changed")
                {
                    REQUIRE(!success);
                }
            }
        }
    }
}