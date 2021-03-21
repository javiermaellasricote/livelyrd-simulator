#ifndef UNILATERALACTION_H
#define UNILATERALACTION_H

#include <functional>
#include <vector>
#include <string>
#include <unordered_map>
#include "AbstractAction.h"
namespace livelyRD
{
    class UnilateralAction : public AbstractAction
    {
    public:
        UnilateralAction
        (
            const std::string& name,
            const std::string& owner,
            std::function<bool(const std::unordered_map<std::string, double>& inputs,
                    std::unordered_map<std::string, double>& outputs)> actionToBeTaken,
            std::function<bool(const std::unordered_map<std::string, double>& inputs)> decisionToBeTaken,
            bool isActionBlocked = false,
            const double& lowerBound = 0.0,
            const double& upperBound = 1.0
        );
        ~UnilateralAction() override = default;

        bool decideIfTakingAction(const std::unordered_map<std::string, double>& inputs,
                const std::string& playerRequestingAction) override;
        bool getIsToBePerformed(const std::string& playerRequestingInfo) override;
        bool resetIsToBePerformed(const std::string& playerRequestingAction) override;
        bool performAction(const std::unordered_map<std::string, double>& inputs,
                std::unordered_map<std::string, double>& outputs,
                const std::string& playerRequestingAction) override;

    private:
        DataVariable<bool> isToBePerformed_{};
        std::function<bool(const std::unordered_map<std::string, double>& inputs,
                std::unordered_map<std::string, double>& outputs)> actionToBeTaken_;
        std::function<bool(const std::unordered_map<std::string, double>& inputs)> decisionToBeTaken_;
        DataVariable<bool> isActionBlocked_{};
    };
}

#endif
