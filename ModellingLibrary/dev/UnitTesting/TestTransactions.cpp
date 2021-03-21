#include "DefinitionsTestTransactions.h"

#include <memory>
#include <vector>
#include <string>
#include <functional>
#include <algorithm>
#include <unordered_map>
#include <cmath>
#include "../thirdPartyLibraries/catch2/catch.hpp"
#define private public
#include "../src/DataInfrastructure/AbstractAction.h"
#include "../src/DataInfrastructure/Player.h"
#include "../src/DataInfrastructure/Resource.h"
#include "../src/DataInfrastructure/MessageMultilateralAction.h"
#include "../src/DataInfrastructure/MultilateralAction.h"
#include "../src/DataInfrastructure/Environment.h"
#include "../src/DataInfrastructure/UnilateralAction.h"

namespace TestTransactions
{
    double objectiveSpoonsForksPlayer1(const std::unordered_map<std::string, double>& inputs)
    {
        std::string forksName = TRANSACTIONS_TEST_RESOURCE1_NAME;
        std::string spoonsName = TRANSACTIONS_TEST_RESOURCE2_NAME;
        std::string player1Name = TRANSACTIONS_TEST_PLAYER1_NAME;
        std::string player2Name = TRANSACTIONS_TEST_PLAYER2_NAME;

        std::string actionName = TRANSACTIONS_TEST_ACTION_NAME;

        double spoonsPlayer1 = inputs.find(player1Name + "_" + spoonsName)->second;
        double forksPlayer1 = inputs.find(player1Name + "_" + forksName)->second;

        for(std::size_t i = 0; i < inputs.find(PLAYER_NUMBER_OF_TIME_STEPS_NAME)->second; ++i)
        {
            forksPlayer1 += inputs.find(actionName + std::to_string(i))->second;
            spoonsPlayer1 -= inputs.find(actionName + std::to_string(i))->second;
        }

        return spoonsPlayer1 * forksPlayer1 +
               inputs.find(player2Name + "_" + spoonsName)->second *
               inputs.find(player2Name + "_" + forksName)->second;
    }

    double objectiveSpoonsForksPlayer2(const std::unordered_map<std::string, double>& inputs)
    {
        std::string forksName = TRANSACTIONS_TEST_RESOURCE1_NAME;
        std::string spoonsName = TRANSACTIONS_TEST_RESOURCE2_NAME;
        std::string player1Name = TRANSACTIONS_TEST_PLAYER1_NAME;
        std::string player2Name = TRANSACTIONS_TEST_PLAYER2_NAME;

        std::string action1Name = TRANSACTIONS_TEST_ACTION_NAME;

        double spoonsPlayer2 = inputs.find(player2Name + "_" + spoonsName)->second;
        double forksPlayer2 = inputs.find(player2Name + "_" + forksName)->second;

        for(std::size_t i = 0; i < inputs.find(PLAYER_NUMBER_OF_TIME_STEPS_NAME)->second; ++i)
        {
            forksPlayer2 += inputs.find(action1Name + std::to_string(i))->second;
            spoonsPlayer2 -= inputs.find(action1Name + std::to_string(i))->second;
        }

        return spoonsPlayer2 * forksPlayer2 +
               inputs.find(player1Name + "_" + spoonsName)->second *
               inputs.find(player1Name + "_" + forksName)->second;
    }

    livelyRD::DycorsOptimizer::PopulationElement selectParetoPoint(
            const std::vector<livelyRD::DycorsOptimizer::PopulationElement>& solutions)
    {
        return *std::max_element(solutions.begin(), solutions.end(), []
                (const livelyRD::DycorsOptimizer::PopulationElement& a,
                        const livelyRD::DycorsOptimizer::PopulationElement& b)
        {
            return (a.solutionValues.begin()->second) < (a.solutionValues.begin()->second);
        });
    }
    
    
    bool decideTrue(const std::unordered_map<std::string, double>& inputs,
                    std::unordered_map<std::string, std::vector<std::shared_ptr<livelyRD::MessageMultilateralAction>>>&
                    messages)
    {
        return true;
    }

    void sendApprovingMessagePlayer1(std::unordered_map<std::string, std::vector<std::shared_ptr
            <livelyRD::MessageMultilateralAction>>>& messages, const double& amountOfTransaction)
    {
        auto newMessage = std::make_shared<livelyRD::MessageMultilateralAction>();
        newMessage->sender = TRANSACTIONS_TEST_PLAYER1_NAME;
        newMessage->recipients = std::vector<std::string>{TRANSACTIONS_TEST_PLAYER2_NAME};
        newMessage->recipientActionName = TRANSACTIONS_TEST_ACTION_NAME;
        newMessage->messageContent = TRANSACTIONS_TEST_APPROVAL_MESSAGE;
        newMessage->doublesContent[TRANSACTIONS_TEST_PLAYER1_WILLINGNESS_TO_BUY_NAME] = amountOfTransaction;
        messages[TRANSACTIONS_TEST_PLAYER2_NAME].push_back(newMessage);
    }

    void sendRequestingMessagePlayer1(std::unordered_map<std::string, std::vector<std::shared_ptr
            <livelyRD::MessageMultilateralAction>>>& messages, const double& amountOfTransaction)
    {
        auto newMessage = std::make_shared<livelyRD::MessageMultilateralAction>();
        newMessage->sender = TRANSACTIONS_TEST_PLAYER1_NAME;
        newMessage->recipients = std::vector<std::string>{TRANSACTIONS_TEST_PLAYER2_NAME};
        newMessage->recipientActionName = TRANSACTIONS_TEST_ACTION_NAME;
        newMessage->messageContent = TRANSACTIONS_TEST_REQUEST_MESSAGE;
        newMessage->doublesContent[TRANSACTIONS_TEST_PLAYER1_WILLINGNESS_TO_BUY_NAME] = amountOfTransaction;
        messages[TRANSACTIONS_TEST_PLAYER2_NAME].push_back(newMessage);
    }

    void sendRejectingMessagePlayer1(std::unordered_map<std::string, std::vector<std::shared_ptr
            <livelyRD::MessageMultilateralAction>>>& messages)
    {
        auto newMessage = std::make_shared<livelyRD::MessageMultilateralAction>();
        newMessage->sender = TRANSACTIONS_TEST_PLAYER1_NAME;
        newMessage->recipients = std::vector<std::string>{TRANSACTIONS_TEST_PLAYER2_NAME};
        newMessage->recipientActionName = TRANSACTIONS_TEST_ACTION_NAME;
        newMessage->messageContent = TRANSACTIONS_TEST_REJECTION_MESSAGE;
        newMessage->doublesContent[TRANSACTIONS_TEST_PLAYER1_WILLINGNESS_TO_BUY_NAME] = 0.0;
        messages[TRANSACTIONS_TEST_PLAYER2_NAME].push_back(newMessage);
    }

    void handleTransactionRequestPlayer1(std::unordered_map<std::string, std::vector<std::shared_ptr
            <livelyRD::MessageMultilateralAction>>>& messages, const double& amountPlayer1Wants,
            const double& amountPlayer2Wants, std::unordered_map<std::string, double>& outputs)
    {
        if(std::signbit(amountPlayer2Wants) != std::signbit(amountPlayer1Wants))
        {
            double amountOfTransaction = std::min(std::abs(amountPlayer1Wants), std::abs(amountPlayer2Wants));
            if(std::signbit(amountPlayer1Wants))
            {
                amountOfTransaction = - amountOfTransaction;
            }
            outputs[TRANSACTIONS_TEST_RESOURCE1_NAME] += amountOfTransaction;
            outputs[TRANSACTIONS_TEST_RESOURCE2_NAME] -= amountOfTransaction;
            sendApprovingMessagePlayer1(messages, amountOfTransaction);
        }
        else
        {
            sendRejectingMessagePlayer1(messages);
        }
    }

    bool tradePlayer1(const std::unordered_map<std::string, double>& inputs,std::unordered_map
            <std::string, double>& outputs, std::unordered_map<std::string, std::vector<std::shared_ptr
            <livelyRD::MessageMultilateralAction>>>& messages, std::unordered_map<std::string,
            std::unique_ptr<livelyRD::UnilateralAction>>& availableActions)
    {
        for(auto& message : messages[TRANSACTIONS_TEST_PLAYER1_NAME])
        {
            if(message->recipientActionName == TRANSACTIONS_TEST_ACTION_NAME)
            {
                double amountPlayer2Wants = message->doublesContent[TRANSACTIONS_TEST_PLAYER2_WILLINGNESS_TO_BUY_NAME];

                if (message->messageContent == TRANSACTIONS_TEST_APPROVAL_MESSAGE)
                {
                    outputs[TRANSACTIONS_TEST_RESOURCE1_NAME] -= amountPlayer2Wants;
                    outputs[TRANSACTIONS_TEST_RESOURCE2_NAME] += amountPlayer2Wants;
                }
                else if(message->messageContent == TRANSACTIONS_TEST_REQUEST_MESSAGE)
                {
                    double amountPlayer1Wants = inputs.find(TRANSACTIONS_TEST_ACTION_NAME +
                                                            std::to_string(0))->second;

                    if(inputs.find(TRANSACTIONS_TEST_TIME_STEP_NAME)->second <
                    TRANSACTIONS_TEST_NUMBER_OF_TIME_STEPS - 1)
                    {
                        handleTransactionRequestPlayer1(messages, amountPlayer1Wants,
                                amountPlayer2Wants, outputs);
                    }
                }
            }
        }
        if(messages[TRANSACTIONS_TEST_PLAYER1_NAME].empty())
        {
            double amountPlayer1Wants = inputs.find(TRANSACTIONS_TEST_ACTION_NAME +
                                                    std::to_string(0))->second;
            sendRequestingMessagePlayer1(messages, amountPlayer1Wants);
        }
        return true;
    }

    void sendApprovingMessagePlayer2(std::unordered_map<std::string, std::vector<std::shared_ptr
            <livelyRD::MessageMultilateralAction>>>& messages, const double& amountOfTransaction)
    {
        auto newMessage = std::make_shared<livelyRD::MessageMultilateralAction>();
        newMessage->sender = TRANSACTIONS_TEST_PLAYER2_NAME;
        newMessage->recipients = std::vector<std::string>{TRANSACTIONS_TEST_PLAYER1_NAME};
        newMessage->recipientActionName = TRANSACTIONS_TEST_ACTION_NAME;
        newMessage->messageContent = TRANSACTIONS_TEST_APPROVAL_MESSAGE;
        newMessage->doublesContent[TRANSACTIONS_TEST_PLAYER2_WILLINGNESS_TO_BUY_NAME] = amountOfTransaction;
        messages[TRANSACTIONS_TEST_PLAYER1_NAME].push_back(newMessage);
    }

    void sendRequestingMessagePlayer2(std::unordered_map<std::string, std::vector<std::shared_ptr
            <livelyRD::MessageMultilateralAction>>>& messages, const double& amountOfTransaction)
    {
        auto newMessage = std::make_shared<livelyRD::MessageMultilateralAction>();
        newMessage->sender = TRANSACTIONS_TEST_PLAYER2_NAME;
        newMessage->recipients = std::vector<std::string>{TRANSACTIONS_TEST_PLAYER1_NAME};
        newMessage->recipientActionName = TRANSACTIONS_TEST_ACTION_NAME;
        newMessage->messageContent = TRANSACTIONS_TEST_REQUEST_MESSAGE;
        newMessage->doublesContent[TRANSACTIONS_TEST_PLAYER2_WILLINGNESS_TO_BUY_NAME] = amountOfTransaction;
        messages[TRANSACTIONS_TEST_PLAYER1_NAME].push_back(newMessage);
    }

    void sendRejectingMessagePlayer2(std::unordered_map<std::string, std::vector<std::shared_ptr
            <livelyRD::MessageMultilateralAction>>>& messages)
    {
        auto newMessage = std::make_shared<livelyRD::MessageMultilateralAction>();
        newMessage->sender = TRANSACTIONS_TEST_PLAYER1_NAME;
        newMessage->recipients = std::vector<std::string>{TRANSACTIONS_TEST_PLAYER1_NAME};
        newMessage->recipientActionName = TRANSACTIONS_TEST_ACTION_NAME;
        newMessage->messageContent = TRANSACTIONS_TEST_REJECTION_MESSAGE;
        newMessage->doublesContent[TRANSACTIONS_TEST_PLAYER2_WILLINGNESS_TO_BUY_NAME] = 0.0;
        messages[TRANSACTIONS_TEST_PLAYER1_NAME].push_back(newMessage);
    }

    void handleTransactionRequestPlayer2(std::unordered_map<std::string, std::vector<std::shared_ptr
            <livelyRD::MessageMultilateralAction>>>& messages, const double& amountPlayer2Wants,
            const double& amountPlayer1Wants, std::unordered_map<std::string, double>& outputs)
    {
        if(std::signbit(amountPlayer1Wants) != std::signbit(amountPlayer2Wants))
        {
            double amountOfTransaction = std::min(std::abs(amountPlayer2Wants), std::abs(amountPlayer1Wants));
            if(std::signbit(amountPlayer2Wants))
            {
                amountOfTransaction = - amountOfTransaction;
            }
            outputs[TRANSACTIONS_TEST_RESOURCE1_NAME] += amountOfTransaction;
            outputs[TRANSACTIONS_TEST_RESOURCE2_NAME] -= amountOfTransaction;
            sendApprovingMessagePlayer2(messages, amountOfTransaction);
        }
        else
        {
            sendRejectingMessagePlayer2(messages);
        }
    }

    bool tradePlayer2(const std::unordered_map<std::string, double>& inputs,std::unordered_map
            <std::string, double>& outputs, std::unordered_map<std::string, std::vector<std::shared_ptr
            <livelyRD::MessageMultilateralAction>>>& messages, std::unordered_map<std::string,
            std::unique_ptr<livelyRD::UnilateralAction>>& availableActions)
    {
        for(auto& message : messages[TRANSACTIONS_TEST_PLAYER2_NAME])
        {
            if(message->recipientActionName == TRANSACTIONS_TEST_ACTION_NAME)
            {
                double amountPlayer1Wants = message->doublesContent[TRANSACTIONS_TEST_PLAYER1_WILLINGNESS_TO_BUY_NAME];

                if (message->messageContent == TRANSACTIONS_TEST_APPROVAL_MESSAGE)
                {
                    outputs[TRANSACTIONS_TEST_RESOURCE1_NAME] -= amountPlayer1Wants;
                    outputs[TRANSACTIONS_TEST_RESOURCE2_NAME] += amountPlayer1Wants;
                }
                else if(message->messageContent == TRANSACTIONS_TEST_REQUEST_MESSAGE)
                {
                    double amountPlayer2Wants = inputs.find(TRANSACTIONS_TEST_ACTION_NAME +
                                                            std::to_string(0))->second;

                    if(inputs.find(TRANSACTIONS_TEST_TIME_STEP_NAME)->second <
                       TRANSACTIONS_TEST_NUMBER_OF_TIME_STEPS - 1)
                    {
                        handleTransactionRequestPlayer2(messages, amountPlayer2Wants,
                                amountPlayer1Wants, outputs);
                    }
                }
            }
        }
        if(messages[TRANSACTIONS_TEST_PLAYER2_NAME].empty())
        {
            double amountPlayer2Wants = inputs.find(TRANSACTIONS_TEST_ACTION_NAME +
                                                    std::to_string(0))->second;
            sendRequestingMessagePlayer2(messages, amountPlayer2Wants);
        }
        return true;
    }
}




SCENARIO("Trading spoons for forks", "")
{
    GIVEN("An Environment with two players trading resources with each other")
    {
        std::string environmentName = TRANSACTIONS_TEST_ENVIRONMNET_NAME;
        std::string player1Name = TRANSACTIONS_TEST_PLAYER1_NAME;
        std::string player2Name = TRANSACTIONS_TEST_PLAYER2_NAME;
        auto environment = std::make_shared<livelyRD::Environment>(environmentName);
        auto player1 = std::make_unique<livelyRD::Player>(player1Name, environmentName);
        auto player2 = std::make_unique<livelyRD::Player>(player2Name, environmentName);

        std::string spoonsName = TRANSACTIONS_TEST_RESOURCE2_NAME;
        double initialSpoonsPlayer1 = TRANSACTIONS_TEST_PLAYER1_RESOURCE2_INITIAL_QUANTITY;
        double initialSpoonsPlayer2 = TRANSACTIONS_TEST_PLAYER2_RESOURCE2_INITIAL_QUANTITY;
        std::string forksName = TRANSACTIONS_TEST_RESOURCE1_NAME;
        double initialForksPlayer1 = TRANSACTIONS_TEST_PLAYER1_RESOURCE1_INITIAL_QUANTITY;
        double initialForksPlayer2 = TRANSACTIONS_TEST_PLAYER2_RESOURCE1_INITIAL_QUANTITY;
        auto spoonsPlayer1 = std::make_unique<livelyRD::Resource>(spoonsName, player1Name, initialSpoonsPlayer1);
        auto spoonsPlayer2 = std::make_unique<livelyRD::Resource>(spoonsName, player2Name, initialSpoonsPlayer2);
        auto forksPlayer1 = std::make_unique<livelyRD::Resource>(forksName, player1Name, initialForksPlayer1);
        auto forksPlayer2 = std::make_unique<livelyRD::Resource>(forksName, player2Name, initialForksPlayer2);

        std::string tradeActionName = TRANSACTIONS_TEST_ACTION_NAME;
        double getForksLowerBound = TRANSACTIONS_TEST_ACTION_LOWER_BOUND;
        double getForksUpperBound = TRANSACTIONS_TEST_ACTION_UPPER_BOUND;

        std::unique_ptr<livelyRD::AbstractAction> tradePlayer1 =
                std::make_unique<livelyRD::MultilateralAction>(tradeActionName, player1Name,
                TestTransactions::tradePlayer1, TestTransactions::decideTrue, false, getForksLowerBound,
                getForksUpperBound);
        tradePlayer1->setCurrentEnvironment(environment);
        std::unique_ptr<livelyRD::AbstractAction> tradePlayer2 =
                std::make_unique<livelyRD::MultilateralAction>(tradeActionName, player2Name,
                TestTransactions::tradePlayer2, TestTransactions::decideTrue, false, getForksLowerBound,
                getForksUpperBound);
        tradePlayer2->setCurrentEnvironment(environment);

        std::string objectiveName = TRANSACTIONS_TEST_OBJECTIVE_NAME;
        auto objectivePlayer1 = std::make_unique<livelyRD::Objective>(objectiveName, player1Name,
                TestTransactions::objectiveSpoonsForksPlayer1, 0.0, true);
        auto objectivePlayer2 = std::make_unique<livelyRD::Objective>(objectiveName, player2Name,
                TestTransactions::objectiveSpoonsForksPlayer2, 0.0, true);

        player1->addResource(spoonsPlayer1);
        player1->addResource(forksPlayer1);
        player1->setResourcesListPublic(environmentName);
        player1->addAction(tradePlayer1);
        player1->addObjective(objectivePlayer1);
        player1->addPreferences(TestTransactions::selectParetoPoint);
        player1->setTimeStepName(TRANSACTIONS_TEST_TIME_STEP_NAME);

        player2->addResource(spoonsPlayer2);
        player2->addResource(forksPlayer2);
        player2->setResourcesListPublic(environmentName);
        player2->addAction(tradePlayer2);
        player2->addObjective(objectivePlayer2);
        player2->addPreferences(TestTransactions::selectParetoPoint);
        player2->setTimeStepName(TRANSACTIONS_TEST_TIME_STEP_NAME);

        environment->setNumberOfTimeSteps(TRANSACTIONS_TEST_NUMBER_OF_TIME_STEPS);
        environment->addPlayer(TRANSACTIONS_TEST_PLAYER1_NAME, player1);
        environment->addPlayer(TRANSACTIONS_TEST_PLAYER2_NAME, player2);

        WHEN("The environment is solved")
        {
            REQUIRE(environment->solve());

            THEN("The resources of each player should be the following")
            {
                const std::unordered_map<std::string, std::unique_ptr<livelyRD::Player>>& players =
                        environment->getAllPlayers();

                REQUIRE(players.find(player1Name)->second->getResource(spoonsName)->getQuantity(player1Name) > 9.0);
                REQUIRE(players.find(player1Name)->second->getResource(spoonsName)->getQuantity(player1Name) < 11.0);
                REQUIRE(players.find(player1Name)->second->getResource(forksName)->getQuantity(player1Name) > 89.0);
                REQUIRE(players.find(player1Name)->second->getResource(forksName)->getQuantity(player1Name) < 91.0);

                REQUIRE(players.find(player2Name)->second->getResource(spoonsName)->getQuantity(player2Name) > 89.0);
                REQUIRE(players.find(player2Name)->second->getResource(spoonsName)->getQuantity(player2Name) < 91.0);
                REQUIRE(players.find(player2Name)->second->getResource(forksName)->getQuantity(player2Name) > 9.0);
                REQUIRE(players.find(player2Name)->second->getResource(forksName)->getQuantity(player2Name) < 11.0);
            }
        }
    }
}