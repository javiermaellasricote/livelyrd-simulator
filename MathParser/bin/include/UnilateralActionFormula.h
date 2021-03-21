#ifndef MATHPARSER_UNILATERALACTIONFORMULA_H
#define MATHPARSER_UNILATERALACTIONFORMULA_H

#include <functional>
#include <string>
#include <unordered_map>
#include "AbstractActionFormula.h"

namespace livelyRD
{
    class UnilateralActionFormula : public AbstractActionFormula
    {
    public:
        explicit UnilateralActionFormula(const std::unordered_map<std::string, std::string>& equationStrings);
        ~UnilateralActionFormula() override = default;

        std::unordered_map<std::string, double> calculateExpectedTotalValue(
                const std::unordered_map<std::string, double> &values,
                std::unordered_map<std::string, double> inputs,
                int numberOfTimeSteps) override;

        std::function<bool(const std::unordered_map<std::string, double>& inputs,
                           std::unordered_map<std::string, double>& outputs)> getEquationsAsFunction();

    private:

    };
}

#endif
