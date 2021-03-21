#include <vector>
#include <string>
#include <cmath>
#include <fstream>
#include "../thirdPartyLibraries/catch2/catch.hpp"
#include "../src/Engines/DycorsOptimizer.h"
#include "DefinitionsTestDycorsOptimizer.h"

#define MARGIN_OF_ERROR_FOR_DOUBLE 0.00001

namespace TestDycorsOptimizer
{
    std::unordered_map<std::string, double> sphereTestFunction(const std::unordered_map<std::string, double>& inputs)
    {
        double sum = 0.0;
        for(const auto& element : inputs)
        {
            sum += element.second*element.second;
        }
        std::unordered_map<std::string, double> solution;
        solution.insert({TEST_DYCORS_SPHERE_FUNCTION_OBJECTIVE_NAME, sum});
        return solution;
    }

    std::unordered_map<std::string, double> zdt1TestFunction(const std::unordered_map<std::string, double>& inputs)
    {
        std::vector<std::string> inputNames = TEST_DYCORS_ZDT1_FUNCTION_INPUT_NAMES;

        std::unordered_map<std::string, double> solution;
        solution.insert({TEST_DYCORS_ZDT1_FUNCTION_OBJECTIVE1_NAME, inputs.find(inputNames[0])->second});

        double g = 0.0;
        for(const auto& input : inputs)
        {
            g += input.second;
        }
        g = 1 + (9.0 / 29.0) * g;
        double h = 1 - std::sqrt(inputs.find(inputNames[0])->second / g);
        double f2 = g * h;
        solution.insert({TEST_DYCORS_ZDT1_FUNCTION_OBJECTIVE2_NAME, f2});
        return solution;
    }

    std::unordered_map<std::string, double> moneyVsTimeTestFunction(
            const std::unordered_map<std::string, double>& inputs)
    {
        double totalKnowledge = MONEY_VS_TIME_FUNCTION_INITIAL_KNOWLEDGE;
        double totalMoney = MONEY_VS_TIME_FUNCTION_INITIAL_MONEY;
        double knowledgeWeight = MONEY_VS_TIME_FUNCTION_KNOWLEDGE_WEIGHT;
        std::string timePrefix = MONEY_VS_TIME_FUNCTION_INPUT_NAME_PREFIX1;
        std::string knowledgePrefix = MONEY_VS_TIME_FUNCTION_INPUT_NAME_PREFIX2;


        std::unordered_map<std::string, double> solutions;
        double totalTime = 0.0;
        for(std::size_t i = 0; i<inputs.size()/2.0; ++i)
        {
            totalTime += inputs.find(timePrefix + std::to_string(i))->second;
            totalKnowledge += inputs.find(knowledgePrefix + std::to_string(i))->second;
            totalMoney += totalMoney * (1 - std::exp(- totalKnowledge/knowledgeWeight)) *
                          (inputs.find(timePrefix + std::to_string(i))->second -
                           inputs.find(knowledgePrefix + std::to_string(i))->second);
        }
        solutions[MONEY_VS_TIME_FUNCTION_OBJECTIVE1_NAME] = totalTime;
        solutions[MONEY_VS_TIME_FUNCTION_OBJECTIVE2_NAME] = totalMoney;
        return solutions;
    }

    void writeSolutionToCsv(const std::string& filePath,
                             const  std::vector<livelyRD::DycorsOptimizer::PopulationElement>& solution)
    {
        std::ofstream file;
        file.open(filePath);
        for(const auto& input : solution.front().inputValues)
        {
            file << input.first << ",";
        }
        for(const auto& output : solution.front().solutionValues)
        {
            file << output.first << ",";
        }
        file << std::endl;
        for(const auto& point : solution)
        {
            for(const auto& input : point.inputValues)
            {
                file << input.second << ",";
            }
            for(const auto& output : point.solutionValues)
            {
                file << output.second << ",";
            }
            file << std::endl;
        }
    }
}


SCENARIO("Optimize a single-objective case")
{
    GIVEN("A spherical function with two inputs that must be minimized")
    {
        std::vector<std::string> inputNames = TEST_DYCORS_SPHERE_FUNCTION_INPUT_NAMES;
        livelyRD::DycorsOptimizer::InputInfo input1;
        input1.name = inputNames[0];
        input1.lowerBound = -1.0;
        input1.upperBound = 1.0;
        livelyRD::DycorsOptimizer::InputInfo input2;
        input2 = input1;
        input2.name = inputNames[1];
        std::vector<livelyRD::DycorsOptimizer::InputInfo> inputs = {input1, input2};

        livelyRD::DycorsOptimizer::OutputInfo output;
        output.name = TEST_DYCORS_SPHERE_FUNCTION_OBJECTIVE_NAME;
        output.isToBeOptimized = true;
        output.isToBeMaximized = false;
        output.isConstraint = false;
        std::vector outputs(1, output);

        std::function<std::unordered_map<std::string, double>
                (const std::unordered_map<std::string, double>&)> blackBox = TestDycorsOptimizer::sphereTestFunction;
        livelyRD::DycorsOptimizer dycorsOptimizer(blackBox);
        dycorsOptimizer.setNumberOfEvaluations(TEST_DYCORS_SPHERE_FUNCTION_NUMBER_OF_EVALUATIONS);
        dycorsOptimizer.setInitialStepSize(TEST_DYCORS_INITIAL_STEP_SIZE);
        dycorsOptimizer.setMinimumStepSize(TEST_DYCORS_MIN_STEP_SIZE);
        dycorsOptimizer.setNumberOfTrialPoints(TEST_DYCORS_NUMBER_OF_NEIGHBOURS);
        dycorsOptimizer.setConsecutiveFailTolerance(TEST_DYCORS_CONSECUTIVE_FAIL_TOLERANCE);
        dycorsOptimizer.setConsecutiveSuccessTolerance(TEST_DYCORS_CONSECUTIVE_SUCCESS_TOLERANCE);
        dycorsOptimizer.setRbfWeight(TEST_DYCORS_RBF_WEIGHT);
        dycorsOptimizer.setInputs(inputs);
        dycorsOptimizer.setOutputs(outputs);

        WHEN("The Dycors Optimizer runs")
        {
            REQUIRE(dycorsOptimizer.run());

            THEN("The optimum solution should have been found")
            {
                std::vector<livelyRD::DycorsOptimizer::PopulationElement> solution;
                solution = dycorsOptimizer.getOptimalPoints();
                REQUIRE(solution.size() == 1);
                REQUIRE(solution[0].inputValues[inputNames[0]] < MARGIN_OF_ERROR_FOR_DOUBLE);
                REQUIRE(solution[0].inputValues[inputNames[0]] > - MARGIN_OF_ERROR_FOR_DOUBLE);
                REQUIRE(solution[0].inputValues[inputNames[1]] < MARGIN_OF_ERROR_FOR_DOUBLE);
                REQUIRE(solution[0].inputValues[inputNames[1]] > - MARGIN_OF_ERROR_FOR_DOUBLE);
            }
        }
    }
}

SCENARIO("Optimize ZDT1 multi-objective case")
{
    GIVEN("The ZDT1 test function with 30 inputs and 2 outputs to minimize")
    {
        std::vector<livelyRD::DycorsOptimizer::InputInfo> inputs;
        std::vector<std::string> inputNames = TEST_DYCORS_ZDT1_FUNCTION_INPUT_NAMES;
        inputs.resize(inputNames.size());
        livelyRD::DycorsOptimizer::InputInfo inputInfo;
        inputInfo.upperBound = 1.0;
        inputInfo.lowerBound = 0.0;
        for(std::size_t i = 0; i<inputNames.size(); ++i)
        {
            inputInfo.name = inputNames[i];
            inputs[i] = inputInfo;
        }

        std::vector<livelyRD::DycorsOptimizer::OutputInfo> outputs;
        outputs.resize(2);
        outputs[0].name = TEST_DYCORS_ZDT1_FUNCTION_OBJECTIVE1_NAME;
        outputs[0].isToBeOptimized = true;
        outputs[0].isToBeMaximized = false;
        outputs[0].isConstraint = false;
        outputs[1].name = TEST_DYCORS_ZDT1_FUNCTION_OBJECTIVE2_NAME;
        outputs[1].isToBeOptimized = true;
        outputs[1].isToBeMaximized = false;
        outputs[1].isConstraint = false;

        std::function<std::unordered_map<std::string, double>
                (const std::unordered_map<std::string, double>&)> blackBox = TestDycorsOptimizer::zdt1TestFunction;
        livelyRD::DycorsOptimizer dycorsOptimizer(blackBox);
        dycorsOptimizer.setNumberOfEvaluations(TEST_DYCORS_ZDT1_FUNCTION_NUMBER_OF_EVALUATIONS);
        dycorsOptimizer.setInitialStepSize(TEST_DYCORS_INITIAL_STEP_SIZE);
        dycorsOptimizer.setMinimumStepSize(TEST_DYCORS_MIN_STEP_SIZE);
        dycorsOptimizer.setNumberOfTrialPoints(TEST_DYCORS_NUMBER_OF_NEIGHBOURS);
        dycorsOptimizer.setConsecutiveFailTolerance(TEST_DYCORS_CONSECUTIVE_FAIL_TOLERANCE);
        dycorsOptimizer.setConsecutiveSuccessTolerance(TEST_DYCORS_CONSECUTIVE_SUCCESS_TOLERANCE);
        dycorsOptimizer.setRbfWeight(TEST_DYCORS_RBF_WEIGHT);
        dycorsOptimizer.setInputs(inputs);
        dycorsOptimizer.setOutputs(outputs);

        WHEN("The Dycors Optimizer runs")
        {
            REQUIRE(dycorsOptimizer.run());

            THEN("The optimum solution should have been found")
            {
                std::vector<livelyRD::DycorsOptimizer::PopulationElement> solution;
                solution = dycorsOptimizer.getOptimalPoints();
                REQUIRE(!solution.empty());
                TestDycorsOptimizer::writeSolutionToCsv(TEST_DYCORS_ZDT1_FUNCTION_PATH_TO_SOLUTION, solution);
            }
        }
    }
}

SCENARIO("Money Vs Time case")
{
    GIVEN("The moneyVsTime test function with 20 inputs and 2 outputs")
    {
        std::vector<livelyRD::DycorsOptimizer::InputInfo> inputs;
        inputs.resize(MONEY_VS_TIME_FUNCTION_MAX_NUMBER_OF_TIME_STEPS * 2);
        for(std::size_t i = 0; i<inputs.size()/2.0; ++i)
        {
            inputs[i].name = MONEY_VS_TIME_FUNCTION_INPUT_NAME_PREFIX1 + std::to_string(i);
            inputs[i].lowerBound = 0.0;
            inputs[i].upperBound = 1.0;

            int j = inputs.size()/2 + i;
            inputs[j].name = MONEY_VS_TIME_FUNCTION_INPUT_NAME_PREFIX2 + std::to_string(i);
            inputs[j].lowerBound = 0.0;
            inputs[j].upperBound = 1.0;
        }

        std::vector<livelyRD::DycorsOptimizer::OutputInfo> outputs;
        outputs.resize(2);
        outputs[0].name = MONEY_VS_TIME_FUNCTION_OBJECTIVE1_NAME;
        outputs[0].isToBeOptimized = true;
        outputs[0].isToBeMaximized = false;
        outputs[0].isConstraint = false;
        outputs[1].name = MONEY_VS_TIME_FUNCTION_OBJECTIVE2_NAME;
        outputs[1].isToBeOptimized = true;
        outputs[1].isToBeMaximized = true;
        outputs[1].isConstraint = false;

        std::function<std::unordered_map<std::string, double>
                (const std::unordered_map<std::string, double>&)> blackBox =
                        TestDycorsOptimizer::moneyVsTimeTestFunction;
        livelyRD::DycorsOptimizer dycorsOptimizer(blackBox);
        dycorsOptimizer.setNumberOfEvaluations(MONEY_VS_TIME_FUNCTION_NUMBER_OF_EVALUATIONS);
        dycorsOptimizer.setInitialStepSize(TEST_DYCORS_INITIAL_STEP_SIZE);
        dycorsOptimizer.setMinimumStepSize(TEST_DYCORS_MIN_STEP_SIZE);
        dycorsOptimizer.setNumberOfTrialPoints(TEST_DYCORS_NUMBER_OF_NEIGHBOURS);
        dycorsOptimizer.setConsecutiveFailTolerance(TEST_DYCORS_CONSECUTIVE_FAIL_TOLERANCE);
        dycorsOptimizer.setConsecutiveSuccessTolerance(TEST_DYCORS_CONSECUTIVE_SUCCESS_TOLERANCE);
        dycorsOptimizer.setRbfWeight(TEST_DYCORS_RBF_WEIGHT);
        dycorsOptimizer.setInputs(inputs);
        dycorsOptimizer.setOutputs(outputs);

        WHEN("The Dycors Optimizer runs")
        {
            REQUIRE(dycorsOptimizer.run());

            THEN("The optimum solution should have been found")
            {
                std::vector<livelyRD::DycorsOptimizer::PopulationElement> solution;
                solution = dycorsOptimizer.getOptimalPoints();
                REQUIRE(!solution.empty());
                TestDycorsOptimizer::writeSolutionToCsv(MONEY_VS_TIME_FUNCTION_PATH_TO_SOLUTION, solution);
            }
        }
    }
}