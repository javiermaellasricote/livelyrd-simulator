#include "UnilateralActionFormula.h"
#include "UnilateralActionCalculator.h"

livelyRD::UnilateralActionFormula::UnilateralActionFormula(
        const std::unordered_map<std::string, std::string> &equationStrings) :
        AbstractActionFormula(equationStrings)
{

}

std::function<bool(const std::unordered_map<std::string, double> &inputs,
        std::unordered_map<std::string, double> &outputs)> livelyRD::UnilateralActionFormula::getEquationsAsFunction()
{
    std::unordered_map<std::string, std::shared_ptr<livelyRD::AbstractMathFormula::EquationElement>> roots;
    for(auto& equation : equationsAsTree_)
    {
        roots[equation.first] = equation.second.back();
    }
    std::function<bool(const std::unordered_map<std::string, double> &inputs,
                       std::unordered_map<std::string, double> &outputs)> equationFunction =
    [roots](const std::unordered_map<std::string, double> &inputs,std::unordered_map<std::string, double> &outputs)
    {
        UnilateralActionCalculator calculator(roots, inputs, outputs);
        calculator.performAction();
        outputs = calculator.getOutputs();
        return true;
    };
    return equationFunction;
}

std::unordered_map<std::string, double> livelyRD::UnilateralActionFormula::calculateExpectedTotalValue(
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
            results[name] = UnilateralActionCalculator::calculateAction(equationsAsTreeCopy[name].back(), inputs, i);
        }
        for(const auto& equation : equationsAsTreeCopy)
        {
            name = equation.first;
            inputs.find(name)->second = results[name];
        }
    }
    return results;
}

















