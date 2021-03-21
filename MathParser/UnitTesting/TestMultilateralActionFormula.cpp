#include <catch.hpp>
#include "MultilateralActionFormula.h"
#include "DefinitionsMultilateralActionCalculator.h"

SCENARIO("Calculate RECEIVER_UNIDIRECTIONAL_COMPULSORY action from a string")
{
    std::string player1Name = "player1";
    std::string resourceName = "resource1";
    std::string actionName = "action";
    std::unordered_map<std::string, std::string> action;
    action[resourceName] = "resource1 + action_value";
    std::vector<std::string> recipients;
    recipients.emplace_back("me");
    std::string recipientActionName = "Recipient action";
    livelyRD::MultilateralActionFormula::MultilateralActionType type =
            livelyRD::MultilateralActionFormula::RECEIVER_UNIDIRECTIONAL_COMPULSORY;

    WHEN("The equation is converted to an MultilateralActionFormula") {
        livelyRD::MultilateralActionFormula actionFormula(action, type, recipients, player1Name, actionName,
                                                          recipientActionName);

        THEN("It should be possible to get the equation as a function")
        {
            auto actionAsFunction = actionFormula.getEquationsAsFunction();

            AND_GIVEN("A set of inputs, outputs, and messages")
            {
                int numberOfTimeSteps = 5;
                std::unordered_map<std::string, double> inputs;
                inputs[resourceName] = 1.0;
                for (int i = 0; i < numberOfTimeSteps; ++i)
                {
                    inputs[actionName + std::to_string(i)] = 1.0;
                }
                std::unordered_map<std::string, double> outputs;
                std::unordered_map<std::string, std::vector
                        <std::shared_ptr<livelyRD::MessageMultilateralAction>>> messages;
                auto message = std::make_shared<livelyRD::MessageMultilateralAction>();
                message->sender = recipients.back();
                message->recipients.push_back(player1Name);
                message->recipientActionName = actionName;
                message->doublesContent[recipients.back()] = 2.0;
                message->senderActionName = recipientActionName;
                message->messageContent = MULTILATERAL_REQUEST_MESSAGE;
                messages[player1Name].push_back(message);
                std::unordered_map<std::string,
                        std::unique_ptr<livelyRD::UnilateralAction>> unilateralActions;

                WHEN("The function gets evaluated")
                {
                    REQUIRE(actionAsFunction(inputs, outputs, messages, unilateralActions));

                    THEN("The output values should have changed")
                    {
                        REQUIRE(outputs[resourceName] == 3.0);
                    }
                }
            }
        }
        AND_GIVEN("A set of inputs and a number of time-steps")
        {
            int numberOfTimeSteps = 5;
            std::unordered_map<std::string, double> inputs, currentValues, results;
            inputs[resourceName] = 0.0;
            currentValues[resourceName] = 0.0;
            for(int i = 0; i<numberOfTimeSteps; ++i)
            {
                inputs[actionName + std::to_string(i)] = 1.0;
            }

            THEN("It should be possible to evaluate the function")
            {
                results = actionFormula.calculateExpectedTotalValue(currentValues, inputs, numberOfTimeSteps);
                REQUIRE(results[resourceName] == 5.0);
            }
        }
    }
}


SCENARIO("Calculate SENDER_UNIDIRECTIONAL_COMPULSORY action from a string")
{
    std::string player1Name = "player1";
    std::string resourceName = "resource1";
    std::string actionName = "action";
    std::unordered_map<std::string, std::string> action;
    action[resourceName] = "resource1 + action_value";
    std::vector<std::string> recipients;
    recipients.emplace_back("me");
    std::string recipientActionName = "Recipient action";
    livelyRD::MultilateralActionFormula::MultilateralActionType type =
            livelyRD::MultilateralActionFormula::SENDER_UNIDIRECTIONAL_COMPULSORY;

    WHEN("The equation is converted to an MultilateralActionFormula") {
        livelyRD::MultilateralActionFormula actionFormula(action, type, recipients, player1Name, actionName,
                                                          recipientActionName);

        THEN("It should be possible to get the equation as a function")
        {
            auto actionAsFunction = actionFormula.getEquationsAsFunction();

            AND_GIVEN("A set of inputs, outputs, and messages")
            {
                int numberOfTimeSteps = 5;
                std::unordered_map<std::string, double> inputs;
                inputs[resourceName] = 1.0;
                for (int i = 0; i < numberOfTimeSteps; ++i)
                {
                    inputs[actionName + std::to_string(i)] = 1.0;
                }
                std::unordered_map<std::string, double> outputs;
                std::unordered_map<std::string, std::vector
                        <std::shared_ptr<livelyRD::MessageMultilateralAction>>> messages;
                auto message = std::make_shared<livelyRD::MessageMultilateralAction>();
                message->sender = recipients.back();
                message->recipients.push_back(player1Name);
                message->recipientActionName = actionName;
                message->doublesContent[recipients.back()] = 2.0;
                message->senderActionName = recipientActionName;
                message->messageContent = MULTILATERAL_APPROVAL_MESSAGE;
                messages[player1Name].push_back(message);
                std::unordered_map<std::string,
                        std::unique_ptr<livelyRD::UnilateralAction>> unilateralActions;

                WHEN("The function gets evaluated")
                {
                    REQUIRE(actionAsFunction(inputs, outputs, messages, unilateralActions));

                    THEN("The output values should have changed")
                    {
                        REQUIRE(outputs[resourceName] == 3.0);
                        REQUIRE(!messages.find(recipients.back())->second.empty());
                        REQUIRE(messages[recipients.back()].back()->messageContent == MULTILATERAL_REQUEST_MESSAGE);
                        REQUIRE(messages[recipients.back()].back()->doublesContent[player1Name] == 1.0);
                    }
                }
            }
        }
        AND_GIVEN("A set of inputs and a number of time-steps")
        {
            int numberOfTimeSteps = 5;
            std::unordered_map<std::string, double> inputs, currentValues, results;
            inputs[resourceName] = 0.0;
            currentValues[resourceName] = 0.0;
            for(int i = 0; i<numberOfTimeSteps; ++i)
            {
                inputs[actionName + std::to_string(i)] = 1.0;
            }

            THEN("It should be possible to evaluate the function")
            {
                results = actionFormula.calculateExpectedTotalValue(currentValues, inputs, numberOfTimeSteps);
                REQUIRE(results[resourceName] == 5.0);
            }
        }
    }
}


SCENARIO("Calculate BIDIRECTIONAL_COMPULSORY action from a string")
{
    std::string player1Name = "player1";
    std::string resourceName = "resource1";
    std::string actionName = "action";
    std::unordered_map<std::string, std::string> action;
    action[resourceName] = "resource1 + action_value";
    std::vector<std::string> recipients;
    recipients.emplace_back("me");
    std::string recipientActionName = "Recipient action";
    livelyRD::MultilateralActionFormula::MultilateralActionType type =
            livelyRD::MultilateralActionFormula::BIDIRECTIONAL_COMPULSORY;

    WHEN("The equation is converted to an MultilateralActionFormula") {
        livelyRD::MultilateralActionFormula actionFormula(action, type, recipients, player1Name, actionName,
                                                          recipientActionName);

        THEN("It should be possible to get the equation as a function")
        {
            auto actionAsFunction = actionFormula.getEquationsAsFunction();

            AND_GIVEN("A set of inputs, outputs, and an approval message")
            {
                int numberOfTimeSteps = 5;
                std::unordered_map<std::string, double> inputs;
                inputs[resourceName] = 1.0;
                for (int i = 0; i < numberOfTimeSteps; ++i)
                {
                    inputs[actionName + std::to_string(i)] = 1.0;
                }
                std::unordered_map<std::string, double> outputs;
                std::unordered_map<std::string, std::vector
                        <std::shared_ptr<livelyRD::MessageMultilateralAction>>> messages;
                auto message = std::make_shared<livelyRD::MessageMultilateralAction>();
                message->sender = recipients.back();
                message->recipients.push_back(player1Name);
                message->recipientActionName = actionName;
                message->doublesContent[recipients.back()] = 2.0;
                message->senderActionName = recipientActionName;
                message->messageContent = MULTILATERAL_APPROVAL_MESSAGE;
                messages[player1Name].push_back(message);
                std::unordered_map<std::string,
                        std::unique_ptr<livelyRD::UnilateralAction>> unilateralActions;

                WHEN("The function gets evaluated")
                {
                    REQUIRE(actionAsFunction(inputs, outputs, messages, unilateralActions));

                    THEN("The output values should have changed")
                    {
                        REQUIRE(outputs[resourceName] == 3.0);
                        REQUIRE(!messages.find(recipients.back())->second.empty());
                        REQUIRE(messages[recipients.back()].back()->messageContent == MULTILATERAL_REQUEST_MESSAGE);
                        REQUIRE(messages[recipients.back()].back()->doublesContent[player1Name] == 1.0);
                    }
                }
            }

            AND_GIVEN("A set of inputs, outputs, and a request message")
            {
                int numberOfTimeSteps = 5;
                std::unordered_map<std::string, double> inputs;
                inputs[resourceName] = 1.0;
                for (int i = 0; i < numberOfTimeSteps; ++i)
                {
                    inputs[actionName + std::to_string(i)] = 1.0;
                }
                std::unordered_map<std::string, double> outputs;
                std::unordered_map<std::string, std::vector
                        <std::shared_ptr<livelyRD::MessageMultilateralAction>>> messages;
                auto message = std::make_shared<livelyRD::MessageMultilateralAction>();
                message->sender = recipients.back();
                message->recipients.push_back(player1Name);
                message->recipientActionName = actionName;
                message->doublesContent[recipients.back()] = 2.0;
                message->senderActionName = recipientActionName;
                message->messageContent = MULTILATERAL_REQUEST_MESSAGE;
                messages[player1Name].push_back(message);
                std::unordered_map<std::string,
                        std::unique_ptr<livelyRD::UnilateralAction>> unilateralActions;

                WHEN("The function gets evaluated")
                {
                    REQUIRE(actionAsFunction(inputs, outputs, messages, unilateralActions));

                    THEN("The output values should have changed")
                    {
                        REQUIRE(outputs[resourceName] == 3.0);
                        REQUIRE(!messages.find(recipients.back())->second.empty());
                        REQUIRE(messages[recipients.back()].back()->messageContent == MULTILATERAL_APPROVAL_MESSAGE);
                        REQUIRE(messages[recipients.back()].back()->doublesContent[player1Name] == 2.0);
                    }
                }
            }
        }
        AND_GIVEN("A set of inputs and a number of time-steps")
        {
            int numberOfTimeSteps = 5;
            std::unordered_map<std::string, double> inputs, currentValues, results;
            inputs[resourceName] = 0.0;
            currentValues[resourceName] = 0.0;
            for(int i = 0; i<numberOfTimeSteps; ++i)
            {
                inputs[actionName + std::to_string(i)] = 1.0;
            }

            THEN("It should be possible to evaluate the function")
            {
                results = actionFormula.calculateExpectedTotalValue(currentValues, inputs, numberOfTimeSteps);
                REQUIRE(results[resourceName] == 5.0);
            }
        }
    }
}


SCENARIO("Calculate NON_NEGOTIABLE action from a string")
{
    std::string player1Name = "player1";
    std::string resourceName = "resource1";
    std::string actionName = "action";
    std::unordered_map<std::string, std::string> action;
    action[resourceName] = "resource1 + action_value";
    std::vector<std::string> recipients;
    recipients.emplace_back("me");
    recipients.emplace_back("you");
    recipients.emplace_back("him");
    std::string recipientActionName = "Recipient action";
    livelyRD::MultilateralActionFormula::MultilateralActionType type =
            livelyRD::MultilateralActionFormula::NON_NEGOTIABLE;

    WHEN("The equation is converted to an MultilateralActionFormula") {
        livelyRD::MultilateralActionFormula actionFormula(action, type, recipients, player1Name, actionName,
                                                          recipientActionName);

        THEN("It should be possible to get the equation as a function")
        {
            auto actionAsFunction = actionFormula.getEquationsAsFunction();

            AND_GIVEN("A set of inputs, outputs, and an approval message")
            {
                int numberOfTimeSteps = 5;
                std::unordered_map<std::string, double> inputs;
                inputs[resourceName] = 1.0;
                for (int i = 0; i < numberOfTimeSteps; ++i)
                {
                    inputs[actionName + std::to_string(i)] = 1.0;
                }
                std::unordered_map<std::string, double> outputs;
                std::unordered_map<std::string, std::vector
                        <std::shared_ptr<livelyRD::MessageMultilateralAction>>> messages;
                auto message = std::make_shared<livelyRD::MessageMultilateralAction>();
                message->sender = recipients[0];
                message->recipients.push_back(player1Name);
                message->recipientActionName = actionName;
                message->doublesContent[recipients[0]] = 2.0;
                message->senderActionName = recipientActionName;
                message->messageContent = MULTILATERAL_APPROVAL_MESSAGE;
                messages[player1Name].push_back(message);
                std::unordered_map<std::string,
                        std::unique_ptr<livelyRD::UnilateralAction>> unilateralActions;

                WHEN("The function gets evaluated")
                {
                    REQUIRE(actionAsFunction(inputs, outputs, messages, unilateralActions));

                    THEN("The output values should have changed")
                    {
                        REQUIRE(outputs[resourceName] == 3.0);
                        REQUIRE(!messages.find(recipients.back())->second.empty());
                        REQUIRE(messages[recipients[0]].back()->messageContent == MULTILATERAL_REQUEST_MESSAGE);
                        REQUIRE(messages[recipients[0]].back()->doublesContent[player1Name] == 1.0);
                    }
                }
            }

            AND_GIVEN("A set of inputs, outputs, and a request message")
            {
                int numberOfTimeSteps = 5;
                std::unordered_map<std::string, double> inputs;
                inputs[resourceName] = 1.0;
                for (int i = 0; i < numberOfTimeSteps; ++i)
                {
                    inputs[actionName + std::to_string(i)] = 1.0;
                }
                std::unordered_map<std::string, double> outputs;
                std::unordered_map<std::string, std::vector
                        <std::shared_ptr<livelyRD::MessageMultilateralAction>>> messages;
                auto message = std::make_shared<livelyRD::MessageMultilateralAction>();
                message->sender = recipients[0];
                message->recipients.push_back(player1Name);
                message->recipientActionName = actionName;
                message->doublesContent[recipients[0]] = 2.0;
                message->senderActionName = recipientActionName;
                message->messageContent = MULTILATERAL_REQUEST_MESSAGE;
                messages[player1Name].push_back(message);
                std::unordered_map<std::string,
                        std::unique_ptr<livelyRD::UnilateralAction>> unilateralActions;

                WHEN("The function gets evaluated")
                {
                    REQUIRE(actionAsFunction(inputs, outputs, messages, unilateralActions));

                    THEN("The output values should have changed")
                    {
                        REQUIRE(outputs[resourceName] == 3.0);
                        REQUIRE(!messages.find(recipients[0])->second.empty());
                        REQUIRE(messages[recipients[0]].back()->messageContent == MULTILATERAL_APPROVAL_MESSAGE);
                        REQUIRE(messages[recipients[0]].back()->doublesContent[player1Name] == 2.0);
                    }
                }
            }

            AND_GIVEN("A set of inputs, outputs, and multiple request message")
            {
                int numberOfTimeSteps = 5;
                std::unordered_map<std::string, double> inputs;
                inputs[resourceName] = 1.0;
                for (int i = 0; i < numberOfTimeSteps; ++i)
                {
                    inputs[actionName + std::to_string(i)] = 1.0;
                }
                std::unordered_map<std::string, double> outputs;
                std::unordered_map<std::string, std::vector
                        <std::shared_ptr<livelyRD::MessageMultilateralAction>>> messages;
                auto message1 = std::make_shared<livelyRD::MessageMultilateralAction>();
                message1->sender = recipients[0];
                message1->recipients.push_back(player1Name);
                message1->recipientActionName = actionName;
                message1->doublesContent[recipients[0]] = 2.0;
                message1->senderActionName = recipientActionName;
                message1->messageContent = MULTILATERAL_REQUEST_MESSAGE;

                messages[player1Name].push_back(message1);

                auto message2 = std::make_shared<livelyRD::MessageMultilateralAction>();
                message2->sender = recipients[1];
                message2->recipients.push_back(player1Name);
                message2->recipientActionName = actionName;
                message2->doublesContent[recipients[1]] = 1.5;
                message2->senderActionName = recipientActionName;
                message2->messageContent = MULTILATERAL_REQUEST_MESSAGE;
                messages[player1Name].push_back(message2);

                auto message3 = std::make_shared<livelyRD::MessageMultilateralAction>();
                message3->sender = recipients[2];
                message3->recipients.push_back(player1Name);
                message3->recipientActionName = actionName;
                message3->doublesContent[recipients[2]] = -1.0;
                message3->senderActionName = recipientActionName;
                message3->messageContent = MULTILATERAL_REQUEST_MESSAGE;
                messages[player1Name].push_back(message3);

                std::unordered_map<std::string,
                        std::unique_ptr<livelyRD::UnilateralAction>> unilateralActions;

                WHEN("The function gets evaluated")
                {
                    REQUIRE(actionAsFunction(inputs, outputs, messages, unilateralActions));

                    THEN("The output values should have changed")
                    {
                        REQUIRE(outputs[resourceName] == 2.5);

                        REQUIRE(!messages.find(recipients[0])->second.empty());
                        REQUIRE(messages[recipients[0]].back()->messageContent == MULTILATERAL_REJECTION_MESSAGE);
                        REQUIRE(messages[recipients[0]].back()->doublesContent[player1Name] == 0.0);

                        REQUIRE(!messages.find(recipients[1])->second.empty());
                        REQUIRE(messages[recipients[1]].back()->messageContent == MULTILATERAL_APPROVAL_MESSAGE);
                        REQUIRE(messages[recipients[1]].back()->doublesContent[player1Name] == 1.5);

                        REQUIRE(!messages.find(recipients[2])->second.empty());
                        REQUIRE(messages[recipients[2]].back()->messageContent == MULTILATERAL_REJECTION_MESSAGE);
                        REQUIRE(messages[recipients[2]].back()->doublesContent[player1Name] == 0.0);
                    }
                }
            }
        }
        AND_GIVEN("A set of inputs and a number of time-steps")
        {
            int numberOfTimeSteps = 5;
            std::unordered_map<std::string, double> inputs, currentValues, results;
            inputs[resourceName] = 0.0;
            currentValues[resourceName] = 0.0;
            for(int i = 0; i<numberOfTimeSteps; ++i)
            {
                inputs[actionName + std::to_string(i)] = 1.0;
            }

            THEN("It should be possible to evaluate the function")
            {
                results = actionFormula.calculateExpectedTotalValue(currentValues, inputs, numberOfTimeSteps);
                REQUIRE(results[resourceName] == 5.0);
            }
        }
    }
}


SCENARIO("Calculate NEGOTIABLE action from a string")
{
    std::string player1Name = "player1";
    std::string resourceName = "resource1";
    std::string actionName = "action";
    std::unordered_map<std::string, std::string> action;
    action[resourceName] = "resource1 + action_value";
    std::vector<std::string> recipients;
    recipients.emplace_back("me");
    recipients.emplace_back("you");
    recipients.emplace_back("him");
    std::string recipientActionName = "Recipient action";
    livelyRD::MultilateralActionFormula::MultilateralActionType type =
            livelyRD::MultilateralActionFormula::NEGOTIABLE;

    WHEN("The equation is converted to an MultilateralActionFormula") {
        livelyRD::MultilateralActionFormula actionFormula(action, type, recipients, player1Name, actionName,
                                                          recipientActionName);

        THEN("It should be possible to get the equation as a function")
        {
            auto actionAsFunction = actionFormula.getEquationsAsFunction();

            AND_GIVEN("A set of inputs, outputs, and an approval message")
            {
                int numberOfTimeSteps = 5;
                std::unordered_map<std::string, double> inputs;
                inputs[resourceName] = 1.0;
                for (int i = 0; i < numberOfTimeSteps; ++i)
                {
                    inputs[actionName + std::to_string(i)] = 1.0;
                }
                std::unordered_map<std::string, double> outputs;
                std::unordered_map<std::string, std::vector
                        <std::shared_ptr<livelyRD::MessageMultilateralAction>>> messages;
                auto message = std::make_shared<livelyRD::MessageMultilateralAction>();
                message->sender = recipients[0];
                message->recipients.push_back(player1Name);
                message->recipientActionName = actionName;
                message->doublesContent[recipients[0]] = 2.0;
                message->senderActionName = recipientActionName;
                message->messageContent = MULTILATERAL_APPROVAL_MESSAGE;
                messages[player1Name].push_back(message);
                std::unordered_map<std::string,
                        std::unique_ptr<livelyRD::UnilateralAction>> unilateralActions;

                WHEN("The function gets evaluated")
                {
                    REQUIRE(actionAsFunction(inputs, outputs, messages, unilateralActions));

                    THEN("The output values should have changed")
                    {
                        REQUIRE(outputs[resourceName] == 3.0);
                        REQUIRE(!messages.find(recipients.back())->second.empty());
                        REQUIRE(messages[recipients[0]].back()->messageContent == MULTILATERAL_REQUEST_MESSAGE);
                        REQUIRE(messages[recipients[0]].back()->doublesContent[player1Name] == 1.0);
                    }
                }
            }

            AND_GIVEN("A set of inputs, outputs, and a request message")
            {
                int numberOfTimeSteps = 5;
                std::unordered_map<std::string, double> inputs;
                inputs[resourceName] = 1.0;
                for (int i = 0; i < numberOfTimeSteps; ++i)
                {
                    inputs[actionName + std::to_string(i)] = 1.0;
                }
                std::unordered_map<std::string, double> outputs;
                std::unordered_map<std::string, std::vector
                        <std::shared_ptr<livelyRD::MessageMultilateralAction>>> messages;
                auto message = std::make_shared<livelyRD::MessageMultilateralAction>();
                message->sender = recipients[0];
                message->recipients.push_back(player1Name);
                message->recipientActionName = actionName;
                message->doublesContent[recipients[0]] = 2.0;
                message->senderActionName = recipientActionName;
                message->messageContent = MULTILATERAL_REQUEST_MESSAGE;
                messages[player1Name].push_back(message);
                std::unordered_map<std::string,
                        std::unique_ptr<livelyRD::UnilateralAction>> unilateralActions;

                WHEN("The function gets evaluated")
                {
                    REQUIRE(actionAsFunction(inputs, outputs, messages, unilateralActions));

                    THEN("The output values should have changed")
                    {
                        REQUIRE(outputs[resourceName] == 1.0);
                        REQUIRE(!messages.find(recipients[0])->second.empty());
                        REQUIRE(messages[recipients[0]].back()->messageContent == MULTILATERAL_AMEND_MESSAGE);
                        REQUIRE(messages[recipients[0]].back()->doublesContent[player1Name] == 1.0);
                    }
                }
            }

            AND_GIVEN("A set of inputs, outputs, and multiple request messages")
            {
                int numberOfTimeSteps = 5;
                std::unordered_map<std::string, double> inputs;
                inputs[resourceName] = 1.0;
                for (int i = 0; i < numberOfTimeSteps; ++i)
                {
                    inputs[actionName + std::to_string(i)] = 1.0;
                }
                std::unordered_map<std::string, double> outputs;
                std::unordered_map<std::string, std::vector
                        <std::shared_ptr<livelyRD::MessageMultilateralAction>>> messages;
                auto message1 = std::make_shared<livelyRD::MessageMultilateralAction>();
                message1->sender = recipients[0];
                message1->recipients.push_back(player1Name);
                message1->recipientActionName = actionName;
                message1->doublesContent[recipients[0]] = 2.0;
                message1->senderActionName = recipientActionName;
                message1->messageContent = MULTILATERAL_REQUEST_MESSAGE;

                messages[player1Name].push_back(message1);

                auto message2 = std::make_shared<livelyRD::MessageMultilateralAction>();
                message2->sender = recipients[1];
                message2->recipients.push_back(player1Name);
                message2->recipientActionName = actionName;
                message2->doublesContent[recipients[1]] = 1.5;
                message2->senderActionName = recipientActionName;
                message2->messageContent = MULTILATERAL_REQUEST_MESSAGE;
                messages[player1Name].push_back(message2);

                auto message3 = std::make_shared<livelyRD::MessageMultilateralAction>();
                message3->sender = recipients[2];
                message3->recipients.push_back(player1Name);
                message3->recipientActionName = actionName;
                message3->doublesContent[recipients[2]] = -1.0;
                message3->senderActionName = recipientActionName;
                message3->messageContent = MULTILATERAL_REQUEST_MESSAGE;
                messages[player1Name].push_back(message3);

                std::unordered_map<std::string,
                        std::unique_ptr<livelyRD::UnilateralAction>> unilateralActions;

                WHEN("The function gets evaluated")
                {
                    REQUIRE(actionAsFunction(inputs, outputs, messages, unilateralActions));

                    THEN("The output values should have changed")
                    {
                        REQUIRE(outputs[resourceName] == 1.0);

                        REQUIRE(!messages.find(recipients[0])->second.empty());
                        REQUIRE(messages[recipients[0]].back()->messageContent == MULTILATERAL_AMEND_MESSAGE);
                        REQUIRE(messages[recipients[0]].back()->doublesContent[player1Name] == 1.0);

                        REQUIRE(!messages.find(recipients[1])->second.empty());
                        REQUIRE(messages[recipients[1]].back()->messageContent == MULTILATERAL_AMEND_MESSAGE);
                        REQUIRE(messages[recipients[1]].back()->doublesContent[player1Name] == 1.0);

                        REQUIRE(!messages.find(recipients[2])->second.empty());
                        REQUIRE(messages[recipients[2]].back()->messageContent == MULTILATERAL_AMEND_MESSAGE);
                        REQUIRE(messages[recipients[2]].back()->doublesContent[player1Name] == 1.0);
                    }
                }
            }

            AND_GIVEN("A set of inputs, outputs, and multiple amend messages [answer: more amendments]")
            {
                int numberOfTimeSteps = 5;
                std::unordered_map<std::string, double> inputs;
                inputs[resourceName] = 1.0;
                for (int i = 0; i < numberOfTimeSteps; ++i)
                {
                    inputs[actionName + std::to_string(i)] = 1.0;
                }
                std::unordered_map<std::string, double> outputs;
                std::unordered_map<std::string, std::vector
                        <std::shared_ptr<livelyRD::MessageMultilateralAction>>> messages;
                auto message1 = std::make_shared<livelyRD::MessageMultilateralAction>();
                message1->sender = recipients[0];
                message1->recipients.push_back(player1Name);
                message1->recipientActionName = actionName;
                message1->doublesContent[recipients[0]] = 2.0;
                message1->doublesContent[player1Name + MULTILATERAL_AMEND_PREVIOUS_VALUE] = 1.0;
                message1->senderActionName = recipientActionName;
                message1->messageContent = MULTILATERAL_AMEND_MESSAGE;

                messages[player1Name].push_back(message1);

                auto message2 = std::make_shared<livelyRD::MessageMultilateralAction>();
                message2->sender = recipients[1];
                message2->recipients.push_back(player1Name);
                message2->recipientActionName = actionName;
                message2->doublesContent[recipients[1]] = 1.5;
                message2->doublesContent[player1Name + MULTILATERAL_AMEND_PREVIOUS_VALUE] = 1.0;
                message2->senderActionName = recipientActionName;
                message2->messageContent = MULTILATERAL_AMEND_MESSAGE;
                messages[player1Name].push_back(message2);

                auto message3 = std::make_shared<livelyRD::MessageMultilateralAction>();
                message3->sender = recipients[2];
                message3->recipients.push_back(player1Name);
                message3->recipientActionName = actionName;
                message3->doublesContent[recipients[2]] = -1.0;
                message3->doublesContent[player1Name + MULTILATERAL_AMEND_PREVIOUS_VALUE] = 1.0;
                message3->senderActionName = recipientActionName;
                message3->messageContent = MULTILATERAL_AMEND_MESSAGE;
                messages[player1Name].push_back(message3);

                std::unordered_map<std::string,
                        std::unique_ptr<livelyRD::UnilateralAction>> unilateralActions;

                WHEN("The function gets evaluated")
                {
                    REQUIRE(actionAsFunction(inputs, outputs, messages, unilateralActions));

                    THEN("The output values should have changed")
                    {
                        REQUIRE(outputs[resourceName] == 1.0);

                        REQUIRE(!messages.find(recipients[0])->second.empty());
                        REQUIRE(messages[recipients[0]].back()->messageContent == MULTILATERAL_AMEND_MESSAGE);
                        REQUIRE(messages[recipients[0]].back()->doublesContent[player1Name] == 1.1);

                        REQUIRE(!messages.find(recipients[1])->second.empty());
                        REQUIRE(messages[recipients[1]].back()->messageContent == MULTILATERAL_AMEND_MESSAGE);
                        REQUIRE(messages[recipients[1]].back()->doublesContent[player1Name] == 1.1);

                        REQUIRE(!messages.find(recipients[2])->second.empty());
                        REQUIRE(messages[recipients[2]].back()->messageContent == MULTILATERAL_AMEND_MESSAGE);
                        REQUIRE(messages[recipients[2]].back()->doublesContent[player1Name] == 0.9);
                    }
                }
            }

            AND_GIVEN("A set of inputs, outputs, and multiple amend messages [answer: one approval]")
            {
                int numberOfTimeSteps = 5;
                std::unordered_map<std::string, double> inputs;
                inputs[resourceName] = 1.0;
                for (int i = 0; i < numberOfTimeSteps; ++i)
                {
                    inputs[actionName + std::to_string(i)] = 1.0;
                }
                std::unordered_map<std::string, double> outputs;
                std::unordered_map<std::string, std::vector
                        <std::shared_ptr<livelyRD::MessageMultilateralAction>>> messages;
                auto message1 = std::make_shared<livelyRD::MessageMultilateralAction>();
                message1->sender = recipients[0];
                message1->recipients.push_back(player1Name);
                message1->recipientActionName = actionName;
                message1->doublesContent[recipients[0]] = 2.0;
                message1->doublesContent[player1Name + MULTILATERAL_AMEND_PREVIOUS_VALUE] = 1.45;
                message1->senderActionName = recipientActionName;
                message1->messageContent = MULTILATERAL_AMEND_MESSAGE;

                messages[player1Name].push_back(message1);

                auto message2 = std::make_shared<livelyRD::MessageMultilateralAction>();
                message2->sender = recipients[1];
                message2->recipients.push_back(player1Name);
                message2->recipientActionName = actionName;
                message2->doublesContent[recipients[1]] = 1.5;
                message2->doublesContent[player1Name + MULTILATERAL_AMEND_PREVIOUS_VALUE] = 1.45;
                message2->senderActionName = recipientActionName;
                message2->messageContent = MULTILATERAL_AMEND_MESSAGE;
                messages[player1Name].push_back(message2);

                auto message3 = std::make_shared<livelyRD::MessageMultilateralAction>();
                message3->sender = recipients[2];
                message3->recipients.push_back(player1Name);
                message3->recipientActionName = actionName;
                message3->doublesContent[recipients[2]] = -1.0;
                message3->doublesContent[player1Name + MULTILATERAL_AMEND_PREVIOUS_VALUE] = 0.55;
                message3->senderActionName = recipientActionName;
                message3->messageContent = MULTILATERAL_AMEND_MESSAGE;
                messages[player1Name].push_back(message3);

                std::unordered_map<std::string,
                        std::unique_ptr<livelyRD::UnilateralAction>> unilateralActions;

                WHEN("The function gets evaluated")
                {
                    REQUIRE(actionAsFunction(inputs, outputs, messages, unilateralActions));

                    THEN("The output values should have changed")
                    {
                        REQUIRE(outputs[resourceName] == 2.5);

                        REQUIRE(!messages.find(recipients[0])->second.empty());
                        REQUIRE(messages[recipients[0]].back()->messageContent == MULTILATERAL_REJECTION_MESSAGE);
                        REQUIRE(messages[recipients[0]].back()->doublesContent[player1Name] == 0.0);

                        REQUIRE(!messages.find(recipients[1])->second.empty());
                        REQUIRE(messages[recipients[1]].back()->messageContent == MULTILATERAL_APPROVAL_MESSAGE);
                        REQUIRE(messages[recipients[1]].back()->doublesContent[player1Name] == 1.5);

                        REQUIRE(!messages.find(recipients[2])->second.empty());
                        REQUIRE(messages[recipients[2]].back()->messageContent == MULTILATERAL_REJECTION_MESSAGE);
                        REQUIRE(messages[recipients[2]].back()->doublesContent[player1Name] == 0.0);
                    }
                }
            }
        }
        AND_GIVEN("A set of inputs and a number of time-steps")
        {
            int numberOfTimeSteps = 5;
            std::unordered_map<std::string, double> inputs, currentValues, results;
            inputs[resourceName] = 0.0;
            currentValues[resourceName] = 0.0;
            for(int i = 0; i<numberOfTimeSteps; ++i)
            {
                inputs[actionName + std::to_string(i)] = 1.0;
            }

            THEN("It should be possible to evaluate the function")
            {
                results = actionFormula.calculateExpectedTotalValue(currentValues, inputs, numberOfTimeSteps);
                REQUIRE(results[resourceName] == 5.0);
            }
        }
    }
}
