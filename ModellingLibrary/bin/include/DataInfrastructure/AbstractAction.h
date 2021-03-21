#ifndef ABSTRACTACTION_H
#define ABSTRACTACTION_H

#include <memory>
#include <string>
#include <unordered_map>
#include "AbstractDataElement.h"
#include "MessageMultilateralAction.h"

namespace livelyRD
{
    class Environment;

    class AbstractAction : public AbstractDataElement
    {
    public:
        AbstractAction(const std::string& name, const std::string& owner,
        const double& lowerBound = 0.0, const double& upperBound = 1.0);
        virtual ~AbstractAction() = default;
        virtual void setCurrentEnvironment(std::shared_ptr<Environment>& currentEnvironment);

        virtual bool decideIfTakingAction(const std::unordered_map<std::string, double>& inputs,
                const std::string& playerRequestingAction) = 0;
        virtual bool getIsToBePerformed(const std::string& playerRequestingInfo) = 0;
        virtual bool resetIsToBePerformed(const std::string& playerRequestingAction) = 0;
        virtual bool performAction
                (
                        const std::unordered_map<std::string, double>& inputs,
                        std::unordered_map<std::string, double>& outputs,
                        const std::string& playerRequestingAction
                ) = 0;

        virtual bool addSubscriber(const std::string& playerName, const std::string& playerRequestingAction);
        virtual bool eliminateSubscriber(const std::string& playerName, const std::string& playerRequestingAction);
        virtual bool changeSubscriberValue(const double& value,
                                   const std::string& playerName,
                                   const std::string& playerRequestingAction);
        virtual bool receiveMessage(std::shared_ptr<MessageMultilateralAction>& message);

        [[nodiscard]] const double& getLowerBound() const;
        [[nodiscard]] const double& getUpperBound() const;

    private:

        DataVariable<double> lowerBound_{};
        DataVariable<double> upperBound_{};
    };
}

#endif
