#ifndef MATHPARSER_ABSTRACTMATHFORMULA_H
#define MATHPARSER_ABSTRACTMATHFORMULA_H

#include <string>
#include <vector>
#include <unordered_map>

namespace livelyRD
{
    class AbstractMathFormula
    {

    public:
        struct EquationElement
        {
            std::string name;
            double value;
            std::pair<std::shared_ptr<EquationElement>, std::shared_ptr<EquationElement>> children;
        };

    public:
        explicit AbstractMathFormula(std::unordered_map<std::string, std::string>  equationString);
        virtual ~AbstractMathFormula() = default;

    protected:
        std::unordered_map<std::string, std::string> equationsAsString_;
        std::unordered_map<std::string, std::vector<std::string>> equationsAsVector_;
        std::unordered_map<std::string, std::vector<std::shared_ptr<EquationElement>>> equationsAsTree_;

    private:
        void separateElementsEquation();
        void shuntingYardSorting();
    };
}

#endif
