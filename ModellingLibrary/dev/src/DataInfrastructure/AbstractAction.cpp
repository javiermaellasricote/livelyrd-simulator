//
// Created by Javi Maellas on 2020-02-05.
//

#include "AbstractAction.h"

livelyRD::AbstractAction::AbstractAction(const std::string& name, const std::string& owner,
        const double& lowerBound, const double& upperBound) :
AbstractDataElement(name, owner)
{
    lowerBound_.value = lowerBound;
    lowerBound_.isPublic = false;
    upperBound_.value = upperBound;
    upperBound_.isPublic = false;
}

bool livelyRD::AbstractAction::addSubscriber(const std::string &playerName, const std::string &playerRequestingAction)
{
    return false;
}

bool livelyRD::AbstractAction::eliminateSubscriber(const std::string &playerName,
                                                   const std::string &playerRequestingAction)
{
    return false;
}

bool livelyRD::AbstractAction::changeSubscriberValue(const double &value, const std::string &playerName,
                                                     const std::string &playerRequestingAction)
{
    return false;
}

bool livelyRD::AbstractAction::receiveMessage(std::shared_ptr<livelyRD::MessageMultilateralAction>& message)
{
    return false;
}

const double &livelyRD::AbstractAction::getLowerBound() const
{
    return lowerBound_.value;
}

const double &livelyRD::AbstractAction::getUpperBound() const
{
    return upperBound_.value;
}

void livelyRD::AbstractAction::setCurrentEnvironment(std::shared_ptr<Environment> &currentEnvironment)
{

}
