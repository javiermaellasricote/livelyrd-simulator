#ifndef OBJECTIVE_H
#define OBJECTIVE_H

#include <functional>
#include <string>
#include <vector>
#include <unordered_map>

#include "AbstractDataElement.h"

namespace livelyRD
{
    class Objective : public AbstractDataElement
    {
    public:
        Objective
        (
           std::string& name,
           std::string& owner,
           std::function<double(const std::unordered_map<std::string, double>& inputs)> objectiveCalculator,
           double initialValue = 0.0,
           bool isToBeMaximized = false,
           double upperBound = 1.0,
           double lowerBound = 0.0
        );
        ~Objective() override = default;

        double getCurrentValue(const std::string& playerRequestingInfo);
        [[nodiscard]] bool getIsToBeMaximized() const;
        [[nodiscard]] const double& getUpperBound() const;
        [[nodiscard]] const double& getLowerBound() const;
        bool setCurrentValuePublic(const std::string& playerRequestingAction);
        bool setCurrentValuePrivate(const std::string& playerRequestingAction);

        bool calculateObjective(const std::unordered_map<std::string, double>& inputs,
                const std::string& playerRequestingAction);

    private:
        DataVariable<bool> isToBeMaximized_{};
        DataVariable<double> currentValue_{};
        DataVariable<double> upperBound_{};
        DataVariable<double> lowerBound_{};
        std::function<double(const std::unordered_map<std::string, double>& inputs)> objectiveCalculator_;
    };
}

#endif