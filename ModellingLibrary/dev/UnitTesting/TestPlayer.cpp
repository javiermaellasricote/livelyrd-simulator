#include <memory>
#include <functional>
#include <vector>
#include <unordered_map>
#include <string>
#include <cmath>
#include "../thirdPartyLibraries/catch2/catch.hpp"
#include "../src/DataInfrastructure/Player.h"
#include "../src/DataInfrastructure/UnilateralAction.h"
#include "DefinitionsTestPlayer.h"

double calculateTotalProfits1(const std::unordered_map<std::string, double>& profits)
{
    double sum = 0.0;
    for(const auto& profit : profits)
    {
        sum += profit.second;
    }
    return sum;
}

bool decideIfBuyingApples1(const std::unordered_map<std::string, double>& inputs)
{
    return (inputs.find("0")->second <= 30.0);
}

bool buyApples1(const std::unordered_map<std::string, double>& inputs,
        std::unordered_map<std::string, double>& outputs)
{
    outputs[0] += inputs.find("0")->second;
    return true;
}

namespace TestPlayer
{
    std::function<double(const std::unordered_map<std::string, double>& inputs)> calculateTime =
            [](const std::unordered_map<std::string, double>& inputs)
    {
        double totalTime = 0.0;
        std::string playerName = TIME_VS_MONEY_PLAYER_NAME;
        std::string timeResourceName = TIME_VS_MONEY_TIME_RESOURCE_NAME;
        for(std::size_t i = 0; i < inputs.find(playerName + timeResourceName)->second; ++i)
        {
            totalTime += inputs.find(TIME_VS_MONEY_INVEST_TIME_ACTION_NAME + std::to_string(i))->second;
        }
        return totalTime;
    };

    std::function<double(const std::unordered_map<std::string, double>& inputs)> calculateMoney =
            [](const std::unordered_map<std::string, double>& inputs) -> double
    {
        std::string playerName = TIME_VS_MONEY_PLAYER_NAME;
        std::string timeResourceName = TIME_VS_MONEY_TIME_RESOURCE_NAME;
        std::string knowledgeResourceName = TIME_VS_MONEY_KNOWLEDGE_RESOURCE_NAME;
        std::string moneyResourceName = TIME_VS_MONEY_MONEY_RESOURCE_NAME;

        double totalMoney = inputs.find(playerName + moneyResourceName)->second;
        double knowledge = inputs.find(playerName + knowledgeResourceName)->second;
        for(std::size_t i = 0; i < inputs.find(playerName + timeResourceName)->second; ++i)
        {
            totalMoney += totalMoney*(1 - std::exp(-knowledge/10.0))*
                          (inputs.find(TIME_VS_MONEY_INVEST_TIME_ACTION_NAME+std::to_string(i))
                                   ->second
                           - inputs.find(TIME_VS_MONEY_STUDY_ACTION_NAME+std::to_string(i))
                                   ->second);

            knowledge += inputs.find(TIME_VS_MONEY_STUDY_ACTION_NAME+
                                     std::to_string(i))->second;
        }
        return totalMoney;
    };

    std::function<bool(const std::unordered_map<std::string, double>& inputs)> decideIfInvestingTime =
            [](const std::unordered_map<std::string, double>& inputs){return true;};

    std::function<bool(const std::unordered_map<std::string, double>& inputs,
            std::unordered_map<std::string, double>& outputs)> investTime =
            [](const std::unordered_map<std::string, double>& inputs,std::unordered_map<std::string, double>& outputs)
    {
        outputs[TIME_VS_MONEY_TIME_RESOURCE_NAME] -= 1.0;
        return true;
    };

    std::function<bool(const std::unordered_map<std::string, double>& inputs)> decideIfStudy =
            [](const std::unordered_map<std::string, double>& inputs) {return true;};

    std::function<bool(const std::unordered_map<std::string, double>& inputs,
                       std::unordered_map<std::string, double>& outputs)> study =
            [](const std::unordered_map<std::string, double>& inputs,
               std::unordered_map<std::string, double>& outputs)
    {
        outputs[TIME_VS_MONEY_KNOWLEDGE_RESOURCE_NAME] +=
                inputs.find(TIME_VS_MONEY_STUDY_ACTION_NAME + std::to_string(0))->second;
        return true;
    };

    std::function<livelyRD::DycorsOptimizer::PopulationElement
            (const std::vector<livelyRD::DycorsOptimizer::PopulationElement>& solutions)> timeStepPreferences =
            [](const std::vector<livelyRD::DycorsOptimizer::PopulationElement>& solutions)
    {
        livelyRD::DycorsOptimizer::PopulationElement result;
        result.solutionValues[TIME_VS_MONEY_TIME_OBJECTIVE_NAME] = 1.0;
        result.solutionValues[TIME_VS_MONEY_MONEY_OBJECTIVE_NAME] = 0.0;
        for(const auto& solution : solutions)
        {
            if(solution.solutionValues.find(TIME_VS_MONEY_MONEY_OBJECTIVE_NAME)->second/
               solution.solutionValues.find(TIME_VS_MONEY_TIME_OBJECTIVE_NAME)->second >=
               result.solutionValues[TIME_VS_MONEY_MONEY_OBJECTIVE_NAME]/
               result.solutionValues[TIME_VS_MONEY_TIME_OBJECTIVE_NAME])
            {
                result = solution;
            }
        }
        return result;
    };
}



SCENARIO("Change list of resources", "")
{
    GIVEN("A player named 'Pret a Manger' in 'London' environment.")
    {
        livelyRD::Player pret("Pret a Manger", "London");

        AND_GIVEN("A resource called 'Apples' and owned by pret")
        {
            std::unique_ptr<livelyRD::Resource> apples;
            apples = std::make_unique<livelyRD::Resource>("Apples", "Pret a Manger", 5.0);

            WHEN("Apples are added to Players as a resource")
            {
                bool success = pret.addResource(apples);

                AND_WHEN("Resources list is public")
                {
                    pret.setResourcesListPublic("London");

                    THEN("It is possible to retrieve the resource from the player")
                    {
                        REQUIRE(success);

                        REQUIRE(pret.getResource("Apples")->getQuantity("Pret a Manger") == 5.0);
                    }

                    AND_WHEN("Apples are removed from the player")
                    {
                        success = pret.removeResource("Apples");

                        THEN("Apples should not exist inside Player")
                        {
                            REQUIRE(success);
                            REQUIRE(pret.getResource("Apples") == nullptr);
                        }
                    }
                }

                AND_WHEN("Resources list is private")
                {
                    pret.setResourcesListPrivate("London");

                    THEN("Apples should not be accessible through Player")
                    {
                        REQUIRE(success);
                        REQUIRE(pret.getResource("Apples") == nullptr);
                    }
                }
            }
        }
    }
}

SCENARIO("Change list of objectives", "")
{
    GIVEN("A player named 'Pret a Manger' in 'London' environment.")
    {
        livelyRD::Player pret("Pret a Manger", "London");

        AND_GIVEN("An objective called 'Total profits' and owned by pret")
        {
            std::unique_ptr<livelyRD::Objective> totalProfits;
            std::string objectiveName = "Total profits";
            std::string ownerName = "Pret a Manger";
            totalProfits = std::make_unique<livelyRD::Objective>(objectiveName, ownerName,
                    calculateTotalProfits1, 5.0);

            WHEN("Total profits is added to Players as an objective")
            {
                bool success = pret.addObjective(totalProfits);

                AND_WHEN("Resources list is public")
                {
                    pret.setObjectiveListPublic("London");

                    THEN("It is possible to retrieve the objective from the player")
                    {
                        REQUIRE(success);
                        REQUIRE(pret.getObjective("Total profits")->getCurrentValue("Pret a Manger") == 5.0);
                    }

                    AND_WHEN("Total profits is removed from the player")
                    {
                        success = pret.removeObjective("Total profits");

                        THEN("Total profits should not exist inside Player")
                        {
                            REQUIRE(success);
                            REQUIRE(pret.getObjective("Total profits") == nullptr);
                        }
                    }
                }
                AND_WHEN("Objective list is private")
                {
                    pret.setObjectiveListPrivate("London");

                    THEN("Apples should not be accessible through Player")
                    {
                        REQUIRE(success);
                        REQUIRE(pret.getResource("Apples") == nullptr);
                    }
                }
            }
        }
    }
}

SCENARIO("Change list of actions", "")
{
    GIVEN("A player named 'Pret a Manger' in 'London' environment.")
    {
        livelyRD::Player pret("Pret a Manger", "London");

        AND_GIVEN("An action called 'buy apples' and owned by pret")
        {
            std::unique_ptr<livelyRD::AbstractAction> toBuyApples;
            toBuyApples = std::make_unique<livelyRD::UnilateralAction>("Buy Apples", "Pret a Manger",
                    buyApples1, decideIfBuyingApples1);

            WHEN("Apples are added to Players as a resource")
            {
                bool success = pret.addAction(toBuyApples);

                AND_WHEN("Action list is public")
                {
                    pret.setActionListPublic("London");

                    THEN("It is possible to retrieve the action from the player")
                    {
                        REQUIRE(success);
                        REQUIRE(pret.getAction("Buy Apples", "London")->getName("Pret a Manger") == "Buy Apples");
                    }

                    AND_WHEN("Action is removed from the player")
                    {
                        success = pret.removeAction("Buy apples");

                        THEN("Action should not exist inside Player")
                        {
                            REQUIRE(success);
                            REQUIRE(pret.getAction("Buy apples", "London") == nullptr);
                        }
                    }
                }

                AND_WHEN("Resources list is private")
                {
                    pret.setActionListPrivate("London");

                    THEN("Apples should not be accessible through Player")
                    {
                        REQUIRE(success);
                        REQUIRE(pret.getAction("Buy apples", "London") == nullptr);
                    }
                }
            }
        }
    }
}


SCENARIO("Time vs Money test-case")
{
    GIVEN("A player who wants to maximize its money investing the least amount of time possible")
    {
        std::string playerName = TIME_VS_MONEY_PLAYER_NAME;
        std::string playerOwner = TIME_VS_MONEY_PLAYER_OWNER;
        livelyRD::Player player(playerName, playerOwner);

        std::string moneyName = TIME_VS_MONEY_MONEY_RESOURCE_NAME;
        double initialMoney = TIME_VS_MONEY_MONEY_INITIAL_VALUE;
        std::unique_ptr<livelyRD::Resource> money;
        money = std::make_unique<livelyRD::Resource>(moneyName, playerName, initialMoney);

        std::string knowledgeName = TIME_VS_MONEY_KNOWLEDGE_RESOURCE_NAME;
        double initialKnowledge = TIME_VS_MONEY_KNOWLEDGE_INITIAL_VALUE;
        std::unique_ptr<livelyRD::Resource> knowledge;
        knowledge = std::make_unique<livelyRD::Resource>(knowledgeName, playerName, initialKnowledge);

        std::string timeName = TIME_VS_MONEY_TIME_RESOURCE_NAME;
        double initialTimeAvailable = TIME_VS_MONEY_TIME_INITIAL_VALUE;
        std::unique_ptr<livelyRD::Resource> timeAvailable;
        timeAvailable = std::make_unique<livelyRD::Resource>(timeName, playerName, initialTimeAvailable);

        std::string timeSpentName = TIME_VS_MONEY_TIME_OBJECTIVE_NAME;
        std::unique_ptr<livelyRD::Objective> timeSpent;
        timeSpent = std::make_unique<livelyRD::Objective>(timeSpentName, playerName, TestPlayer::calculateTime);

        std::string moneyGainedName = TIME_VS_MONEY_MONEY_OBJECTIVE_NAME;
        std::unique_ptr<livelyRD::Objective> moneyGained;
        moneyGained = std::make_unique<livelyRD::Objective>(moneyGainedName, playerName, TestPlayer::calculateMoney,
                0.0, true);

        std::string investTimeName = TIME_VS_MONEY_INVEST_TIME_ACTION_NAME;
        std::unique_ptr<livelyRD::AbstractAction> toInvestTime;
        toInvestTime = std::make_unique<livelyRD::UnilateralAction>(investTimeName, playerName,
                TestPlayer::investTime, TestPlayer::decideIfInvestingTime);

        std::string studyName = TIME_VS_MONEY_STUDY_ACTION_NAME;
        std::unique_ptr<livelyRD::AbstractAction> investInStudying;
        investInStudying = std::make_unique<livelyRD::UnilateralAction>(studyName, playerName,
                TestPlayer::study, TestPlayer::decideIfStudy);

        player.addResource(money);
        player.addResource(knowledge);
        player.addResource(timeAvailable);

        player.addObjective(timeSpent);
        player.addObjective(moneyGained);

        player.addAction(toInvestTime);
        player.addAction(investInStudying);

        player.setNumberOfTimeSteps(TIME_VS_MONEY_TIME_INITIAL_VALUE);
        player.addPreferences(TestPlayer::timeStepPreferences);

        WHEN("The next move is decided")
        {
            std::unordered_map<std::string, double> environmentInputs;
            environmentInputs.clear();
            REQUIRE(player.decideNextMove(environmentInputs));

            AND_WHEN("The next move gets executed")
            {
                int iterNumber = 0;
                REQUIRE(player.executeNextMove(environmentInputs, iterNumber));

                THEN("The resouces of the player should have been modified")
                {
                    REQUIRE(player.setResourcesListPublic(playerOwner));
                    REQUIRE(player.getResource(timeName)->getQuantity(playerName) < initialTimeAvailable);
                    REQUIRE(player.getResource(moneyName)->getQuantity(playerName) >= initialMoney);
                    REQUIRE(player.getResource(knowledgeName)->getQuantity(playerName) > initialKnowledge);
                }
            }
        }
    }
}
