#include "DycorsOptimizer.h"
#include <numeric>
#include <algorithm>
#include <cmath>
#include <random>
#include "LivelyMath.h"

#include <iostream>

livelyRD::DycorsOptimizer::DycorsOptimizer(
        std::function<std::unordered_map<std::string, double>
                (const std::unordered_map<std::string, double>&)>& blackBoxFunction):
        blackBoxFunction_(blackBoxFunction),
        numberOfEvaluations_(100),
        initialStepSize_(0.98),
        minimumStepSize_(0.01),
        numberOfTrialPoints_(4),
        consecutiveFailTolerance_(20),
        consecutiveSuccessTolerance_(20),
        rbfWeight_(0.6),
        inputs_({}),
        outputs_({}),
        stepSize_(0.98),
        failureCounter_(0),
        successCounter_(0)
{
}

void livelyRD::DycorsOptimizer::setNumberOfEvaluations(const int &numberOfEvaluations)
{
    numberOfEvaluations_ = numberOfEvaluations + 1;
}

void livelyRD::DycorsOptimizer::setInitialStepSize(const double &initialStepSize)
{
    initialStepSize_ = initialStepSize;
}

void livelyRD::DycorsOptimizer::setMinimumStepSize(const double &minimumStepSize)
{
    minimumStepSize_ = minimumStepSize;
}

void livelyRD::DycorsOptimizer::setNumberOfTrialPoints(const int &numberOfTrialPoints)
{
    numberOfTrialPoints_ = numberOfTrialPoints;
}

void livelyRD::DycorsOptimizer::setConsecutiveFailTolerance(const int &consecutiveFailTolerance)
{
    consecutiveFailTolerance_ = consecutiveFailTolerance;
}

void livelyRD::DycorsOptimizer::setConsecutiveSuccessTolerance(const int &consecutiveSuccessTolerance)
{
    consecutiveSuccessTolerance_ = consecutiveSuccessTolerance;
}

void livelyRD::DycorsOptimizer::setRbfWeight(const double &rbfWeight)
{
    rbfWeight_ = rbfWeight;
}

void livelyRD::DycorsOptimizer::setInputs(const std::vector<InputInfo>& inputs)
{
    inputs_ = inputs;
    numberOfSamplePoints_ = 2*(inputs_.size() + 1);
}

void livelyRD::DycorsOptimizer::setOutputs(const std::vector<OutputInfo> &outputs)
{
    outputs_ = outputs;
}

bool livelyRD::DycorsOptimizer::run()
{
    bool isInitialized;
    isInitialized = initializeDycors();
    if(!isInitialized)
    {
        return false;
    }
    int i = 0;
    while((population_.size() < numberOfEvaluations_-1) && i < numberOfEvaluations_*1.2)
    {
        updateModel();
        updateMutationProbability();
        generateTrialPoints();
        selectNextIterate();
        evaluateBlackBox();
        classifyPopulationElements();
        selectBestOfIteration();
        updateCounters();
        adjustStepSize();
        ++i;
    }
    return true;
}

bool livelyRD::DycorsOptimizer::initializeDycors() //O(1)
{
    if (!inputs_.empty() && !outputs_.empty())
    {
        bool successLHC, successBB, successClasify;
        population_.clear();
        inputsMatrix_.resize(0, 0);
        outputsMatrix_.resize(0, 0);
        successLHC = evaluateLatinHyperCube();
        successBB = evaluateBlackBox();
        successClasify = classifyPopulationElements();
        failureCounter_ = 0;
        successCounter_ = 0;
        stepSize_ = initialStepSize_;
        return successBB && successLHC && successClasify;
    }
    else
    {
        return false;
    }
}

bool livelyRD::DycorsOptimizer::evaluateLatinHyperCube() //O(n) n = numberOfSamples_
{
    if (inputs_.empty())
    {
        return false;
    }
    std::vector<std::unordered_map<std::string, double>> solutionLatinHyperCube;
    solutionLatinHyperCube = latinHyperCube();
    int oldPopulationSize = population_.size();
    population_.resize(population_.size() + solutionLatinHyperCube.size());
    for(std::size_t i = 0; i<solutionLatinHyperCube.size(); ++i)
    {
        population_[oldPopulationSize + i].inputValues = solutionLatinHyperCube[i];
    }
    return true;
}

bool livelyRD::DycorsOptimizer::evaluateBlackBox() //O(n)
{
    if(!population_.empty())
    {
        for(auto& element : population_)
        {
            if(element.solutionValues.empty())
            {
                element.solutionValues = blackBoxFunction_(element.inputValues);
                element.isBestOfIteration = true;
                element.constraintsHaveBeenChecked = false;
            }
        }
        return true;
    }
    else
    {
        return false;
    }
}

bool livelyRD::DycorsOptimizer::classifyPopulationElements() //O(n)
{
    if(!outputs_.empty() && !population_.empty())
    {
        std::vector<int> indicesToEliminate;
        for(std::size_t i; i < population_.size(); ++i)
        {
            if(!population_[i].constraintsHaveBeenChecked && !evaluateConstraints(population_[i]))
            {
                indicesToEliminate.push_back(i);
            }
        }
        for(const auto& index : indicesToEliminate)
        {
            population_[index] = population_.back();
            population_.pop_back();
        }

        selectBestOfIteration();
        return true;
    }
    else
    {
        return false;
    }
}

bool livelyRD::DycorsOptimizer::evaluateConstraints(const PopulationElement& element) //O(n)
{
    if(!element.solutionValues.empty())
    {
        for(const auto& outputInfo : outputs_)
        {
            bool isBigEnough = element.solutionValues.find(outputInfo.name)->second > outputInfo.lowerBound;
            bool isSmallEnough = element.solutionValues.find(outputInfo.name)->second < outputInfo.upperBound;
            if(!isBigEnough || !isSmallEnough)
            {
                return false;
            }
        }
        return true;
    }
    else
    {
        return false;
    }
}

bool livelyRD::DycorsOptimizer::selectBestOfIteration()
{
    if(population_.empty() || outputs_.empty())
    {
        return false;
    }
    for(const auto& elementToCompare : population_)
    {
        if(elementToCompare.isBestOfIteration)
        {
            for(auto& element : population_)
            {
                if(element.inputValues != elementToCompare.inputValues && element.isBestOfIteration)
                {
                    element.isBestOfIteration = !isFirstElementBehindSecond(element,elementToCompare);
                }
            }
        }
    }
    return true;
}

bool livelyRD::DycorsOptimizer::isFirstElementBehindSecond(
        const PopulationElement& firstElement, const PopulationElement& secondElement) //O(n)
{
    bool firstIsBehindSecond = true;
    for(const auto& outputInfo : outputs_)
    {
        if(outputInfo.isToBeOptimized && outputInfo.isToBeMaximized)
        {
            if(firstElement.solutionValues.find(outputInfo.name)->second >
                    secondElement.solutionValues.find(outputInfo.name)->second)
            {
                firstIsBehindSecond = false;
            }
        }
        else if(outputInfo.isToBeOptimized && !outputInfo.isToBeMaximized)
        {
            if(firstElement.solutionValues.find(outputInfo.name)->second <
               secondElement.solutionValues.find(outputInfo.name)->second)
            {
                firstIsBehindSecond = false;
            }
        }
    }
    return firstIsBehindSecond;
}

bool livelyRD::DycorsOptimizer::updateModel() //O(newPopulationPoints)
{
    Eigen::size_t oldMatrixRowsNumber = inputsMatrix_.rows();
    inputsMatrix_.conservativeResize(population_.size(), inputs_.size());
    outputsMatrix_.conservativeResize(population_.size(), outputs_.size());
    for(Eigen::size_t i = oldMatrixRowsNumber; i<population_.size(); ++i)
    {
        for(Eigen::size_t j = 0; j<inputs_.size(); ++j)
        {
            inputsMatrix_(i, j) = population_[i].inputValues[inputs_[j].name];
        }
        for(Eigen::size_t j = 0; j<outputs_.size(); ++j)
        {
            outputsMatrix_(i, j) = population_[i].solutionValues[outputs_[j].name];
        }
    }
    return model_.setModelData(inputsMatrix_, outputsMatrix_);
}

bool livelyRD::DycorsOptimizer::updateMutationProbability() //O(1)
{
    if(!population_.empty())
    {
        double maximumProbability = std::min(20.0/inputs_.size(), 1.0);
        mutationProbability_ = maximumProbability *
                               ( 1 - std::log(population_.size() - numberOfSamplePoints_ + 1) /
                                     std::log(numberOfEvaluations_ - numberOfSamplePoints_));
        return true;
    }
    return false;
}

bool livelyRD::DycorsOptimizer::generateTrialPoints() //O(n) n = trialPoints_.size()
{
    if(population_.empty())
    {
        return false;
    }
    trialPoints_.clear();
    selectOptimal();
    trialPoints_.resize(numberOfTrialPoints_ * optimalPoints_.size());
    for(std::size_t i = 0; i<optimalPoints_.size(); ++i)
    {
        for(std::size_t j = 0; j<numberOfTrialPoints_; ++j)
        {
            trialPoints_[i*numberOfTrialPoints_ + j].inputValues = optimalPoints_[i].inputValues;
        }
    }
    mutateTrialPoints();
    evaluateTrialPoints();
    return true;
}

bool livelyRD::DycorsOptimizer::mutateTrialPoints() //O(trialPoints_.size() * inputs_size())
{
    if(!trialPoints_.empty())
    {
        std::unordered_map<std::string, bool> isToBeMutated;
        bool isPointToBeMutated = false;
        for(auto& point : trialPoints_)
        {
            for(const auto& input : inputs_)
            {
                isToBeMutated[input.name] = (((double) std::rand() / (RAND_MAX)) <= mutationProbability_);
                if(isToBeMutated[input.name])
                {
                    isPointToBeMutated = true;
                }
            }
            if(!isPointToBeMutated)
            {
                int inputToMutate = std::rand() % inputs_.size();
                isToBeMutated[inputs_[inputToMutate].name] = true;
            }

            for(const auto& input : inputs_)
            {
                point.inputValues[input.name] +=
                        stepSize_ * isToBeMutated[input.name] * 2*(((double) std::rand() / (RAND_MAX)) - 0.5) *
                        point.inputValues[input.name];

                if(point.inputValues[input.name] < input.lowerBound)
                {
                    point.inputValues[input.name] = input.lowerBound;
                }
                else if(point.inputValues[input.name] > input.upperBound)
                {
                    point.inputValues[input.name] = input.upperBound;
                }
            }
        }
        return true;
    }
    return false;
}

bool livelyRD::DycorsOptimizer::evaluateTrialPoints() //O(trialPoints_.size() * inputs_size())
{
    if(trialPoints_.empty())
    {
        return false;
    }
    Eigen::MatrixXd inputsMatrix(trialPoints_.size(), trialPoints_[0].inputValues.size());
    for(Eigen::size_t i = 0; i < inputsMatrix.rows(); ++i)
    {
        for(Eigen::size_t j = 0; j < inputsMatrix.cols(); ++j)
        {
            inputsMatrix(i, j) = trialPoints_[i].inputValues[inputs_[j].name];
        }
    }
    Eigen::MatrixXd trialPointsResults = model_.evaluateModel(inputsMatrix);
    for(Eigen::size_t j = 0; j < trialPointsResults.cols(); ++j)
    {
        for(Eigen::size_t i = 0; i < trialPointsResults.rows(); ++i)
        {
            trialPoints_[i].solutionValues[outputs_[j].name] = trialPointsResults(i, j);
        }
        maximumTrialPointsOutputs_[outputs_[j].name] = trialPointsResults.col(j).maxCoeff();
        minimumTrialPointsOutputs_[outputs_[j].name] = trialPointsResults.col(j).minCoeff();
    }
    return true;
}

bool livelyRD::DycorsOptimizer::selectNextIterate() //O(n)
{
    calculateScores();
    double maximumScore = 0.0;
    int selectedTrialPointIndex = trialPoints_.size() + 100;
    for(std::size_t i = 0; i<trialPoints_.size(); ++i)
    {
        if(trialPointsScores_[i] > maximumScore)
        {
            maximumScore = trialPointsScores_[i];
            selectedTrialPointIndex = i;
        }
    }
    if(selectedTrialPointIndex > trialPoints_.size())
    {
        return false;
    }
    trialPoints_[selectedTrialPointIndex].solutionValues.clear();
    trialPoints_[selectedTrialPointIndex].isBestOfIteration = true;
    population_.push_back(trialPoints_[selectedTrialPointIndex]);
    return true;
}

bool livelyRD::DycorsOptimizer::calculateScores() //O(n)
{
    if(trialPoints_.empty())
    {
        return false;
    }
    caclulateRbfScores();
    calculateDistanceScores();
    if(trialPointsRbfScores_.empty() || trialPointsDistanceScores_.empty())
    {
        return false;
    }
    trialPointsScores_.clear();
    trialPointsScores_.resize(trialPoints_.size());
    for(std::size_t i = 0; i<trialPoints_.size(); ++i)
    {
        trialPointsScores_[i] = rbfWeight_ * trialPointsRbfScores_[i] +
                (1.0 - rbfWeight_) * trialPointsDistanceScores_[i];
    }
    return true;
}

bool livelyRD::DycorsOptimizer::caclulateRbfScores() //O(n)
{
    if(trialPoints_.empty())
    {
        return false;
    }
    trialPointsRbfScores_.clear();
    trialPointsRbfScores_.resize(trialPoints_.size());
    std::vector<std::unordered_map<std::string, double>> rbfPartialScores;
    rbfPartialScores.resize(trialPoints_.size());
    int numberOfValues = 0;
    for(std::size_t i = 0; i < trialPoints_.size(); ++i)
    {
        for (const auto &output : outputs_)
        {
            if (maximumTrialPointsOutputs_[output.name] == minimumTrialPointsOutputs_[output.name]
                && output.isToBeOptimized)
            {
                rbfPartialScores[i][output.name] = 1.0;
                ++numberOfValues;
            }
            else if(output.isToBeOptimized && output.isToBeMaximized)
            {
                rbfPartialScores[i][output.name] =
                        (trialPoints_[i].solutionValues[output.name] - minimumTrialPointsOutputs_[output.name]) /
                        (maximumTrialPointsOutputs_[output.name] - minimumTrialPointsOutputs_[output.name]);
                ++numberOfValues;
            }
            else if(output.isToBeOptimized && !output.isToBeMaximized)
            {
                rbfPartialScores[i][output.name] =
                        (maximumTrialPointsOutputs_[output.name] - trialPoints_[i].solutionValues[output.name]) /
                        (maximumTrialPointsOutputs_[output.name] - minimumTrialPointsOutputs_[output.name]);
                ++numberOfValues;
            }
            else
            {
                rbfPartialScores[i][output.name] = 0.0;
            }
        }
        double scoreSum = std::accumulate(rbfPartialScores[i].begin(), rbfPartialScores[i].end(), 0.0,
                [](const double& sum, const std::unordered_map<std::string, double>::value_type& current)
                {return sum + current.second;});
        trialPointsRbfScores_[i] = scoreSum / numberOfValues;
    }
    return true;
}

bool livelyRD::DycorsOptimizer::calculateDistanceScores()
{
    if(trialPoints_.empty())
    {
        return false;
    }
    trialPointsDistanceScores_.clear();
    trialPointsDistanceScores_.resize(trialPoints_.size());
    std::vector<double> minimumDistances;
    minimumDistances.resize(trialPoints_.size());
    for(std::size_t i = 0; i < trialPoints_.size(); ++i)
    {
        std::vector<double> distances;
        distances.resize(population_.size());
        for(std::size_t j = 0; j<population_.size(); ++j)
        {
            distances[j] = calculatePointsDistance(trialPoints_[i], population_[j]);
        }
        minimumDistances[i] = *std::min_element(distances.begin(), distances.end());
    }
    double absoluteMinimumDistance = *std::min_element(minimumDistances.begin(), minimumDistances.end());
    double absoluteMaximumDistance = *std::max_element(minimumDistances.begin(), minimumDistances.end());
    for(std::size_t i = 0; i < trialPoints_.size(); ++i)
    {
        trialPointsDistanceScores_[i] = (minimumDistances[i] - absoluteMinimumDistance) /
                                        (absoluteMaximumDistance - absoluteMinimumDistance);
    }
    return true;
}


double livelyRD::DycorsOptimizer::calculatePointsDistance(const PopulationElement& firstPoint,
                                                          const PopulationElement& secondPoint)
{
    double normSquared = 0.0;
    for(const auto& input : inputs_)
    {
        normSquared += std::pow(firstPoint.inputValues.find(input.name)->second -
                secondPoint.inputValues.find(input.name)->second, 2.0);
    }
    return std::sqrt(normSquared);
}

bool livelyRD::DycorsOptimizer::updateCounters()
{
    if(population_.back().isBestOfIteration)
    {
        successCounter_ += 1;
        failureCounter_ = 0;
    }
    else
    {
        successCounter_ = 0;
        failureCounter_ += 1;
    }
    return false;
}

bool livelyRD::DycorsOptimizer::adjustStepSize()
{
    if(successCounter_ >= consecutiveSuccessTolerance_)
    {
        stepSize_ = 2*stepSize_;
        successCounter_ = 0;
    }
    else if(failureCounter_ >= consecutiveFailTolerance_)
    {
        stepSize_ = stepSize_/2.0;
        failureCounter_ = 0;
    }
    return false;
}

const std::vector<livelyRD::DycorsOptimizer::PopulationElement> & livelyRD::DycorsOptimizer::getOptimalPoints()
{
    selectOptimal();
    return optimalPoints_;
}

bool livelyRD::DycorsOptimizer::selectOptimal()
{
    if(population_.empty())
    {
        return false;
    }
    optimalPoints_.clear();
    for(auto point : population_)
    {
        if(point.isBestOfIteration)
        {
            optimalPoints_.push_back(point);
        }
    }
    return true;
}

const std::vector<livelyRD::DycorsOptimizer::PopulationElement> &livelyRD::DycorsOptimizer::getAllPoints() const
{
    return population_;
}

std::vector<std::unordered_map<std::string, double>> livelyRD::DycorsOptimizer::latinHyperCube()
{
    std::vector<std::unordered_map<std::string, double>> results;
    std::vector<std::vector<double>> locationVectors;
    std::unordered_map<std::string, double> pickedValues;
    int indexPicked;

    locationVectors.resize(inputs_.size());
    for(int i = 0; i<locationVectors.size(); ++i)
    {
        locationVectors[i] = livelyRD::linSpace(numberOfSamplePoints_,inputs_[i].lowerBound,inputs_[i].upperBound);
    }
    while(results.size() < numberOfSamplePoints_)
    {
        srand(time(NULL));
        for(std::size_t i = 0; i < inputs_.size(); ++i)
        {
            indexPicked = rand()%locationVectors[i].size();
            pickedValues[inputs_[i].name] = locationVectors[i][indexPicked];
            locationVectors[i].erase(locationVectors[i].begin() + indexPicked);
        }
        results.push_back(pickedValues);
        pickedValues.clear();
    }
    return results;
}
