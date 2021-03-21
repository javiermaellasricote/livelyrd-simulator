#include <memory>
#include <vector>
#include <string>
#include <functional>
#include <algorithm>
#include <unordered_map>
#include "../thirdPartyLibraries/catch2/catch.hpp"
#define private public
#include "../src/DataInfrastructure/AbstractAction.h"
#include "../src/DataInfrastructure/Player.h"
#include "../src/DataInfrastructure/Resource.h"
#include "../src/DataInfrastructure/MessageMultilateralAction.h"
#include "../src/DataInfrastructure/MultilateralAction.h"
#include "../src/DataInfrastructure/Environment.h"
#include "../src/DataInfrastructure/UnilateralAction.h"
#include "DefinitionsTestEnvironment.h"

namespace TestEnvironment
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

    bool decideTrue(const std::unordered_map<std::string, double>& inputs)
    {
        return true;
    }

    bool getSpoonsPlayer1(const std::unordered_map<std::string, double>& inputs,std::unordered_map<std::string,
            double>& outputs)
    {
        std::string playerName = SPOON_FOR_FORKS_TEST_PLAYER1_NAME;
        double amountToExchange = inputs.find(SPOON_FOR_FORKS_TEST_ACTION1_NAME + std::to_string(0))->second;
        outputs[playerName + "_" + SPOON_FOR_FORKS_TEST_RESOURCE2_NAME] += amountToExchange;
        outputs[playerName + "_" + SPOON_FOR_FORKS_TEST_RESOURCE1_NAME] -= amountToExchange;
        return true;
    }

    bool getForksPlayer1(const std::unordered_map<std::string, double>& inputs,std::unordered_map<std::string,
            double>& outputs)
    {
        std::string playerName = SPOON_FOR_FORKS_TEST_PLAYER1_NAME;
        double amountToExchange = inputs.find(SPOON_FOR_FORKS_TEST_ACTION2_NAME + std::to_string(0))->second;
        outputs[playerName + "_" + SPOON_FOR_FORKS_TEST_RESOURCE1_NAME] += amountToExchange;
        outputs[playerName + "_" + SPOON_FOR_FORKS_TEST_RESOURCE2_NAME] -= amountToExchange;
        return true;
    }

    bool getSpoonsPlayer2(const std::unordered_map<std::string, double>& inputs,std::unordered_map<std::string,
            double>& outputs)
    {
        std::string playerName = SPOON_FOR_FORKS_TEST_PLAYER2_NAME;
        double amountToExchange = inputs.find(SPOON_FOR_FORKS_TEST_ACTION1_NAME + std::to_string(0))->second;
        outputs[playerName + "_" + SPOON_FOR_FORKS_TEST_RESOURCE2_NAME] += amountToExchange;
        outputs[playerName + "_" + SPOON_FOR_FORKS_TEST_RESOURCE1_NAME] -= amountToExchange;
        return true;
    }

    bool getForksPlayer2(const std::unordered_map<std::string, double>& inputs,std::unordered_map<std::string,
            double>& outputs)
    {
        std::string playerName = SPOON_FOR_FORKS_TEST_PLAYER2_NAME;
        double amountToExchange = inputs.find(SPOON_FOR_FORKS_TEST_ACTION2_NAME + std::to_string(0))->second;
        outputs[playerName + "_" + SPOON_FOR_FORKS_TEST_RESOURCE1_NAME] += amountToExchange;
        outputs[playerName + "_" + SPOON_FOR_FORKS_TEST_RESOURCE2_NAME] -= amountToExchange;
        return true;
    }

    double objectiveSpoonsForksPlayer1(const std::unordered_map<std::string, double>& inputs)
    {
        std::string forksName = SPOON_FOR_FORKS_TEST_RESOURCE1_NAME;
        std::string spoonsName = SPOON_FOR_FORKS_TEST_RESOURCE2_NAME;
        std::string player1Name = SPOON_FOR_FORKS_TEST_PLAYER1_NAME;
        std::string player2Name = SPOON_FOR_FORKS_TEST_PLAYER2_NAME;

        std::string action1Name = SPOON_FOR_FORKS_TEST_ACTION1_NAME;
        std::string action2Name = SPOON_FOR_FORKS_TEST_ACTION2_NAME;

        double spoonsPlayer1 = inputs.find(player1Name + "_" + spoonsName)->second;
        double forksPlayer1 = inputs.find(player1Name + "_" + forksName)->second;

        for(std::size_t i = 0; i < inputs.find(PLAYER_NUMBER_OF_TIME_STEPS_NAME)->second; ++i)
        {
            spoonsPlayer1 += inputs.find(action1Name + std::to_string(i))->second;
            forksPlayer1 -= inputs.find(action1Name + std::to_string(i))->second;

            spoonsPlayer1 -= inputs.find(action2Name + std::to_string(i))->second;
            forksPlayer1 += inputs.find(action2Name + std::to_string(i))->second;
        }

        return spoonsPlayer1 * forksPlayer1 +
                inputs.find(player2Name + "_" + spoonsName)->second *
                inputs.find(player2Name + "_" + forksName)->second;
    }

    double objectiveSpoonsForksPlayer2(const std::unordered_map<std::string, double>& inputs)
    {
        std::string forksName = SPOON_FOR_FORKS_TEST_RESOURCE1_NAME;
        std::string spoonsName = SPOON_FOR_FORKS_TEST_RESOURCE2_NAME;
        std::string player1Name = SPOON_FOR_FORKS_TEST_PLAYER1_NAME;
        std::string player2Name = SPOON_FOR_FORKS_TEST_PLAYER2_NAME;

        std::string action1Name = SPOON_FOR_FORKS_TEST_ACTION1_NAME;
        std::string action2Name = SPOON_FOR_FORKS_TEST_ACTION2_NAME;

        double spoonsPlayer2 = inputs.find(player2Name + "_" + spoonsName)->second;
        double forksPlayer2 = inputs.find(player2Name + "_" + forksName)->second;

        for(std::size_t i = 0; i < inputs.find(PLAYER_NUMBER_OF_TIME_STEPS_NAME)->second; ++i)
        {
            spoonsPlayer2 += inputs.find(action1Name + std::to_string(i))->second;
            forksPlayer2 -= inputs.find(action1Name + std::to_string(i))->second;

            spoonsPlayer2 -= inputs.find(action2Name + std::to_string(i))->second;
            forksPlayer2 += inputs.find(action2Name + std::to_string(i))->second;
        }

        return spoonsPlayer2 * forksPlayer2 +
               inputs.find(player1Name + "_" + spoonsName)->second *
               inputs.find(player1Name + "_" + forksName)->second;
    }

    livelyRD::DycorsOptimizer::PopulationElement selectParetoPoint(
            const std::vector<livelyRD::DycorsOptimizer::PopulationElement>& solutions)
    {
        return *std::max_element(solutions.begin(), solutions.end(), []
        (const livelyRD::DycorsOptimizer::PopulationElement& a, const livelyRD::DycorsOptimizer::PopulationElement& b)
        {
            return (a.solutionValues.begin()->second) < (a.solutionValues.begin()->second);
        });
    }
}


SCENARIO("Interface of Environment", "")
{
    GIVEN("An Environment named 'World' ")
    {
        std::string environmentName = "World";
        livelyRD::Environment world(environmentName);
        bool success;

        WHEN("A Player is added")
        {
            std::string player1Name = "Player 1";
            std::unique_ptr<livelyRD::Player> player1;
            player1 = std::make_unique<livelyRD::Player>(player1Name, environmentName);
            success = world.addPlayer(player1Name, player1);

            THEN("Player should be inside Environment")
            {
                REQUIRE(success);
                REQUIRE(world.listOfPlayers_[player1Name] != nullptr);
            }
        }

        WHEN("A resource is added")
        {
            std::string resourceName1 = "Resource 1";
            std::unique_ptr<livelyRD::Resource> resource1;
            resource1 = std::make_unique<livelyRD::Resource>(resourceName1, environmentName);
            success = world.addExternalResource(resourceName1, resource1);

            THEN("We can get Resource from environment")
            {
                REQUIRE(success);
                REQUIRE(world.getExternalResource(resourceName1) != nullptr);
            }
        }

        GIVEN("A message, a sender player and two receiver players")
        {
            std::string senderName = "sender player";
            std::vector<std::string> nameReceivers{"receiver 1", "receiver 2"};
            std::string actionName = "the action";
            livelyRD::MessageMultilateralAction messageCreate;
            std::unique_ptr<livelyRD::Player> receiver1 =
                    std::make_unique<livelyRD::Player>(nameReceivers[0], environmentName);
            std::unique_ptr<livelyRD::Player> receiver2 =
                    std::make_unique<livelyRD::Player>(nameReceivers[1], environmentName);
            std::unique_ptr<livelyRD::AbstractAction> action1;
            action1 = std::make_unique<livelyRD::MultilateralAction>
                    (actionName, nameReceivers[0], TestEnvironment::buyApples, TestEnvironment::decideIfBuyingApples);
            std::unique_ptr<livelyRD::AbstractAction> action2 =
                    std::make_unique<livelyRD::MultilateralAction>
                    (actionName, nameReceivers[1], TestEnvironment::buyApples, TestEnvironment::decideIfBuyingApples);
            receiver1->addAction(action1);
            receiver2->addAction(action2);

            world.addPlayer(nameReceivers[0], receiver1);
            world.addPlayer(nameReceivers[1], receiver2);

            messageCreate.sender = senderName;
            messageCreate.recipients = nameReceivers;
            messageCreate.recipientActionName = actionName;
            std::shared_ptr<livelyRD::MessageMultilateralAction> message =
                    std::make_shared<livelyRD::MessageMultilateralAction>(messageCreate);

            WHEN("A message is sent")
            {
                success = world.sendMessage(message);

                THEN("The message must have been received by recipients")
                {
                    REQUIRE(success);
                    REQUIRE(dynamic_cast<livelyRD::MultilateralAction&>
                            (* world.listOfPlayers_[nameReceivers[0]]->
                                    actions_.value[actionName]).messages_.find(nameReceivers[0]) !=
                            dynamic_cast<livelyRD::MultilateralAction&>
                            (* world.listOfPlayers_[nameReceivers[0]]->
                                    actions_.value[actionName]).messages_.end());
                    REQUIRE(dynamic_cast<livelyRD::MultilateralAction&>
                            (* world.listOfPlayers_[nameReceivers[1]]->
                            actions_.value[actionName]).messages_.find(nameReceivers[1]) !=
                            dynamic_cast<livelyRD::MultilateralAction&>
                            (* world.listOfPlayers_[nameReceivers[1]]->
                            actions_.value[actionName]).messages_.end());
                }
            }
        }
    }
}

SCENARIO("Changing spoons for forks", "")
{
    GIVEN("An Environment with two players trading resources NOT with each other")
    {
        std::string environmentName = SPOON_FOR_FORKS_TEST_ENVIRONMNET_NAME;
        std::string player1Name = SPOON_FOR_FORKS_TEST_PLAYER1_NAME;
        std::string player2Name = SPOON_FOR_FORKS_TEST_PLAYER2_NAME;
        auto environment = std::make_unique<livelyRD::Environment>(environmentName);
        auto player1 = std::make_unique<livelyRD::Player>(player1Name, environmentName);
        auto player2 = std::make_unique<livelyRD::Player>(player2Name, environmentName);

        std::string spoonsName = SPOON_FOR_FORKS_TEST_RESOURCE2_NAME;
        double initialSpoonsPlayer1 = SPOON_FOR_FORKS_TEST_PLAYER1_RESOURCE2_INITIAL_QUANTITY;
        double initialSpoonsPlayer2 = SPOON_FOR_FORKS_TEST_PLAYER2_RESOURCE2_INITIAL_QUANTITY;
        std::string forksName = SPOON_FOR_FORKS_TEST_RESOURCE1_NAME;
        double initialForksPlayer1 = SPOON_FOR_FORKS_TEST_PLAYER1_RESOURCE1_INITIAL_QUANTITY;
        double initialForksPlayer2 = SPOON_FOR_FORKS_TEST_PLAYER2_RESOURCE1_INITIAL_QUANTITY;
        auto spoonsPlayer1 = std::make_unique<livelyRD::Resource>(spoonsName, player1Name, initialSpoonsPlayer1);
        auto spoonsPlayer2 = std::make_unique<livelyRD::Resource>(spoonsName, player2Name, initialSpoonsPlayer2);
        auto forksPlayer1 = std::make_unique<livelyRD::Resource>(forksName, player1Name, initialForksPlayer1);
        auto forksPlayer2 = std::make_unique<livelyRD::Resource>(forksName, player2Name, initialForksPlayer2);

        std::string getSpoonsName = SPOON_FOR_FORKS_TEST_ACTION1_NAME;
        double getSpoonsLowerBound = SPOON_FOR_FORKS_TEST_ACTION1_LOWER_BOUND;
        double getSpoonsUpperBound = SPOON_FOR_FORKS_TEST_ACTION1_UPPER_BOUND;
        std::string getForksName = SPOON_FOR_FORKS_TEST_ACTION2_NAME;
        double getForksLoweBound = SPOON_FOR_FORKS_TEST_ACTION2_LOWER_BOUND;
        double getForksUpperBound = SPOON_FOR_FORKS_TEST_ACTION2_UPPER_BOUND;
        std::unique_ptr<livelyRD::AbstractAction> getMoreSpoonsPlayer1 =
                std::make_unique<livelyRD::UnilateralAction>(getSpoonsName, player1Name, TestEnvironment::getSpoonsPlayer1,
                TestEnvironment::decideTrue, false, getSpoonsLowerBound, getSpoonsUpperBound);
        std::unique_ptr<livelyRD::AbstractAction> getMoreSpoonsPlayer2 =
                std::make_unique<livelyRD::UnilateralAction>(getSpoonsName, player2Name, TestEnvironment::getSpoonsPlayer2,
                TestEnvironment::decideTrue, false, getSpoonsLowerBound, getSpoonsUpperBound);
        std::unique_ptr<livelyRD::AbstractAction> getMoreForksPlayer1 =
                std::make_unique<livelyRD::UnilateralAction>(getForksName, player1Name, TestEnvironment::getForksPlayer1,
                TestEnvironment::decideTrue, false, getForksLoweBound, getForksUpperBound);
        std::unique_ptr<livelyRD::AbstractAction> getMoreForksPlayer2 =
                std::make_unique<livelyRD::UnilateralAction>(getForksName, player2Name, TestEnvironment::getForksPlayer2,
                TestEnvironment::decideTrue, false, getForksLoweBound, getForksUpperBound);

        std::string objectiveName = SPOON_FOR_FORKS_TEST_OBJECTIVE_NAME;
        auto objectivePlayer1 = std::make_unique<livelyRD::Objective>(objectiveName, player1Name,
                TestEnvironment::objectiveSpoonsForksPlayer1, 0.0, true);
        auto objectivePlayer2 = std::make_unique<livelyRD::Objective>(objectiveName, player2Name,
                TestEnvironment::objectiveSpoonsForksPlayer2, 0.0, true);

        player1->addResource(spoonsPlayer1);
        player1->addResource(forksPlayer1);
        player1->setResourcesListPublic(environmentName);
        player1->addAction(getMoreSpoonsPlayer1);
        player1->addAction(getMoreForksPlayer1);
        player1->addObjective(objectivePlayer1);
        player1->addPreferences(TestEnvironment::selectParetoPoint);

        player2->addResource(spoonsPlayer2);
        player2->addResource(forksPlayer2);
        player2->setResourcesListPublic(environmentName);
        player2->addAction(getMoreSpoonsPlayer2);
        player2->addAction(getMoreForksPlayer2);
        player2->addObjective(objectivePlayer2);
        player2->addPreferences(TestEnvironment::selectParetoPoint);

        environment->setNumberOfTimeSteps(SPOON_FOR_FORKS_TEST_NUMBER_OF_TIME_STEPS);
        environment->addPlayer(SPOON_FOR_FORKS_TEST_PLAYER1_NAME, player1);
        environment->addPlayer(SPOON_FOR_FORKS_TEST_PLAYER2_NAME, player2);

        WHEN("The environment is solved")
        {
            REQUIRE(environment->solve());

            THEN("The resources of each player should be the following")
            {
                const std::unordered_map<std::string, std::unique_ptr<livelyRD::Player>>& players =
                        environment->getAllPlayers();

                REQUIRE(players.find(player1Name)->second->getResource(spoonsName)->getQuantity(player1Name) > 14.0);
                REQUIRE(players.find(player1Name)->second->getResource(spoonsName)->getQuantity(player1Name) < 16.0);
                REQUIRE(players.find(player1Name)->second->getResource(forksName)->getQuantity(player1Name) > 84.0);
                REQUIRE(players.find(player1Name)->second->getResource(forksName)->getQuantity(player1Name) < 86.0);

                REQUIRE(players.find(player2Name)->second->getResource(spoonsName)->getQuantity(player2Name) > 84.0);
                REQUIRE(players.find(player2Name)->second->getResource(spoonsName)->getQuantity(player2Name) < 86.0);
                REQUIRE(players.find(player2Name)->second->getResource(forksName)->getQuantity(player2Name) > 14.0);
                REQUIRE(players.find(player2Name)->second->getResource(forksName)->getQuantity(player2Name) < 16.0);
            }
        }
    }
}