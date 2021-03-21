#ifndef MATHPARSER_ABSTRACTACTIONFORMULA_H
#define MATHPARSER_ABSTRACTACTIONFORMULA_H

#include <vector>
#include <string>

#include "AbstractMathFormula.h"

namespace livelyRD
{
    class AbstractActionFormula : public AbstractMathFormula
    {
    public:
        explicit AbstractActionFormula(const std::unordered_map<std::string, std::string>& equationStrings);
        ~AbstractActionFormula() override = default;

        virtual std::unordered_map<std::string, double> calculateExpectedTotalValue(
                const std::unordered_map<std::string, double> &values,
                std::unordered_map<std::string, double> inputs,
                int numberOfTimeSteps) = 0;

        std::vector<std::string> getModifiedResourcesNames();

    private:

    };
}

#endif
