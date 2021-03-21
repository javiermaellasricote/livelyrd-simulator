#include <regex>
#include <utility>

#include "UnilateralActionCalculator.h"


livelyRD::UnilateralActionCalculator::UnilateralActionCalculator(
        std::unordered_map<std::string, std::shared_ptr<livelyRD::AbstractMathFormula::EquationElement>> roots,
        std::unordered_map<std::string, double> inputs, std::unordered_map<std::string, double> &outputs)
        : AbstractActionCalculator(std::move(roots), std::move(inputs), outputs)
{

}

void livelyRD::UnilateralActionCalculator::performAction()
{
    for(const auto& root : roots_)
    {
        root.second->value = outputs_[root.first];
        outputs_[root.first] = calculateAction(root.second, inputs_);
    }
}
