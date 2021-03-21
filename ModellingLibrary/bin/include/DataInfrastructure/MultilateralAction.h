#ifndef MULTILATERALACTION_H
#define MULTILATERALACTION_H

#include <memory>
#include <functional>
#include <unordered_map>
#include <string>
#include <vector>
#include "AbstractAction.h"
#include "UnilateralAction.h"
#include "MessageMultilateralAction.h"

namespace livelyRD
{
    class Environment;

    class MultilateralAction : public AbstractAction
    {
    public:
        MultilateralAction
        (
            const std::string& name,
            const std::string& owner,
            std::function<bool(const std::unordered_map<std::string, double>& inputs,
                    std::unordered_map<std::string, double>& outputs, std::unordered_map<std::string,
                    std::vector<std::shared_ptr<MessageMultilateralAction>>>& messages,
                    std::unordered_map<std::string, std::unique_ptr<UnilateralAction>>& availableActions)>
                    actionToBeTaken,

            std::function<bool(const std::unordered_map<std::string, double>& inputs,
                    std::unordered_map<std::string, std::vector<std::shared_ptr<MessageMultilateralAction>>>&
                            messages)> decisionToBeTaken,
            bool isActionBlocked = false,
            const double& lowerBound = 0.0,
            const double& upperBound = 1.0
        );
        ~MultilateralAction() override = default;

        void setCurrentEnvironment(std::shared_ptr<Environment>& currentEnvironment) override;

        bool addActionToList
        (
                const std::string& actionName,
                std::unique_ptr<UnilateralAction>& newAction,
                const std::string& playerRequestingAction
        );
        bool eliminateActionFromList(const std::string& actionName, const std::string& playerRequestingAction);

        bool addSubscriber(const std::string& playerName, const std::string& playerRequestingAction) override;
        bool eliminateSubscriber(const std::string& playerName, const std::string& playerRequestingAction) override;
        bool changeSubscriberValue(const double& value,
                                   const std::string& playerName,
                                   const std::string& playerRequestingAction) override;

        bool decideIfTakingAction(const std::unordered_map<std::string, double>& inputs,
                const std::string& playerRequestingAction) override;
        bool getIsToBePerformed(const std::string& playerRequestingInfo) override;
        bool resetIsToBePerformed(const std::string& playerRequestingAction) override;
        bool performAction
        (
                const std::unordered_map<std::string, double>& inputs,
                std::unordered_map<std::string, double>& outputs,
                const std::string& playerRequestingAction
        ) override;
        bool receiveMessage(std::shared_ptr<MessageMultilateralAction>& message) override;


    private:

        bool spreadMessage(const std::string& recipient);
        bool sendAllMessages();

        DataVariable<bool> isToBePerformed_;

        std::function<bool(const std::unordered_map<std::string, double>& inputs,
                std::unordered_map<std::string, double>& outputs, std::unordered_map<std::string,
                std::vector<std::shared_ptr<MessageMultilateralAction>>>& messages,
                std::unordered_map<std::string, std::unique_ptr<UnilateralAction>>& availableActions)>
                actionToBeTaken_;

        std::function<bool(const std::unordered_map<std::string, double>& inputs,
                std::unordered_map<std::string, std::vector<std::shared_ptr<MessageMultilateralAction>>>&
                messages)> decisionToBeTaken_;

        DataVariable<bool> isActionBlocked_;
        DataVariable<std::unordered_map<std::string, std::unique_ptr<UnilateralAction>>> actionsToPerform_;
        DataVariable<std::unordered_map<std::string, double>> subscriberInfo_;
        std::unordered_map<std::string, std::vector<std::shared_ptr<MessageMultilateralAction>>> messages_;
        std::shared_ptr<Environment> currentEnvironment_;
    };
}

#endif
