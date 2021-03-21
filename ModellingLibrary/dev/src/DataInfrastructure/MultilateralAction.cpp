//
// Created by Javi Maellas on 2020-01-20.
//

#include "MultilateralAction.h"
#include "Environment.h"

livelyRD::MultilateralAction::MultilateralAction
        (
                const std::string& name,
                const std::string& owner,
                std::function<bool(const std::unordered_map<std::string, double>& inputs,
                        std::unordered_map<std::string, double>& outputs, std::unordered_map<std::string,
                        std::vector<std::shared_ptr<livelyRD::MessageMultilateralAction>>>& messages,
                        std::unordered_map<std::string, std::unique_ptr<UnilateralAction>>& availableActions)>
                actionToBeTaken,

                std::function<bool(const std::unordered_map<std::string, double>& inputs,
                                   std::unordered_map<std::string,
                                   std::vector<std::shared_ptr<livelyRD::MessageMultilateralAction>>>& messages)>
                decisionToBeTaken,
                bool isActionBlocked,
                const double& lowerBound,
                const double& upperBound

        ) : AbstractAction(name, owner, lowerBound, upperBound)
{
    actionsToPerform_.isPublic = false;
    subscriberInfo_.isPublic = false;

    actionToBeTaken_ = actionToBeTaken;
    decisionToBeTaken_ = decisionToBeTaken;
    isActionBlocked_.value = isActionBlocked;
    isActionBlocked_.isPublic = false;
    isToBePerformed_.value = false;
    isToBePerformed_.isPublic = false;

    currentEnvironment_ = nullptr;

}

bool livelyRD::MultilateralAction::addActionToList(const std::string& actionName,
                                                   std::unique_ptr<livelyRD::UnilateralAction>& newAction,
                                                   const std::string &playerRequestingAction)
{
    if(playerRequestingAction == owner_.value)
    {
        if (actionsToPerform_.value.find(actionName) == actionsToPerform_.value.end())
        {
            actionsToPerform_.value.insert({actionName, std::move(newAction)});
            return true;
        }
    }
    return false;
}

bool livelyRD::MultilateralAction::eliminateActionFromList(const std::string &actionName,
                                                           const std::string& playerRequestingAction)
{
    if(playerRequestingAction == owner_.value)
    {
        if (actionsToPerform_.value.find(actionName) != actionsToPerform_.value.end())
        {
            actionsToPerform_.value.erase(actionName);
            return true;
        }
    }
    return false;
}

bool
livelyRD::MultilateralAction::addSubscriber(const std::string &playerName, const std::string &playerRequestingAction)
{
    if(playerRequestingAction == owner_.value)
    {
        if (subscriberInfo_.value.find(playerName) == subscriberInfo_.value.end())
        {
            subscriberInfo_.value.insert({playerName, 0.0});
            return true;
        }
    }
    return false;
}

bool livelyRD::MultilateralAction::eliminateSubscriber(const std::string &playerName,
                                                       const std::string &playerRequestingAction)
{
    if(playerRequestingAction == owner_.value)
    {
        if (subscriberInfo_.value.find(playerName) != subscriberInfo_.value.end())
        {
            subscriberInfo_.value.erase(playerName);
            return true;
        }
    }
    return false;
}

bool livelyRD::MultilateralAction::changeSubscriberValue(const double &value, const std::string &playerName,
                                                         const std::string &playerRequestingAction)
{
    if(playerRequestingAction == owner_.value)
    {
        auto iter = subscriberInfo_.value.find(playerName);
        if(iter != subscriberInfo_.value.end())
        {
            subscriberInfo_.value[playerName] = value;
            return true;
        }
        else
        {
            return false;
        }
    }
    return false;
}

void livelyRD::MultilateralAction::setCurrentEnvironment(std::shared_ptr<livelyRD::Environment>& currentEnvironment)
{
    currentEnvironment_ = currentEnvironment;
}

bool livelyRD::MultilateralAction::spreadMessage(const std::string& recipient)
{
    bool failure = false;
    if (currentEnvironment_ != nullptr && recipient != name_.value)
    {
        for(auto& message : messages_[recipient])
        {
            failure += !currentEnvironment_->sendMessage(message);
        }
        if(!failure)
        {
            messages_.erase(recipient);
        }
        return !failure;
    }
    else
    {
        return !failure;
    }
}

bool livelyRD::MultilateralAction::decideIfTakingAction(const std::unordered_map<std::string, double> &inputs,
                                                        const std::string& playerRequestingAction)
{
    if (playerRequestingAction == owner_.value)
    {
        isToBePerformed_.value = decisionToBeTaken_(inputs, messages_);
        return isToBePerformed_.value;
    }
    else
    {
        return false;
    }
}

bool livelyRD::MultilateralAction::getIsToBePerformed(const std::string& playerRequestingInfo)
{
    if (isToBePerformed_.isPublic || playerRequestingInfo == owner_.value)
        return isToBePerformed_.value;
    else
        return false;
}

bool livelyRD::MultilateralAction::resetIsToBePerformed(const std::string& playerRequestingAction)
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

bool livelyRD::MultilateralAction::performAction(const std::unordered_map<std::string, double> &inputs,
        std::unordered_map<std::string, double> &outputs, const std::string& playerRequestingAction)
{
    if ((playerRequestingAction == owner_.value) && (isToBePerformed_.value))
    {
        bool actionSuccess, messageSuccess;
        isToBePerformed_.value = false;
        actionSuccess = actionToBeTaken_(inputs, outputs, messages_, actionsToPerform_.value);
        messages_.erase(name_.value);
        messageSuccess = sendAllMessages();
        return actionSuccess && messageSuccess;
    }
    else
    {
        return false;
    }
}

bool livelyRD::MultilateralAction::sendAllMessages()
{
    if (currentEnvironment_ != nullptr)
    {
        for(const auto& message : messages_)
        {
            if(message.first != name_.value)
            {
                spreadMessage(message.first);
            }
        }
        return true;
    }
    else
    {
        return false;
    }
}

bool livelyRD::MultilateralAction::receiveMessage(std::shared_ptr<MessageMultilateralAction>& message)
{
    messages_[owner_.value].push_back(message);
    return true;
}
