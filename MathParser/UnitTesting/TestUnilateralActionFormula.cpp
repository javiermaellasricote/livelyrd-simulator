#include <catch.hpp>
#include <vector>
#include <unordered_map>
#include <string>

#include "UnilateralActionFormula.h"

SCENARIO("Calculate a simple action from a string")
{
    GIVEN("An equation represented as a string of characters")
    {
        std::string resourceName = "resource1";
        std::string actionName = "action";
        std::unordered_map<std::string, std::string> action;
        action[resourceName] = "resource1 + action_value + 3.5";

        WHEN("The equation is converted to an UnilateralActionFormula")
        {
            livelyRD::UnilateralActionFormula actionFormula(action);

            THEN("It should be possible to get the equation as a function")
            {
                std::vector<std::function<bool(const std::unordered_map<std::string, double>& inputs,
                                   std::unordered_map<std::string, double>& outputs)>> equationAsFunction;

                equationAsFunction.push_back(actionFormula.getEquationsAsFunction());
                REQUIRE(!equationAsFunction.empty());

                AND_GIVEN("A set of inputs and outputs")
                {
                    int numberOfTimeSteps = 5;
                    std::unordered_map<std::string, double> inputs;
                    inputs[resourceName] = 1.0;
                    for(int i = 0; i<numberOfTimeSteps; ++i)
                    {
                        inputs[actionName + std::to_string(i)] = 1.0;
                    }
                    std::unordered_map<std::string, double> outputs;

                    WHEN("The function gets evaluated")
                    {
                        auto function = equationAsFunction.back();
                        REQUIRE(function(inputs, outputs));

                        THEN("The output values should have changed")
                        {
                            REQUIRE(outputs[resourceName] == 5.5);
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
                    REQUIRE(results[resourceName] == 22.5);
                }
            }
        }
    }
}

SCENARIO("Calculate a simple equation with multiple operations")
{
    GIVEN("A simple equation with multiple operations")
    {
        std::string resourceName = "resource1";
        std::string auxiliaryResource = "resource2";
        std::string actionName = "action";
        std::unordered_map<std::string, std::string> equationStrings;
        equationStrings[resourceName] = resourceName + "+" + actionName + "_value" + "*" + auxiliaryResource;

        WHEN("The equation is converted to a UnilateralActionFormula")
        {
            livelyRD::UnilateralActionFormula actionFormula(equationStrings);

            AND_GIVEN("A set of inputs and outputs")
            {
                int numberOfTimeSteps = 5;
                std::unordered_map<std::string, double> inputs;
                inputs[resourceName] = 1.0;
                inputs[auxiliaryResource] = 2.5;
                for(int i = 0; i<numberOfTimeSteps; ++i)
                {
                    inputs[actionName + std::to_string(i)] = 1.0;
                }
                std::unordered_map<std::string, double> outputs;

                WHEN("The equation is extracted as a function and it's evaluated")
                {
                    auto actionFunction = actionFormula.getEquationsAsFunction();
                    REQUIRE(actionFunction(inputs, outputs));

                    THEN("The function can be evaluated to get the right result")
                    {
                        REQUIRE(outputs[resourceName] == 3.5);
                    }
                }
            }
        }
    }
}

SCENARIO("Calculate a simple equation with parenthesis")
{
    GIVEN("A simple equation with parenthesis")
    {
        std::string resourceName = "resource1";
        std::string auxiliaryResource = "resource2";
        std::string actionName = "action";
        std::unordered_map<std::string, std::string> equationStrings;
        equationStrings[resourceName] = "(" + resourceName + "+" + actionName + "_value" + ")" + "*" + auxiliaryResource;

        WHEN("The equation is converted to a UnilateralActionFormula")
        {
            livelyRD::UnilateralActionFormula actionFormula(equationStrings);

            AND_GIVEN("A set of inputs and outputs")
            {
                int numberOfTimeSteps = 5;
                std::unordered_map<std::string, double> inputs;
                inputs[resourceName] = 1.0;
                inputs[auxiliaryResource] = 2.5;
                for(int i = 0; i<numberOfTimeSteps; ++i)
                {
                    inputs[actionName + std::to_string(i)] = 1.0;
                }
                std::unordered_map<std::string, double> outputs;

                WHEN("The equation is extracted as a function and it's evaluated")
                {
                    auto actionFunction = actionFormula.getEquationsAsFunction();
                    REQUIRE(actionFunction(inputs, outputs));

                    THEN("The function can be evaluated to get the right result")
                    {
                        REQUIRE(outputs[resourceName] == 5.0);
                    }
                }
            }
        }
    }
}


SCENARIO("Calculate a simple set of equations")
{
    GIVEN("A simple equation with parenthesis")
    {
        std::string resourceName = "resource1";
        std::string auxiliaryResource = "resource2";
        std::string actionName = "action";
        std::unordered_map<std::string, std::string> equationStrings;
        equationStrings[resourceName] = "(" + resourceName + "+" + actionName + "_value" + ")" + "*" + auxiliaryResource;
        equationStrings[auxiliaryResource] = resourceName + "+" + actionName + "_value" + "*" + auxiliaryResource;

        WHEN("The equation is converted to a UnilateralActionFormula")
        {
            livelyRD::UnilateralActionFormula actionFormula(equationStrings);

            AND_GIVEN("A set of inputs and outputs")
            {
                int numberOfTimeSteps = 5;
                std::unordered_map<std::string, double> inputs, currentValues, results;
                inputs[resourceName] = 1.0;
                currentValues[resourceName] = 1.0;
                inputs[auxiliaryResource] = 2.5;
                currentValues[auxiliaryResource] = 2.5;
                for(int i = 0; i<numberOfTimeSteps; ++i)
                {
                    inputs[actionName + std::to_string(i)] = 1.0;
                }
                std::unordered_map<std::string, double> outputs;

                WHEN("The equation is extracted as a function and it's evaluated")
                {
                    auto actionFunction = actionFormula.getEquationsAsFunction();
                    REQUIRE(actionFunction(inputs, outputs));

                    THEN("The function can be evaluated to get the right result")
                    {
                        REQUIRE(outputs[resourceName] == 5.0);
                        REQUIRE(outputs[auxiliaryResource] == 3.5);
                    }
                }

                WHEN("The expected value of the function is evaluated")
                {
                    results = actionFormula.calculateExpectedTotalValue(currentValues, inputs, numberOfTimeSteps);

                    THEN("An accurate solution should be obtained")
                    {
                        REQUIRE(results[resourceName] == 1200925.5);
                        REQUIRE(results[auxiliaryResource] == 5762.5);
                    }
                }
            }
        }
    }
}