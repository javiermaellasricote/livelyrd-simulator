//
// Created by Javi Maellas on 2020-04-29.
//

#include <unordered_map>
#include <utility>
#include "ObjectiveFormula.h"
#include "ObjectiveCalculator.h"
#define EQUATION_NAME "."

livelyRD::ObjectiveFormula::ObjectiveFormula(const std::string &equationString,
                                             std::vector<std::shared_ptr<AbstractActionFormula>> actions) :
        AbstractMathFormula(std::unordered_map<std::string, std::string>{{EQUATION_NAME, equationString}}),
        actions_(std::move(actions))
{

}

std::function<double(const std::unordered_map<std::string, double> &inputs)>
livelyRD::ObjectiveFormula::getEquationAsFunction()
{
    auto root = equationsAsTree_[EQUATION_NAME].back();
    auto actions = actions_;
    std::function<double(const std::unordered_map<std::string, double> &inputs)> equationAsFunction =
    [root, actions](const std::unordered_map<std::string, double> &inputs)
    {
        return livelyRD::calculateObjectiveEquation(root, inputs, actions);
    };
    return equationAsFunction;
}
