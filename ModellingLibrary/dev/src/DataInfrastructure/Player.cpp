#include <functional>

#include "Player.h"

livelyRD::Player::Player(const std::string& name, const std::string& owner) : AbstractDataElement(name, owner),
currentTimeStepName_(PLAYER_ENVIRONMENT_CURRENT_TIME_STEP_NAME)
{
    resources_.isPublic = false;
    objectives_.isPublic = false;
    actions_.isPublic = false;
    nextMoveInputs_.isPublic = false;
    numberOfTimeSteps_ = 0;
}

bool livelyRD::Player::addResource(std::unique_ptr<livelyRD::Resource>& newResource)
{
    std::string newResourceName = newResource->getName(name_.value);
    if(!newResourceName.empty())
    {
        resources_.value.insert({newResourceName, std::move(newResource)});
        return true;
    }
    else
    {
        return false;
    }
}

bool livelyRD::Player::removeResource(const std::string &resourceName)
{
    resources_.value.erase(resourceName);
    return (getResource(resourceName) == nullptr);
}

const std::unique_ptr<livelyRD::Resource>& livelyRD::Player::getResource(const std::string &resourceName) const
{
    auto iter = resources_.value.find(resourceName);

    if (iter == resources_.value.end())
    {
        return nullResourcePointer_;
    }
    else
    {
        if (resources_.isPublic == true)
        {
            return iter->second;
        }
        else
        {
            return nullResourcePointer_;
        }
    }
}

bool livelyRD::Player::setResourcesListPublic(const std::string &playerRequestingAction)
{
    if (playerRequestingAction == owner_.value)
    {
        resources_.isPublic = true;
        return true;
    }
    else
    {
        return false;
    }
}

bool livelyRD::Player::setResourcesListPrivate(const std::string &playerRequestingAction)
{
    if (playerRequestingAction == owner_.value)
    {
        resources_.isPublic = false;
        return true;
    }
    else
    {
        return false;
    }
}

bool livelyRD::Player::addObjective(std::unique_ptr<livelyRD::Objective>& newObjective)
{
    std::string newObjectiveName = newObjective->getName(name_.value);
    if(!newObjectiveName.empty())
    {
        objectives_.value.insert({newObjectiveName, std::move(newObjective)});
        return true;
    }
    else
    {
        return false;
    }
}

bool livelyRD::Player::removeObjective(const std::string &objectiveName)
{
    objectives_.value.erase(objectiveName);
    return (getObjective(objectiveName) == nullptr);
}

const std::unique_ptr<livelyRD::Objective>& livelyRD::Player::getObjective(const std::string &objectiveName) const
{
    auto iter = objectives_.value.find(objectiveName);

    if (iter == objectives_.value.end())
    {
        return nullObjectivePointer_;
    }
    else
    {
        if (objectives_.isPublic == true)
        {
            return iter->second;
        }
        else
        {
            return nullObjectivePointer_;
        }
    }
}

bool livelyRD::Player::setObjectiveListPublic(const std::string &playerRequestingAction)
{
    if (playerRequestingAction == owner_.value)
    {
        objectives_.isPublic = true;
        return true;
    }
    else
    {
        return false;
    }
}

bool livelyRD::Player::setObjectiveListPrivate(const std::string &playerRequestingAction)
{
    if (playerRequestingAction == owner_.value)
    {
        objectives_.isPublic = false;
        return true;
    }
    else
    {
        return false;
    }
}

bool livelyRD::Player::addAction(std::unique_ptr<livelyRD::AbstractAction>& newAction)
{
    std::string newActionName = newAction->getName(name_.value);
    if(!newActionName.empty())
    {
        actions_.value.insert({newActionName, std::move(newAction)});
        return true;
    }
    else
    {
        return false;
    }
}

bool livelyRD::Player::removeAction(const std::string &actionName)
{
    actions_.value.erase(actionName);
    return (getResource(actionName) == nullptr);
}

const std::unique_ptr<livelyRD::AbstractAction>& livelyRD::Player::getAction(
        const std::string &actionName,
        const std::string &playerRequestingInfo) const
{
    auto iter = actions_.value.find(actionName);

    if (iter == actions_.value.end())
    {
        return nullActionPointer_;
    }
    else
    {
        if (actions_.isPublic == true || playerRequestingInfo == owner_.value)
        {
            return iter->second;
        }
        else
        {
            return nullActionPointer_;
        }
    }
}

bool livelyRD::Player::setActionListPublic(const std::string &playerRequestingAction)
{
    if (playerRequestingAction == owner_.value)
    {
        actions_.isPublic = true;
        return true;
    }
    else
    {
        return false;
    }
}

bool livelyRD::Player::setActionListPrivate(const std::string &playerRequestingAction)
{
    if (playerRequestingAction == owner_.value)
    {
        objectives_.isPublic = false;
        return true;
    }
    else
    {
        return false;
    }
}

bool livelyRD::Player::receiveMessage(std::shared_ptr<livelyRD::MessageMultilateralAction>& message)
{
    return actions_.value[message->recipientActionName]->receiveMessage(message);;
}

bool livelyRD::Player::decideNextMove(const std::unordered_map<std::string, double>& environmentInputs)
{
    BlackBoxFunction blackBox = createBlackBox(environmentInputs);
    std::vector<DycorsOptimizer::OutputInfo> optimizerOutputs = createOptimizerOutputs();
    std::vector<DycorsOptimizer::InputInfo> optimizerInputs = createOptimizerInputs();

    DycorsOptimizer optimizer(blackBox);

    optimizer.setNumberOfEvaluations(PLAYER_OPTIMIZER_NUMBER_OF_EVALUATIONS);
    optimizer.setInitialStepSize(PLAYER_OPTIMIZER_INITIAL_STEP_SIZE);
    optimizer.setMinimumStepSize(PLAYER_OPTIMIZER_MIN_STEP_SIZE);
    optimizer.setNumberOfTrialPoints(PLAYER_OPTIMIZER_NUMBER_OF_NEIGHBOURS);
    optimizer.setConsecutiveFailTolerance(PLAYER_OPTIMIZER_CONSECUTIVE_FAIL_TOLERANCE);
    optimizer.setConsecutiveSuccessTolerance(PLAYER_OPTIMIZER_CONSECUTIVE_SUCCESS_TOLERANCE);
    optimizer.setRbfWeight(PLAYER_OPTIMIZER_RBF_WEIGHT);
    optimizer.setInputs(optimizerInputs);
    optimizer.setOutputs(optimizerOutputs);

    optimizer.run();
    std::vector<DycorsOptimizer::PopulationElement> solution;
    solution = optimizer.getOptimalPoints();
    selectPointFromParetoFront(solution);
    return true;
}

livelyRD::Player::BlackBoxFunction livelyRD::Player::createBlackBox(
        const std::unordered_map<std::string, double>& environmentInputs)
{
    BlackBoxFunction blackBox = [this, environmentInputs](const std::unordered_map<std::string, double>& inputs)
    {
        std::unordered_map<std::string, double> objectiveInputs = inputs;
        for(const auto& resource : this->resources_.value)
        {
            objectiveInputs[this->name_.value + "_" + resource.first] = resource.second->getQuantity(this->name_.value);
        }
        for(const auto& input : environmentInputs)
        {
            objectiveInputs[input.first] = input.second;
        }

        objectiveInputs[PLAYER_NUMBER_OF_TIME_STEPS_NAME] = numberOfTimeSteps_;

        std::unordered_map<std::string, double> results;
        for(auto& objective : this->objectives_.value)
        {
            objective.second->calculateObjective(objectiveInputs, this->name_.value);
            results[objective.first] = objective.second->getCurrentValue(this->name_.value);
        }
        return results;
    };
    return blackBox;
}

std::vector<livelyRD::DycorsOptimizer::OutputInfo> livelyRD::Player::createOptimizerOutputs()
{
    std::vector<DycorsOptimizer::OutputInfo> optimizerOutputs(objectives_.value.size());
    std::size_t i = 0;
    for(auto& objective : objectives_.value)
    {
        optimizerOutputs[i].name = objective.first;
        optimizerOutputs[i].isToBeOptimized = true;
        optimizerOutputs[i].isToBeMaximized = objective.second->getIsToBeMaximized();
        optimizerOutputs[i].upperBound = objective.second->getUpperBound();
        optimizerOutputs[i].lowerBound = objective.second->getLowerBound();
        optimizerOutputs[i].isConstraint = false;
        ++i;
    }
    return optimizerOutputs;
}

std::vector<livelyRD::DycorsOptimizer::InputInfo> livelyRD::Player::createOptimizerInputs()
{
    std::vector<DycorsOptimizer::InputInfo> optimizerInputs(actions_.value.size() * numberOfTimeSteps_);
    int i = 0;
    for(auto& action : actions_.value)
    {
        for(int j = 0; j< numberOfTimeSteps_; ++j)
        {
            optimizerInputs[i].name = action.first + std::to_string(j);
            optimizerInputs[i].lowerBound = action.second->getLowerBound();
            optimizerInputs[i].upperBound = action.second->getUpperBound();
            ++i;
        }
    }
    return optimizerInputs;
}

bool livelyRD::Player::executeNextMove(const std::unordered_map<std::string, double> &environmentInputs,
        const int &iterationNumber)
{
    std::unordered_map<std::string, double> inputs = nextMoveInputs_.value;
    std::unordered_map<std::string, double> actionOutputs;
    for(const auto& input : environmentInputs)
    {
        inputs[input.first] = input.second;
    }
    for(const auto& resource : resources_.value)
    {
        actionOutputs[name_.value + "_" + resource.first] = resource.second->getQuantity(name_.value);
        inputs[name_.value + "_" + resource.first] = resource.second->getQuantity(name_.value);
    }

    nextMoveInputs_.value[currentTimeStepName_] = iterationNumber;
    inputs[currentTimeStepName_] = iterationNumber;
    for(auto& action : actions_.value)
    {
        action.second->decideIfTakingAction(inputs, name_.value);
        action.second->performAction(inputs,actionOutputs, name_.value);
    }

    for(auto& resource : resources_.value)
    {
        resource.second->setQuantity(actionOutputs.find(name_.value + "_" + resource.first)->second);
    }
    return true;
}

void livelyRD::Player::setNumberOfTimeSteps(const int& timeSteps)
{
    numberOfTimeSteps_ = timeSteps;
}

bool livelyRD::Player::selectPointFromParetoFront(const std::vector<DycorsOptimizer::PopulationElement>& solutions)
{
    nextMoveInputs_.value = timeStepPreferences_(solutions).inputValues;
    return true;
}

void livelyRD::Player::addPreferences(std::function<DycorsOptimizer::PopulationElement(
        const std::vector<DycorsOptimizer::PopulationElement> &)> timeStepPreferences)
{
    timeStepPreferences_ = timeStepPreferences;
}

const livelyRD::AbstractAction::DataVariable<std::unordered_map<std::string, std::unique_ptr<livelyRD::Resource>>>&
livelyRD::Player::getResources() const
{
    return resources_;
}

const std::string& livelyRD::Player::getTimeStepName() const
{
    return currentTimeStepName_;
}

void livelyRD::Player::setTimeStepName(const std::string &name)
{
    currentTimeStepName_ = name;
}
