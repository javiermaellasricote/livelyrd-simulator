#ifndef DYCORSOPTIMIZERS_H
#define DYCORSOPTIMIZERS_H

#include <vector>
#include <unordered_map>
#include <string>
#include <Eigen/Dense>

#include "RbfModel.h"

namespace livelyRD
{
    class DycorsOptimizer
    {
    public:
        struct InputInfo
        {
            std::string name;
            double upperBound;
            double lowerBound;
        };

        struct OutputInfo
        {
            std::string name;
            bool isToBeOptimized;
            bool isToBeMaximized;
            bool isConstraint;
            double upperBound;
            double lowerBound;
        };

        struct PopulationElement
        {
            std::unordered_map<std::string, double> inputValues;
            std::unordered_map<std::string, double> solutionValues;
            bool isBestOfIteration;
            bool constraintsHaveBeenChecked;
        };

    public:
        explicit DycorsOptimizer(
                std::function<std::unordered_map<std::string, double>
                        (const std::unordered_map<std::string, double>&)>& blackBoxFunction);
        ~DycorsOptimizer() = default;
        bool run();
        [[nodiscard]] const std::vector<livelyRD::DycorsOptimizer::PopulationElement> & getOptimalPoints();
        [[nodiscard]] const std::vector<livelyRD::DycorsOptimizer::PopulationElement> & getAllPoints() const;

        void setInputs(const std::vector<InputInfo>& inputs);
        void setOutputs(const std::vector<OutputInfo>& outputs);
        void setNumberOfEvaluations(const int& numberOfEvaluations);
        void setInitialStepSize(const double& initialStepSize);
        void setMinimumStepSize(const double& minimumStepSize);
        void setNumberOfTrialPoints(const int& numberOfTrialPoints);
        void setConsecutiveFailTolerance(const int& consecutiveFailTolerance);
        void setConsecutiveSuccessTolerance(const int& consecutiveSuccessTolerance);
        void setRbfWeight(const double& rbfWeight);

    private:

        bool evaluateLatinHyperCube();
        bool evaluateBlackBox();
        bool classifyPopulationElements();
        bool evaluateConstraints(const PopulationElement& element);
        bool selectBestOfIteration();
        bool isFirstElementBehindSecond(const PopulationElement& firstElement,
                                        const PopulationElement& secondElement);
        bool initializeDycors();

        bool updateModel();
        bool updateMutationProbability();
        bool generateTrialPoints();
        bool mutateTrialPoints();
        bool evaluateTrialPoints();
        bool selectNextIterate();
        bool calculateScores();
        bool caclulateRbfScores();
        bool calculateDistanceScores();
        bool updateCounters();
        bool adjustStepSize();
        bool selectOptimal();


        double calculatePointsDistance(const PopulationElement& firstPoint,
                                       const PopulationElement& secondPoint);

        std::vector<std::unordered_map<std::string, double>> latinHyperCube();


        std::function<std::unordered_map<std::string, double>(const std::unordered_map<std::string, double>&)>
                blackBoxFunction_;
        int failureCounter_;
        int successCounter_;
        int numberOfEvaluations_;
        double stepSize_;
        double initialStepSize_;
        double minimumStepSize_;
        int numberOfTrialPoints_;
        int consecutiveFailTolerance_;
        int consecutiveSuccessTolerance_;
        double rbfWeight_;
        std::vector<OutputInfo> outputs_;
        std::vector<InputInfo> inputs_;
        int numberOfSamplePoints_{};
        std::vector<PopulationElement> population_;
        std::vector<PopulationElement> trialPoints_;
        livelyRD::RbfModel model_;
        double mutationProbability_{};
        std::unordered_map<std::string, double> maximumTrialPointsOutputs_;
        std::unordered_map<std::string, double> minimumTrialPointsOutputs_;
        std::vector<double> trialPointsRbfScores_;
        std::vector<double> trialPointsDistanceScores_;
        std::vector<double> trialPointsScores_;
        std::vector<PopulationElement> optimalPoints_;
        Eigen::MatrixXd inputsMatrix_;
        Eigen::MatrixXd outputsMatrix_;
        //TODO: Implement complex constraints where values are dependent on each other!!
    };
}

#endif