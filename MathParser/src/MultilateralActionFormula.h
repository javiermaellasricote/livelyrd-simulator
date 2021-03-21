#ifndef MATHPARSER_MULTILATERALACTIONFORMULA_H
#define MATHPARSER_MULTILATERALACTIONFORMULA_H

#include <vector>
#include <string>
#include <unordered_map>
#include <functional>
#include <ModellingLibrary.h>

#include "AbstractActionFormula.h"

namespace livelyRD
{
    class MultilateralActionFormula : public AbstractActionFormula
    {
    public:
        enum MultilateralActionType : unsigned int
        {
            NEGOTIABLE,
            NON_NEGOTIABLE,
            BIDIRECTIONAL_COMPULSORY,
            SENDER_UNIDIRECTIONAL_COMPULSORY,
            RECEIVER_UNIDIRECTIONAL_COMPULSORY
        };
    public:
        MultilateralActionFormula(const std::unordered_map<std::string, std::string>& equationStrings,
                MultilateralActionType type, std::vector<std::string> potentialRecipients,
                std::string   playerName, std::string   actionName,
                std::string   recipientActionName);
        ~MultilateralActionFormula() override = default;

        std::unordered_map<std::string, double> calculateExpectedTotalValue(
                const std::unordered_map<std::string, double> &values,
                std::unordered_map<std::string, double> inputs,
                int numberOfTimeSteps) override;

        std::function<bool(const std::unordered_map<std::string, double>& inputs,std::unordered_map
                <std::string, double>& outputs, std::unordered_map<std::string, std::vector<std::shared_ptr
                <livelyRD::MessageMultilateralAction>>>& messages, std::unordered_map<std::string,
                std::unique_ptr<livelyRD::UnilateralAction>>& availableActions)> getEquationsAsFunction();

    private:
        std::vector<std::string> potentialRecipients_;
        MultilateralActionType type_;
        std::string  playerName_;
        std::string  actionName_;
        std::string  recipientActionName_;
    };
}

#endif