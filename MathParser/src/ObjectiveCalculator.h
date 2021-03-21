#ifndef MATHPARSER_OBJECTIVECALCULATOR_H
#define MATHPARSER_OBJECTIVECALCULATOR_H

#include <unordered_map>
#include <string>
#include "AbstractMathFormula.h"

namespace livelyRD
{
    class AbstractActionFormula;
    double calculateObjectiveEquation(const std::shared_ptr<livelyRD::AbstractMathFormula::EquationElement>& root,
            const std::unordered_map<std::string, double> &inputs,
            const std::vector<std::shared_ptr<AbstractActionFormula>> &actions);
}

#endif
