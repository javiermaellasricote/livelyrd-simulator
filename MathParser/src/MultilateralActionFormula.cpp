#include "MultilateralActionFormula.h"
#include "MultilateralActionCalculator.h"

#include <utility>

livelyRD::MultilateralActionFormula::MultilateralActionFormula(
        const std::unordered_map<std::string, std::string>& equationStrings,
        livelyRD::MultilateralActionFormula::MultilateralActionType type,
        std::vector<std::string> potentialRecipients,
        std::string   playerName, std::string   actionName, std::string   recipientActionName):
        AbstractActionFormula(equationStrings), potentialRecipients_(std::move(potentialRecipients)), type_(type),
        playerName_(std::move(playerName)), actionName_(std::move(actionName)),
        recipientActionName_(std::move(recipientActionName))
{

}

std::unordered_map<std::string, double> livelyRD::MultilateralActionFormula::calculateExpectedTotalValue(
        const std::unordered_map<std::string, double> &values,
        std::unordered_map<std::string, double> inputs,
        int numberOfTimeSteps)
{
    std::string name;
    std::unordered_map<std::string, double> results;

    auto equationsAsTreeCopy = equationsAsTree_;
    for(auto& equation : equationsAsTreeCopy)
    {
        name = equation.first;
        equation.second.back()->value = values.find(name)->second;
    }

    for(int i = 0; i<numberOfTimeSteps; ++i)
    {
        for(const auto& equation : equationsAsTreeCopy)
        {
            name = equation.first;
            results[name] = MultilateralActionCalculator::calculateAction(equationsAsTreeCopy[name].back(), inputs, i);
        }
        for(const auto& equation : equationsAsTreeCopy)
        {
            name = equation.first;
            inputs.find(name)->second = results[name];
        }
    }
    return results;
}

std::function<bool(const std::unordered_map<std::string, double> &inputs, std::unordered_map
        <std::string, double> &outputs, std::unordered_map<std::string, std::vector<std::shared_ptr
        <livelyRD::MessageMultilateralAction>>> &messages, std::unordered_map<std::string,
        std::unique_ptr<livelyRD::UnilateralAction>> &availableActions)>
livelyRD::MultilateralActionFormula::getEquationsAsFunction()
{
    std::unordered_map<std::string, std::shared_ptr<livelyRD::AbstractMathFormula::EquationElement>> roots;
    for(auto& equation : equationsAsTree_)
    {
        roots[equation.first] = equation.second.back();
    }
    std::function<bool(const std::unordered_map<std::string, double>& inputs,std::unordered_map
            <std::string, double>& outputs, std::unordered_map<std::string, std::vector<std::shared_ptr
            <livelyRD::MessageMultilateralAction>>>& messages, std::unordered_map<std::string,
            std::unique_ptr<livelyRD::UnilateralAction>>& availableActions)> equationFunction =
            [roots, this]
                    (const std::unordered_map<std::string, double>& inputs,std::unordered_map
                            <std::string, double>& outputs, std::unordered_map<std::string, std::vector<std::shared_ptr
                            <livelyRD::MessageMultilateralAction>>>& messages, std::unordered_map<std::string,
                            std::unique_ptr<livelyRD::UnilateralAction>>& availableActions)
            {
                MultilateralActionCalculator calculator(roots, inputs, outputs, type_,
                        potentialRecipients_, messages, playerName_,
                        actionName_, recipientActionName_);
                calculator.performAction();
                outputs = calculator.getOutputs();
                messages = calculator.getMessages();
                return true;
            };
    return equationFunction;
}