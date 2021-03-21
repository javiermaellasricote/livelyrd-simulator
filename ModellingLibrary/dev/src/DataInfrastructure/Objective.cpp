#include "Objective.h"

#include <utility>

livelyRD::Objective::Objective
(
    std::string& name,
    std::string& owner,
    std::function<double(const std::unordered_map<std::string, double>& inputs)> objectiveCalculator,
    double initialValue,
    bool isToBeMaximized,
    double upperBound,
    double lowerBound
) : AbstractDataElement(name, owner), objectiveCalculator_(objectiveCalculator)
{
    currentValue_.value = initialValue;
    currentValue_.isPublic = false;
    isToBeMaximized_.value = isToBeMaximized;
    isToBeMaximized_.isPublic = false;
    lowerBound_.value = lowerBound;
    lowerBound_.isPublic = false;
    upperBound_.value = upperBound;
    upperBound_.isPublic = false;
}

double livelyRD::Objective::getCurrentValue(const std::string &playerRequestingInfo)
{
    if (currentValue_.isPublic)
        return currentValue_.value;
    else if (playerRequestingInfo == owner_.value)
        return currentValue_.value;
    else
        return -1.0;
}

bool livelyRD::Objective::setCurrentValuePublic(const std::string &playerRequestingAction)
{
    if (playerRequestingAction == owner_.value)
    {
        currentValue_.isPublic = true;
        return true;
    }
    else
    {
        return false;
    }
}

bool livelyRD::Objective::setCurrentValuePrivate(const std::string &playerRequestingAction)
{
    if (playerRequestingAction == owner_.value)
    {
        currentValue_.isPublic = false;
        return true;
    }
    else
    {
        return false;
    }
}

bool
livelyRD::Objective::calculateObjective(const std::unordered_map<std::string, double>& inputs,
        const std::string &playerRequestingAction)
{
    if (playerRequestingAction == owner_.value)
    {
        currentValue_.value = objectiveCalculator_(inputs);
        return true;
    }
    else
    {
        return false;
    }
}

bool livelyRD::Objective::getIsToBeMaximized() const
{
    return isToBeMaximized_.value;
}

const double &livelyRD::Objective::getUpperBound() const
{
    return upperBound_.value;
}

const double &livelyRD::Objective::getLowerBound() const
{
    return lowerBound_.value;
}
