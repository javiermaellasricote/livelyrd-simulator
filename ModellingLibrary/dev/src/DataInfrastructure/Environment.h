#ifndef TESTUNILATERALACTION_H
#define TESTUNILATERALACTION_H

#include <vector>
#include <string>
#include <unordered_map>
#include <memory>

#include "AbstractDataElement.h"
#include "Player.h"
#include "MessageMultilateralAction.h"
#include "Resource.h"

namespace livelyRD
{
    class Environment : public AbstractDataElement
    {
    public:
        explicit Environment(const std::string& name);
        ~Environment() override = default;

        bool sendMessage(std::shared_ptr<MessageMultilateralAction>& message);

        [[nodiscard]] const std::unique_ptr<Resource>& getExternalResource(const std::string& name) const;
        bool addExternalResource(const std::string& name, std::unique_ptr<Resource>& resource, bool isPublic = true);
        bool addPlayer(const std::string& name, std::unique_ptr<Player>& player);
        [[nodiscard]] const std::unordered_map<std::string, std::unique_ptr<Player>>& getAllPlayers() const;

        void setNumberOfTimeSteps(const int& numberOfTimeSteps);

        bool solve();

    private:
        std::unordered_map<std::string, DataVariable<std::unique_ptr<Resource>>> externalResources_;
        std::unordered_map<std::string, std::unique_ptr<Player>> listOfPlayers_;
        std::unique_ptr<Resource> nullResourcePtr_{};
        int numberOfTimeSteps_{};
    };
}

#endif
