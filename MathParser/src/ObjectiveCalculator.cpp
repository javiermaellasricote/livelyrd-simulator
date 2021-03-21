#include <regex>
#include "ObjectiveCalculator.h"
#include "AbstractActionFormula.h"
#include "ModellingLibrary.h"

double livelyRD::calculateObjectiveEquation(const std::shared_ptr<livelyRD::AbstractMathFormula::EquationElement>& root,
        const std::unordered_map<std::string, double> &inputs,
        const std::vector<std::shared_ptr<AbstractActionFormula>> &actions)
{
    if(root->name == "+")
    {
        root->value = calculateObjectiveEquation(root->children.first, inputs, actions) +
                      calculateObjectiveEquation(root->children.second, inputs, actions);
    }
    else if(root->name == "-")
    {
        root->value = calculateObjectiveEquation(root->children.first, inputs, actions) -
                      calculateObjectiveEquation(root->children.second, inputs, actions);
    }
    else if(root->name == "*")
    {
        root->value = calculateObjectiveEquation(root->children.first, inputs, actions) *
                      calculateObjectiveEquation(root->children.second, inputs, actions);
    }
    else if(root->name == "/")
    {
        root->value = calculateObjectiveEquation(root->children.first, inputs, actions) /
                      calculateObjectiveEquation(root->children.second, inputs, actions);
    }
    else
    {
        std::regex totalRegex("(.*)(_total)");
        std::regex currentRegex("(.*)(_current)");
        if(std::regex_match(root->name, totalRegex))
        {
            std::string name = root->name;
            std::regex totalReplace("_total");
            name = std::regex_replace(name, totalReplace, "");
            int numberOfTimeSteps = inputs.find(PLAYER_NUMBER_OF_TIME_STEPS_NAME)->second;

            for(const auto & action : actions)
            {
                std::vector<std::string> resourcesInAction = action->getModifiedResourcesNames();
                std::unordered_map<std::string, double> currentValues, estimationResult;

                for(const auto& actionResourceName : resourcesInAction)
                {
                    if(actionResourceName == name)
                    {
                        currentValues[actionResourceName] = root->value;
                    }
                    else
                    {
                        currentValues[actionResourceName] = inputs.find(actionResourceName)->second;
                    }
                }
                estimationResult = action->calculateExpectedTotalValue(currentValues, inputs, numberOfTimeSteps);
                root->value = estimationResult[name];
            }
        }
        else if(std::regex_match(root->name, currentRegex))
        {
            std::string name = root->name;
            std::regex currentReplace("_current");
            name = std::regex_replace(name, currentReplace, "");
            root->value = inputs.find(name)->second;
        }
    }
    return root->value;
}