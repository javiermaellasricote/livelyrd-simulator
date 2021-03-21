#ifndef PLAYER_H
#define PLAYER_H

#include <memory>
#include <vector>
#include <unordered_map>
#include <string>

#include "DefinitionsPlayer.h"
#include "../Engines/DycorsOptimizer.h"
#include "MessageMultilateralAction.h"
#include "AbstractDataElement.h"
#include "Resource.h"
#include "Objective.h"
#include "AbstractAction.h"

namespace livelyRD
{
    class Player : public AbstractDataElement
    {
    public:

        Player(const std::string& name, const std::string& owner);
        ~Player() override = default;

        bool addResource(std::unique_ptr<livelyRD::Resource>& newResource);
        bool removeResource(const std::string& resourceName);
        [[nodiscard]] const std::unique_ptr<Resource>& getResource(const std::string& resourceName) const;
        [[nodiscard]] const DataVariable<std::unordered_map<std::string, std::unique_ptr<Resource>>>&
        getResources() const;
        bool setResourcesListPublic(const std::string& playerRequestingAction);
        bool setResourcesListPrivate(const std::string& playerRequestingAction);

        bool addObjective(std::unique_ptr<Objective>& newObjective);
        bool removeObjective(const std::string& objectiveName);
        [[nodiscard]] const std::unique_ptr<Objective>& getObjective(const std::string& objectiveName) const;
        bool setObjectiveListPublic(const std::string& playerRequestingAction);
        bool setObjectiveListPrivate(const std::string& playerRequestingAction);
        void setNumberOfTimeSteps(const int& timeSteps);
        void addPreferences(std::function<DycorsOptimizer::PopulationElement
                (const std::vector<DycorsOptimizer::PopulationElement>& solutions)> timeStepPreferences);

        bool addAction(std::unique_ptr<AbstractAction>& newAction);
        bool removeAction(const std::string& actionName);
        [[nodiscard]] const std::unique_ptr<AbstractAction>& getAction(
                const std::string& actionName,
                const std::string &playerRequestingInfo) const;
        bool setActionListPublic(const std::string& playerRequestingAction);
        bool setActionListPrivate(const std::string& playerRequestingAction);
        bool receiveMessage(std::shared_ptr<MessageMultilateralAction>& message);

        bool decideNextMove(const std::unordered_map<std::string, double>& environmentInputs);
        bool executeNextMove(const int &iterationNumber);
        [[nodiscard]] const std::string& getTimeStepName() const;
        void setTimeStepName(const std::string& name);



    private:
        using BlackBoxFunction = std::function<std::unordered_map<std::string, double>
                (const std::unordered_map<std::string, double>&)>;

        BlackBoxFunction createBlackBox(const std::unordered_map<std::string, double>& environmentInputs);
        std::vector<DycorsOptimizer::OutputInfo> createOptimizerOutputs();
        std::vector<DycorsOptimizer::InputInfo> createOptimizerInputs();
        bool selectPointFromParetoFront(const std::vector<DycorsOptimizer::PopulationElement>& solutions);

        DataVariable<std::unordered_map<std::string, std::unique_ptr<Resource>>> resources_;
        DataVariable<std::unordered_map<std::string, std::unique_ptr<Objective>>> objectives_;
        DataVariable<std::unordered_map<std::string, std::unique_ptr<AbstractAction>>> actions_;
        std::unique_ptr<Resource> nullResourcePointer_{};
        std::unique_ptr<Objective> nullObjectivePointer_{};
        std::unique_ptr<AbstractAction> nullActionPointer_{};
        std::function<DycorsOptimizer::PopulationElement
        (const std::vector<DycorsOptimizer::PopulationElement>& solutions)> timeStepPreferences_;

        DataVariable<std::unordered_map<std::string, double>> nextMoveInputs_;
        int numberOfTimeSteps_;
        std::string currentTimeStepName_;
        //For actions that require more than one time-step to finish:
        DataVariable<std::vector<std::string>> actionsInExecution_; //TODO: this should be deprecated.
    };
}

#endif
