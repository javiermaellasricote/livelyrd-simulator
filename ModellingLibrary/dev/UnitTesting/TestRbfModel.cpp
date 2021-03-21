#include "../thirdPartyLibraries/catch2/catch.hpp"
#include <iostream>
#include <string>
#include <unordered_map>
#include <Eigen/Dense>
#include "../src/Engines/RbfModel.h"

#define NAME_OBJECTIVE_SPHERE_FUNCTION "Sphere Objective"
#define MARGIN_OF_ERROR_FOR_DOUBLE 0.00001

namespace TestRbfModel
{
    std::unordered_map<std::string, double> sphereTestFunction(const std::unordered_map<std::string, double>& inputs)
    {
        double sum = 0.0;
        for(const auto& element : inputs)
        {
            sum += element.second*element.second;
        }
        std::unordered_map<std::string, double> solution;
        solution.insert({NAME_OBJECTIVE_SPHERE_FUNCTION, sum});
        return solution;
    }
}



SCENARIO("Model a Sphere function using RbfModel")
{
    GIVEN("A list of points evaluated with the Sphere function")
    {
        Eigen::MatrixXd pointsInputs(5, 3);
        pointsInputs << 3, 2, 5,
                        6, 2, 7,
                        3, 1, 7,
                        8, 9, 3,
                        1, 0, 9;
        Eigen::MatrixXd pointsOutputs(5, 1);

        std::unordered_map<std::string, double> inputs;
        std::unordered_map<std::string, double> outputs;
        for(Eigen::size_t i = 0; i<pointsInputs.rows(); ++i)
        {
            inputs.clear();
            outputs.clear();
            for(Eigen::size_t j = 0; j<pointsInputs.cols(); ++j)
            {
                inputs[std::to_string(j)] = pointsInputs(i, j);
            }
            outputs = TestRbfModel::sphereTestFunction(inputs);
            pointsOutputs(i, 0) = outputs[NAME_OBJECTIVE_SPHERE_FUNCTION];
        }

        AND_GIVEN("An RbfModel initialized with the latter points")
        {
            livelyRD::RbfModel rbfModel;
            REQUIRE(rbfModel.setModelData(pointsInputs, pointsOutputs));
            REQUIRE(rbfModel.isModelInitialized());

            THEN("Some evaluation points should get results from the model")
            {
                Eigen::MatrixXd inputsToEvaluate(3, 3);
                inputsToEvaluate << 3, 2, 5,
                                    2, 3, 4,
                                    8, 6, 4;
                Eigen::MatrixXd evaluationOutputs;
                evaluationOutputs = rbfModel.evaluateModel(inputsToEvaluate);
                REQUIRE(evaluationOutputs.rows() == 3);
                REQUIRE(evaluationOutputs.cols() == 1);
                REQUIRE(evaluationOutputs(0, 0) < pointsOutputs(0, 0) + MARGIN_OF_ERROR_FOR_DOUBLE);
                REQUIRE(evaluationOutputs(0, 0) > pointsOutputs(0, 0) - MARGIN_OF_ERROR_FOR_DOUBLE);
                /*std::cout << evaluationOutputs << std::endl;
                std::cout << "-------" << std::endl;
                Eigen::MatrixXd realEvaluationOutputs(3, 1);
                for(Eigen::size_t i = 0; i<inputsToEvaluate.rows(); ++i)
                {
                    inputs.clear();
                    outputs.clear();
                    for(Eigen::size_t j = 0; j<inputsToEvaluate.cols(); ++j)
                    {
                        inputs[std::to_string(j)] = inputsToEvaluate(i, j);
                    }
                    outputs = TestRbfModel::sphereTestFunction(inputs);
                    realEvaluationOutputs(i, 0) = outputs[NAME_OBJECTIVE_SPHERE_FUNCTION];
                }
                std::cout << realEvaluationOutputs << std::endl;*/
            }
        }
    }
}
