#include <catch.hpp>
#include <string>
#include <unordered_map>
#include <vector>

#include "AbstractActionFormula.h"
#include "UnilateralActionFormula.h"
#include "ObjectiveFormula.h"

SCENARIO("Calculate a simple objective from a string")
{
    GIVEN("A set of strings representing the objective and the actions")
    {
        std::string resourceName = "resource1";
        std::string actionName = "action1";
        std::unordered_map<std::string, std::string> actionStrings;
        actionStrings[resourceName] = resourceName + "+" + actionName + "_value";
        std::string objectiveString = resourceName + "_total" + "*" + resourceName + "_current";

        WHEN("An objective formula is created from the strings")
        {
            std::vector<std::shared_ptr<livelyRD::AbstractActionFormula>> actionFormula =
                    {std::make_unique<livelyRD::UnilateralActionFormula>(actionStrings)};
            livelyRD::ObjectiveFormula objectiveFormula(objectiveString, actionFormula);

            THEN("We should be able to get the function representing the formula")
            {
                auto objectiveFunction = objectiveFormula.getEquationAsFunction();

                AND_GIVEN("A set of inputs")
                {
                    int numberOfTimeSteps = 10;
                    std::unordered_map<std::string, double> inputs;
                    inputs[resourceName] = 3.0;
                    inputs[PLAYER_NUMBER_OF_TIME_STEPS_NAME] = numberOfTimeSteps;

                    for(int i = 0; i<numberOfTimeSteps; ++i)
                    {
                        inputs[actionName + std::to_string(i)] = 1.0;
                    }

                    WHEN("The objective function is evaluated")
                    {
                        double result = objectiveFunction(inputs);

                        THEN("We should be able to get the right result")
                        {
                            REQUIRE(result == 39.0);
                        }
                    }
                }
            }
        }
    }
}


SCENARIO("Calculate a simple objective with parenthesis")
{
    GIVEN("A set of strings representing the objective and the actions")
    {
        std::string resourceName = "resource1";
        std::string actionName = "action1";
        std::unordered_map<std::string, std::string> actionStrings;
        actionStrings[resourceName] = resourceName + "+" + actionName + "_value";
        std::string objectiveString =
                resourceName + "_total" + "*" + "(" + resourceName + "_current" + "+" + resourceName + "_current)";

        WHEN("An objective formula is created from the strings")
        {
            std::vector<std::shared_ptr<livelyRD::AbstractActionFormula>> actionFormula =
                    {std::make_unique<livelyRD::UnilateralActionFormula>(actionStrings)};
            livelyRD::ObjectiveFormula objectiveFormula(objectiveString, actionFormula);

            THEN("We should be able to get the function representing the formula")
            {
                auto objectiveFunction = objectiveFormula.getEquationAsFunction();

                AND_GIVEN("A set of inputs")
                {
                    int numberOfTimeSteps = 10;
                    std::unordered_map<std::string, double> inputs;
                    inputs[resourceName] = 3.0;
                    inputs[PLAYER_NUMBER_OF_TIME_STEPS_NAME] = numberOfTimeSteps;

                    for(int i = 0; i<numberOfTimeSteps; ++i)
                    {
                        inputs[actionName + std::to_string(i)] = 1.0;
                    }

                    WHEN("The objective function is evaluated")
                    {
                        double result = objectiveFunction(inputs);

                        THEN("We should be able to get the right result")
                        {
                            REQUIRE(result == 78.0);
                        }
                    }
                }
            }
        }
    }
}


SCENARIO("Calculate a simple objective with multiple resources")
{
    GIVEN("A set of strings representing the objective and the actions")
    {
        std::string resource1Name = "resource1";
        std::string resource2Name = "resource2";
        std::string actionName = "action1";
        std::unordered_map<std::string, std::string> actionStrings;
        actionStrings[resource1Name] = resource1Name + "+" + actionName + "_value";
        actionStrings[resource2Name] = resource2Name + "*" + actionName + "_value";
        std::string objectiveString =
                resource1Name + "_total" + "*" + "(" + resource1Name + "_current" + "+" + resource2Name + "_current)";

        WHEN("An objective formula is created from the strings")
        {
            std::vector<std::shared_ptr<livelyRD::AbstractActionFormula>> actionFormula =
                    {std::make_unique<livelyRD::UnilateralActionFormula>(actionStrings)};
            livelyRD::ObjectiveFormula objectiveFormula(objectiveString, actionFormula);

            THEN("We should be able to get the function representing the formula")
            {
                auto objectiveFunction = objectiveFormula.getEquationAsFunction();

                AND_GIVEN("A set of inputs")
                {
                    int numberOfTimeSteps = 10;
                    std::unordered_map<std::string, double> inputs;
                    inputs[resource1Name] = 3.0;
                    inputs[resource2Name] = 2.0;
                    inputs[PLAYER_NUMBER_OF_TIME_STEPS_NAME] = numberOfTimeSteps;

                    for(int i = 0; i<numberOfTimeSteps; ++i)
                    {
                        inputs[actionName + std::to_string(i)] = 1.0;
                    }

                    WHEN("The objective function is evaluated")
                    {
                        double result = objectiveFunction(inputs);

                        THEN("We should be able to get the right result")
                        {
                            REQUIRE(result == 65.0);
                        }
                    }
                }
            }
        }
    }
}