#ifndef MATHPARSER_ABSTRACTACTIONCALCULATOR_H
#define MATHPARSER_ABSTRACTACTIONCALCULATOR_H

#include <string>
#include <vector>
#include <unordered_map>
#include "AbstractMathFormula.h"

namespace livelyRD
{
    class AbstractActionCalculator
    {
    public:
        AbstractActionCalculator(
                std::unordered_map<std::string, std::shared_ptr<livelyRD::AbstractMathFormula::EquationElement>> roots,
                std::unordered_map<std::string, double>  inputs,
                std::unordered_map<std::string, double>& outputs);
        virtual ~AbstractActionCalculator() = default;

        virtual void performAction() = 0;

        [[nodiscard]] const std::unordered_map<std::string, double>& getOutputs();

        static double calculateAction(
                const std::shared_ptr<livelyRD::AbstractMathFormula::EquationElement>& root,
                const std::unordered_map<std::string, double> &inputs,
                int currentTimeStep = 0);

    protected:

        const std::unordered_map<std::string, std::shared_ptr<livelyRD::AbstractMathFormula::EquationElement>> roots_;
        const std::unordered_map<std::string, double> inputs_;
        std::unordered_map<std::string, double> outputs_;
    };
}

#endif
