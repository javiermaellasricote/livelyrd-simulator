#include "UnilateralAction.h"

livelyRD::UnilateralAction::UnilateralAction
(
        const std::string& name, const std::string& owner,
        std::function<bool(const std::unordered_map<std::string, double>& inputs,
                std::unordered_map<std::string, double>& outputs)> actionToBeTaken,
        std::function<bool(const std::unordered_map<std::string, double>& inputs)> decisionToBeTaken,
        bool isActionBlocked,
        const double& lowerBound,
        const double& upperBound
) : AbstractAction(name, owner, lowerBound, upperBound), actionToBeTaken_(actionToBeTaken),
decisionToBeTaken_(decisionToBeTaken)
{
    isActionBlocked_.value = isActionBlocked;
    isActionBlocked_.isPublic = false;
    isToBePerformed_.value = false;
    isToBePerformed_.isPublic = false;
}

bool livelyRD::UnilateralAction::decideIfTakingAction(const std::unordered_map<std::string, double> &inputs,
        const std::string& playerRequestingAction)
{
    if (playerRequestingAction == owner_.value)
    {
        isToBePerformed_.value = decisionToBeTaken_(inputs);
        return isToBePerformed_.value;
    }
    else
    {
        return false;
    }
}

bool livelyRD::UnilateralAction::getIsToBePerformed(const std::string& playerRequestingInfo)
{
    if (isToBePerformed_.isPublic)
        return isToBePerformed_.value;
    else if (playerRequestingInfo == owner_.value)
        return isToBePerformed_.value;
    else
        return false;
}

bool livelyRD::UnilateralAction::resetIsToBePerformed(const std::string& playerRequestingAction)
{
    if (playerRequestingAction == owner_.value)
    {
        isToBePerformed_.value = false;
        return true;
    }
    else
    {
        return false;
    }
}

bool livelyRD::UnilateralAction::performAction
(
    const std::unordered_map<std::string, double> &inputs,
    std::unordered_map<std::string, double> &outputs,
    const std::string& playerRequestingAction
)
{
    if ((playerRequestingAction == owner_.value) && (isToBePerformed_.value))
    {
        isToBePerformed_.value = false;
        return actionToBeTaken_(inputs, outputs);
    }
    else
    {
        return false;
    }
}
