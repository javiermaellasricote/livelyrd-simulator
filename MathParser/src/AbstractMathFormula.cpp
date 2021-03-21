#include "AbstractMathFormula.h"

#include <utility>

livelyRD::AbstractMathFormula::AbstractMathFormula(std::unordered_map<std::string, std::string>  equationString) :
equationsAsString_(std::move(equationString))
{
    separateElementsEquation();
    shuntingYardSorting();
}

void livelyRD::AbstractMathFormula::separateElementsEquation()
{
    std::string element;

    for(auto& equation : equationsAsString_)
    {
        element = "";
        for(auto& character : equation.second)
        {
            if(character == ' ')
            {
                if(!element.empty())
                {
                    equationsAsVector_[equation.first].push_back(element);
                    element = "";
                }
            }
            else if(character == '+' || character == '-' ||
                    character == '*' || character == '/' ||
                    character == '(' || character == ')')
            {
                if(!element.empty())
                {
                    equationsAsVector_[equation.first].push_back(element);
                    element = "";
                }
                element += character;
                equationsAsVector_[equation.first].push_back(element);
                element = "";
            }
            else
            {
                element += character;
            }
        }
        if(!element.empty())
        {
            equationsAsVector_[equation.first].push_back(element);
            element = "";
        }
    }
}

void livelyRD::AbstractMathFormula::shuntingYardSorting()
{
    std::vector<std::shared_ptr<EquationElement>> operatorStack;
    std::vector<std::shared_ptr<EquationElement>> orphanChildren;

    for(auto& equationAsVector : equationsAsVector_)
    {
        operatorStack.clear();
        orphanChildren.clear();

        for(const auto & elementName : equationAsVector.second)
        {
            if((elementName == "+" || elementName == "-") ||
               ((elementName == "*" || elementName == "/") && (!operatorStack.empty()) &&
                (operatorStack.back()->name == "*" || operatorStack.back()->name == "/")))
            {
                if(!operatorStack.empty() && operatorStack.back()->name != "(")
                {
                    int i = operatorStack.size() - 1;
                    std::shared_ptr<EquationElement> stackElement = operatorStack[i];
                    while(stackElement->name != "(" && i >= 0)
                    {
                        stackElement = operatorStack[i];
                        stackElement->children.second = orphanChildren.back();
                        orphanChildren.pop_back();
                        stackElement->children.first = orphanChildren.back();
                        orphanChildren.pop_back();
                        equationsAsTree_[equationAsVector.first].push_back(stackElement);
                        orphanChildren.push_back({});
                        orphanChildren.back() = equationsAsTree_[equationAsVector.first].back();
                        --i;
                    }

                    operatorStack.resize(i + 1);
                }

                auto element = std::make_shared<EquationElement>();
                element->name = elementName;
                element->value = 0.0;
                operatorStack.push_back(element);
            }
            else if(elementName == "*" || elementName == "/" || elementName == "(")
            {
                auto element = std::make_shared<EquationElement>();
                element->name = elementName;
                element->value = 0.0;
                operatorStack.push_back(element);
            }
            else if(elementName == ")")
            {
                while(operatorStack.back()->name != "(")
                {
                    operatorStack.back()->children.second = orphanChildren.back();
                    orphanChildren.pop_back();
                    operatorStack.back()->children.first = orphanChildren.back();
                    orphanChildren.pop_back();
                    equationsAsTree_[equationAsVector.first].push_back(operatorStack.back());
                    orphanChildren.push_back({});
                    orphanChildren.back() = equationsAsTree_[equationAsVector.first].back();
                    operatorStack.pop_back();
                }
                if(operatorStack.back()->name == "(")
                {
                    operatorStack.pop_back();
                }
            }
            else
            {
                auto element = std::make_shared<EquationElement>();
                element->name = elementName;
                element->value = 0.0;
                equationsAsTree_[equationAsVector.first].push_back(element);
                orphanChildren.push_back({});
                orphanChildren.back() = equationsAsTree_[equationAsVector.first].back();
            }
        }
        if(!operatorStack.empty() && operatorStack.back()->name != "(")
        {
            int i = operatorStack.size() - 1;
            std::shared_ptr<EquationElement> stackElement = operatorStack[i];
            while(stackElement->name != "(" && i >= 0)
            {
                stackElement = operatorStack[i];
                stackElement->children.second = orphanChildren.back();
                orphanChildren.pop_back();
                stackElement->children.first = orphanChildren.back();
                orphanChildren.pop_back();
                equationsAsTree_[equationAsVector.first].push_back(stackElement);
                orphanChildren.push_back({});
                orphanChildren.back() = equationsAsTree_[equationAsVector.first].back();
                --i;
            }

            operatorStack.resize(operatorStack.size() - i);
        }
    }
}
