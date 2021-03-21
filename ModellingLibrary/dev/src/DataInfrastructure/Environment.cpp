#include <functional>
#include <memory>
#include <thread>
#include "Environment.h"

livelyRD::Environment::Environment(const std::string &name) : AbstractDataElement(name, "none")
{

}

bool livelyRD::Environment::sendMessage(std::shared_ptr<MessageMultilateralAction>& message)
{
    bool success = false;
    for(const auto& recipientName : message->recipients)
    {
        success = listOfPlayers_[recipientName]->receiveMessage(message);
    }
    return success;
}

const std::unique_ptr<livelyRD::Resource>& livelyRD::Environment::getExternalResource(const std::string &name) const
{
    if(externalResources_.find(name) != externalResources_.end())
    {
        return externalResources_.find(name)->second.value;
    }
    return nullResourcePtr_;
}

bool livelyRD::Environment::addExternalResource(const std::string& name,
        std::unique_ptr<livelyRD::Resource> &resource, bool isPublic)
{
    if(externalResources_.find(name) == externalResources_.end())
    {
        DataVariable<std::unique_ptr<Resource>> dataVariable;
        dataVariable.isPublic = isPublic;
        dataVariable.value = std::move(resource);
        externalResources_[name] = std::move(dataVariable);
        return true;
    }
    return false;
}

bool livelyRD::Environment::addPlayer(const std::string& name, std::unique_ptr<livelyRD::Player>& player)
{
    if(listOfPlayers_.find(name) == listOfPlayers_.end())
    {
        listOfPlayers_[name] = std::move(player);
        return true;
    }
    return false;
}

bool livelyRD::Environment::solve()
{
    if(listOfPlayers_.empty() || numberOfTimeSteps_ <= 0)
    {
        return false;
    }

    std::unordered_map<std::string, double> environmentInputs;
    std::string playerName;
    std::vector<std::unique_ptr<std::thread>> threads;
    for(std::size_t i = 0; i<numberOfTimeSteps_; ++i)
    {
        for(auto & player : listOfPlayers_)
        {
            playerName = player.second->getName(name_.value);
            player.second->setNumberOfTimeSteps(numberOfTimeSteps_);
            for(const auto& resource : player.second->getResources().value)
            {
                environmentInputs[playerName + "_" + resource.second->getName(playerName)] =
                        resource.second->getQuantity(playerName);
            }
        }

        threads.resize(listOfPlayers_.size());
        std::size_t j = 0;
        for(auto & player : listOfPlayers_)
        {
            //player.second->decideNextMove(environmentInputs);
            threads[j] = std::make_unique<std::thread>(&Player::decideNextMove, player.second.get(), environmentInputs);
            ++j;
        }
        for(auto& thread : threads)
        {
            thread->join();
        }
        threads.clear();

        for(auto & player : listOfPlayers_)
        {
            player.second->executeNextMove(environmentInputs, i);
        }
    }
    return true;
}

void livelyRD::Environment::setNumberOfTimeSteps(const int &numberOfTimeSteps)
{
    numberOfTimeSteps_ = numberOfTimeSteps;
}

const std::unordered_map<std::string, std::unique_ptr<livelyRD::Player>>& livelyRD::Environment::getAllPlayers() const
{
    return listOfPlayers_;
}
