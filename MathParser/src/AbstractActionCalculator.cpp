#include <regex>
#include <utility>

#include "AbstractActionCalculator.h"

livelyRD::AbstractActionCalculator::AbstractActionCalculator(
        std::unordered_map<std::string, std::shared_ptr<livelyRD::AbstractMathFormula::EquationElement>> roots,
        std::unordered_map<std::string, double> inputs, std::unordered_map<std::string, double> &outputs) :
        roots_(std::move(roots)), inputs_(std::move(inputs)), outputs_(outputs)
{

}

double livelyRD::AbstractActionCalculator::calculateAction(
        const std::shared_ptr<livelyRD::AbstractMathFormula::EquationElement> &root,
        const std::unordered_map<std::string, double> &inputs, int currentTimeStep)
{
    if(root->name == "+")
    {
        root->value = calculateAction(root->children.first, inputs) +
                      calculateAction(root->children.second, inputs);
    }
    else if(root->name == "-")
    {
        root->value = calculateAction(root->children.first, inputs) -
                      calculateAction(root->children.second, inputs);
    }
    else if(root->name == "*")
    {
        root->value = calculateAction(root->children.first, inputs) *
                      calculateAction(root->children.second, inputs);
    }
    else if(root->name == "/")
    {
        root->value = calculateAction(root->children.first, inputs) /
                      calculateAction(root->children.second, inputs);
    }
    else
    {
        std::regex valueRegex("(.*)(_value)");
        std::string name = root->name;
        if(std::regex_match(name, valueRegex))
        {
            name = std::regex_replace(name, valueRegex, "$1");
            name += std::to_string(currentTimeStep);
        }
        auto iterInput = inputs.find(name);
        if(iterInput != inputs.end())
        {
            root->value = iterInput->second;
        }
        else
        {
            root->value = std::stod(root->name);
        }
    }
    return root->value;
}

const std::unordered_map<std::string, double> &livelyRD::AbstractActionCalculator::getOutputs()
{
    return outputs_;
}
