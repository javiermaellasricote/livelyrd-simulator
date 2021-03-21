#ifndef RBFMODEL_H
#define RBFMODEL_H

#include <Eigen/Dense>

namespace livelyRD
{
    class RbfModel
    {
    public:
        RbfModel();
        ~RbfModel() = default;

        bool setModelData(const Eigen::MatrixXd& populationInputs,
                        const Eigen::MatrixXd& populationOutputs);
        [[nodiscard]] bool isModelInitialized() const;
        [[nodiscard]] Eigen::MatrixXd evaluateModel(const Eigen::MatrixXd& pointsToEvaluate) const;


    private:
        bool calculateRbf();

        bool modelIsInitialized_;
        Eigen::MatrixXd populationInputs_;
        Eigen::MatrixXd populationOutputs_;
        Eigen::MatrixXd model_;
    };
}

#endif
