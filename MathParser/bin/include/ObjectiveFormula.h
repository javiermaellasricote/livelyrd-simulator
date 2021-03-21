#ifndef MATHPARSER_OBJECTIVEFORMULA_H
#define MATHPARSER_OBJECTIVEFORMULA_H

#include <ModellingLibrary.h>

#include <string>
#include <vector>
#include <unordered_map>
#include <AbstractMathFormula.h>
#include "AbstractMathFormula.h"

namespace livelyRD
{
    class AbstractActionFormula;

    class ObjectiveFormula : public AbstractMathFormula
    {
    public:
        ObjectiveFormula(const std::string &equationString,
                         std::vector<std::shared_ptr<AbstractActionFormula>> actions);
        ~ObjectiveFormula() override = default;
        [[nodiscard]] std::function<double(const std::unordered_map<std::string, double>& inputs)>
        getEquationAsFunction();

    private:
        std::vector<std::shared_ptr<AbstractActionFormula>> actions_;
    };
}


#endif