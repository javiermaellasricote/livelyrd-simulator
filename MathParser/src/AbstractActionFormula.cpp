#include "AbstractActionFormula.h"

livelyRD::AbstractActionFormula::AbstractActionFormula(
        const std::unordered_map<std::string, std::string>& equationStrings) :
        AbstractMathFormula(equationStrings)
{

}

std::vector<std::string> livelyRD::AbstractActionFormula::getModifiedResourcesNames()
{
    std::vector<std::string> names;
    for(const auto& equation : equationsAsTree_)
    {
        names.push_back(equation.first);
    }
    return names;
}
