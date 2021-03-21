#ifndef MATHPARSER_UNILATERALACTIONCALCULATOR_H
#define MATHPARSER_UNILATERALACTIONCALCULATOR_H

#include <unordered_map>
#include <string>
#include "AbstractMathFormula.h"
#include "AbstractActionCalculator.h"


namespace livelyRD
{
    class UnilateralActionCalculator : public AbstractActionCalculator
    {
    public:
        UnilateralActionCalculator(
                std::unordered_map<std::string, std::shared_ptr<livelyRD::AbstractMathFormula::EquationElement>> roots,
                std::unordered_map<std::string, double>  inputs,
                std::unordered_map<std::string, double>& outputs);

        void performAction() override;
    };
}

#endif
