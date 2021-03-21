#include "SimulationCreator.h"
#include "DefinitionsDb.h"

#include <utility>

livelyRD::SimulationCreator::SimulationCreator(std::string environmentName, mongocxx::collection simulationData,
                                               const mongocxx::instance &instance) :
        environmentName_(std::move(environmentName)), simulationData_(std::move(simulationData)), mongoInstance_(instance)
{
    environment_ = createEnvironment();

    mongocxx::pipeline findPlayersPipeline{};
    findPlayersPipeline.match(make_document(kvp(TYPE_KEY, PLAYER_TYPE)));
    auto playersCursor = simulationData_.aggregate(findPlayersPipeline);
    for(auto&& player : playersCursor)
    {
        std::string playerName = player[NAME_KEY].get_utf8().value.to_string();
        players_[playerName] = createPlayer(playerName, environmentName_);

        auto resourcesArray = player[RESOURCES_KEY].get_array();
        auto resources = createAllResources(resourcesArray, playerName);

        auto actionsArray = player[ACTIONS_KEY].get_array();
        auto actions = createAllActions(actionsArray, playerName);

        auto objectivesArray = player[OBJECTIVE_KEY].get_array();
        auto objectives = createAllObjectives(objectivesArray,playerName);

        for(auto& resource : resources)
        {
            players_[playerName]->addResource(resource);
        }
        for(auto& action : actions)
        {
            players_[playerName]->addAction(action);
        }
        for(auto& objective : objectives)
        {
            players_[playerName]->addObjective(objective);
        }
    }
    for(auto & player : players_)
    {
        environment_->addPlayer(player.first,player.second);
    }
}





[[nodiscard]] std::shared_ptr<livelyRD::Environment> livelyRD::SimulationCreator::createEnvironment()
{
    auto environmentInfo = simulationData_.find_one(make_document(kvp(TYPE_KEY, ENVIRONMENT_TYPE)).view());
    numberOfTimeSteps_ = environmentInfo.value().view()[TIME_STEPS_KEY].get_double();

    auto environment = std::make_shared<livelyRD::Environment>(environmentName_);
    environment->setNumberOfTimeSteps(numberOfTimeSteps_);
    return environment;
}

[[nodiscard]] std::unique_ptr<livelyRD::Resource> livelyRD::SimulationCreator::createResource(
        const std::string& name,
        const std::string& owner,
        const double& initialValue)
{
    return std::make_unique<livelyRD::Resource>(name, owner, initialValue);
}

[[nodiscard]] std::unique_ptr<livelyRD::AbstractAction> livelyRD::SimulationCreator::createUnilateralAction(
        const std::string& name,
        const std::string& owner,
        const std::unordered_map<std::string, std::string>& equations,
        const double& lowerBound,
        const double& upperBound)
{
    std::function<bool(const std::unordered_map<std::string, double>& inputs)> decisionTrue =
            [](const std::unordered_map<std::string, double>& inputs){return true;};

    auto actionFormula = std::make_shared<livelyRD::UnilateralActionFormula>(equations);
    auto action = actionFormula->getEquationsAsFunction();
    actionFormulas_.push_back(actionFormula);

    return std::make_unique<livelyRD::UnilateralAction>(
            name, owner, action, decisionTrue, false, lowerBound, upperBound);
}

[[nodiscard]] std::unique_ptr<livelyRD::AbstractAction> livelyRD::SimulationCreator::createMultilateralAction(
        const std::string& name,
        const std::string& owner,
        const std::unordered_map<std::string, std::string>& equations,
        const livelyRD::MultilateralActionFormula::MultilateralActionType& type,
        const std::vector<std::string>& recipients,
        const std::string& recipientActionName,
        const double& lowerBound,
        const double& upperBound)
{
    std::function<bool(const std::unordered_map<std::string, double>& inputs,
                       std::unordered_map<std::string, std::vector<std::shared_ptr
                               <livelyRD::MessageMultilateralAction>>>& messages)> decisionTrue =
            [](const std::unordered_map<std::string, double>& inputs,
               std::unordered_map<std::string, std::vector<std::shared_ptr<livelyRD::MessageMultilateralAction>>>&
               messages){return true;};

    auto actionFormula = std::make_shared<livelyRD::MultilateralActionFormula>(
            equations, type, recipients, owner, name, recipientActionName);
    auto action = actionFormula->getEquationsAsFunction();
    actionFormulas_.push_back(actionFormula);

    return std::make_unique<livelyRD::MultilateralAction>(name, owner, action, decisionTrue,
                                                          false, lowerBound, upperBound);
}

[[nodiscard]] std::unique_ptr<livelyRD::Player> livelyRD::SimulationCreator::createPlayer(
        const std::string& name,
        const std::string& owner)
{
    auto player = std::make_unique<livelyRD::Player>(name, owner);

    std::function<livelyRD::DycorsOptimizer::PopulationElement(
    const std::vector<livelyRD::DycorsOptimizer::PopulationElement>& solutions)> preferences = [](
    const std::vector<livelyRD::DycorsOptimizer::PopulationElement>& solutions)
    {
        return *std::max_element(solutions.begin(), solutions.end(), []
                (const livelyRD::DycorsOptimizer::PopulationElement& a,
                        const livelyRD::DycorsOptimizer::PopulationElement& b)
        {
            return (a.solutionValues.begin()->second) < (a.solutionValues.begin()->second);
        });
    };
    player->addPreferences(preferences);
    return player;
}

[[nodiscard]] std::unique_ptr<livelyRD::Objective>
livelyRD::SimulationCreator::createObjective(std::string &name, std::string &owner, const std::string &equation,
                                             const double &initialValue, const bool &isToBeMaximized,
                                             const double &lowerBound, const double &upperBound)
{
    livelyRD::ObjectiveFormula objectiveFormula(equation, actionFormulas_);
    auto objectiveFunction = objectiveFormula.getEquationAsFunction();
    return std::make_unique<livelyRD::Objective>(name, owner, objectiveFunction, initialValue,
                                                 isToBeMaximized, upperBound, lowerBound);
}

std::vector<std::unique_ptr<livelyRD::Resource>>
livelyRD::SimulationCreator::createAllResources(const bsoncxx::types::b_array &resourcesArray,
                                                const std::string &playerName)
{
    std::vector<std::unique_ptr<livelyRD::Resource>> results;

    for(const auto & resource : resourcesArray.value)
    {
        auto resourceInfo = resource.get_document().value;
        std::string resourceName = resourceInfo[NAME_KEY].get_utf8().value.to_string();
        double initialValue = resourceInfo[INITIAL_VALUE_KEY].get_double();
        results.push_back(createResource(resourceName, playerName, initialValue));
    }
    return results;
}

[[nodiscard]] std::vector<std::unique_ptr<livelyRD::AbstractAction>>
livelyRD::SimulationCreator::createAllActions(const bsoncxx::types::b_array &actionsArray,
                                              const std::string &playerName)
{
    std::vector<std::unique_ptr<livelyRD::AbstractAction>> results;

    for(const auto& action : actionsArray.value)
    {
        auto actionInfo = action.get_document().value;
        std::string actionName = actionInfo[NAME_KEY].get_utf8().value.to_string();

        std::unordered_map<std::string, std::string> equationStrings;
        auto equationsArray = actionInfo[EQUATIONS_KEY].get_array();
        for(const auto& equation : equationsArray.value)
        {
            auto equationInfo = equation.get_document().value;
            std::string equationName = equationInfo[NAME_KEY].get_utf8().value.to_string();
            std::string equationValue = equationInfo[EQUATIONS_KEY].get_utf8().value.to_string();
            equationStrings[equationName] = equationValue;
        }

        double upperBound = actionInfo[UPPER_BOUND_KEY].get_double();
        double lowerBound = actionInfo[LOWER_BOUND_KEY].get_double();
        std::string actionType = actionInfo[TYPE_KEY].get_utf8().value.to_string();

        if(actionType == UNILATERAL_ACTION_TYPE)
        {
            results.push_back(createUnilateralAction(actionName,playerName,equationStrings,lowerBound, upperBound));
        }
        else
        {
            MultilateralType type;

            if(actionType == NEGOTIABLE_ACTION_TYPE)
            {
                type = MultilateralType::NEGOTIABLE;
            }
            else if(actionType == NON_NEGOTIABLE_ACTION_TYPE)
            {
                type = MultilateralType::NON_NEGOTIABLE;
            }
            else if(actionType == BIDIRECTIONAL_COMPULSORY_ACTION_TYPE)
            {
                type = MultilateralType::BIDIRECTIONAL_COMPULSORY;
            }
            else if(actionType == SENDER_UNIDIRECTIONAL_COMPULSORY_ACTION_TYPE)
            {
                type = MultilateralType::SENDER_UNIDIRECTIONAL_COMPULSORY;
            }
            else
            {
                type = MultilateralType::RECEIVER_UNIDIRECTIONAL_COMPULSORY;
            }

            std::vector<std::string> recipientStrings;
            auto recipientsArray = actionInfo[RECIPIENTS_KEY].get_array();
            recipientStrings.resize(recipientsArray.value.length());
            std::size_t j = 0;
            for(const auto& recipient : recipientsArray.value)
            {
                recipientStrings[j] = recipient.get_utf8().value.to_string();
                ++j;
            }

            std::string recipientActionName = actionInfo[RECIPIENT_ACTION_KEY].get_utf8().value.to_string();

            results.push_back(createMultilateralAction(actionName, playerName, equationStrings, type,
                    recipientStrings, recipientActionName, lowerBound, upperBound));
        }
    }
    return results;
}

std::vector<std::unique_ptr<livelyRD::Objective>>
livelyRD::SimulationCreator::createAllObjectives(const bsoncxx::types::b_array &objectivesArray,
                                                 const std::string &playerName)
{
    std::vector<std::unique_ptr<livelyRD::Objective>> results;
    std::string playerNameCopy = playerName;

    for(const auto & objective : objectivesArray.value)
    {
        auto objectiveInfo = objective.get_document().value;
        std::string objectiveName = objectiveInfo[NAME_KEY].get_utf8().value.to_string();
        std::string equation = objectiveInfo[EQUATIONS_KEY].get_utf8().value.to_string();
        double initialValue = objectiveInfo[INITIAL_VALUE_KEY].get_double();
        bool isToBeMaximized = objectiveInfo[IS_TO_BE_MAXIMIZED].get_bool();
        double lowerBound = objectiveInfo[LOWER_BOUND_KEY].get_double();
        double upperBound = objectiveInfo[UPPER_BOUND_KEY].get_double();

        results.push_back(createObjective(objectiveName,playerNameCopy,equation,
                initialValue,isToBeMaximized,lowerBound,upperBound));
    }
    return results;

}

const std::shared_ptr<livelyRD::Environment>& livelyRD::SimulationCreator::getSimulationEnvironment() const
{
    return environment_;
}
























