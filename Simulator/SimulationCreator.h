#ifndef SIMULATOR_SIMULATIONCREATOR_H
#define SIMULATOR_SIMULATIONCREATOR_H

#include <unordered_map>
#include <ModellingLibrary.h>
#include <MathParser.h>

#include "MongoDb/MongoDb.h"

namespace livelyRD
{
    class SimulationCreator
    {
        using MultilateralType = livelyRD::MultilateralActionFormula::MultilateralActionType;

    public:
        SimulationCreator(std::string environmentName, mongocxx::collection simulationData,
                          const mongocxx::instance &instance);
        ~SimulationCreator() = default;

        const std::shared_ptr<Environment>& getSimulationEnvironment() const;

    private:
        [[nodiscard]] std::shared_ptr<livelyRD::Environment> createEnvironment();

        [[nodiscard]] std::unique_ptr<Resource> createResource(const std::string &name, const std::string &owner,
                const double &initialValue = 0.0);

        [[nodiscard]] std::unique_ptr<AbstractAction> createUnilateralAction(const std::string &name,
                const std::string &owner,
                const std::unordered_map<std::string, std::string> &equations,
                const double &lowerBound = 0.0,
                const double &upperBound = 1.0);

        [[nodiscard]] std::unique_ptr<AbstractAction> createMultilateralAction(const std::string &name,
                const std::string &owner,
                const std::unordered_map<std::string, std::string> &equations,
                const MultilateralActionFormula::MultilateralActionType &type,
                const std::vector<std::string> &recipients,
                const std::string &recipientActionName,
                const double &lowerBound = 0.0,
                const double &upperBound = 1.0);

        [[nodiscard]] std::unique_ptr<Player> createPlayer(const std::string &name, const std::string &owner);


        [[nodiscard]] std::unique_ptr<livelyRD::Objective>
        createObjective(std::string &name, std::string &owner, const std::string &equation,
                        const double &initialValue, const bool &isToBeMaximized,
                        const double &lowerBound, const double &upperBound);

        [[nodiscard]] std::vector<std::unique_ptr<livelyRD::Resource>>
        createAllResources(const bsoncxx::types::b_array &resourcesArray,
                           const std::string &playerName);

        [[nodiscard]] std::vector<std::unique_ptr<livelyRD::AbstractAction>>
        createAllActions(const bsoncxx::types::b_array &actionsArray,
                         const std::string &playerName);

        [[nodiscard]] std::vector<std::unique_ptr<livelyRD::Objective>>
        createAllObjectives(const bsoncxx::types::b_array &objectivesArray,
                         const std::string &playerName);

    private:

        std::string environmentName_;
        std::unordered_map<std::string, std::unique_ptr<Player>> players_{};
        std::shared_ptr<livelyRD::Environment> environment_;
        const mongocxx::instance& mongoInstance_;
        mongocxx::collection simulationData_;
        std::vector<std::shared_ptr<livelyRD::AbstractActionFormula>> actionFormulas_;
        int numberOfTimeSteps_{};
    };
}

#endif