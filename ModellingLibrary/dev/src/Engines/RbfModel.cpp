#include "RbfModel.h"

livelyRD::RbfModel::RbfModel() : modelIsInitialized_(false)
{

}

bool livelyRD::RbfModel::setModelData(const Eigen::MatrixXd &populationInputs,
                                 const Eigen::MatrixXd &populationOutputs)
{
    populationInputs_ = populationInputs;
    populationOutputs_ = populationOutputs;
    modelIsInitialized_ = calculateRbf();
    return modelIsInitialized_;
}

bool livelyRD::RbfModel::calculateRbf()
{
    Eigen::MatrixXd distances = -2 * populationInputs_ * populationInputs_.transpose();
    Eigen::MatrixXd squaredNormColumn = populationInputs_.rowwise().squaredNorm();
    Eigen::MatrixXd normColumns(distances.rows(), distances.cols());
    for(Eigen::size_t i = 0; i<normColumns.cols(); ++i)
    {
        normColumns.col(i) = squaredNormColumn;
    }
    distances += normColumns + normColumns.transpose();
    Eigen::MatrixXd rbfCoefficients = distances.cwiseAbs().cwiseSqrt().array().pow(3.0);

    Eigen::MatrixXd ones = Eigen::MatrixXd::Ones(populationInputs_.rows(), 1);
    Eigen::MatrixXd polynomialCoefficients(ones.rows(), ones.cols() + populationInputs_.cols());
    polynomialCoefficients << ones, populationInputs_;

    Eigen::MatrixXd zeros = Eigen::MatrixXd::Zero(populationInputs_.cols()+1, populationInputs_.cols()+1);
    Eigen::MatrixXd equationCoefficients(rbfCoefficients.rows() + polynomialCoefficients.cols(),
                                          rbfCoefficients.cols() + polynomialCoefficients.cols());
    equationCoefficients << rbfCoefficients, polynomialCoefficients, polynomialCoefficients.transpose(), zeros;

    Eigen::MatrixXd equationSolutions = Eigen::MatrixXd::Zero(
            populationInputs_.rows() + populationInputs_.cols() + 1, populationOutputs_.cols());
    equationSolutions.block(0, 0,
            populationOutputs_.rows(), populationOutputs_.cols()) = populationOutputs_;

    model_.resize(0,0);
    model_.resize(equationSolutions.rows(), equationSolutions.cols());
    for(Eigen::size_t i = 0; i<equationSolutions.cols(); ++i)
    {
        model_.col(i) = equationCoefficients.colPivHouseholderQr().solve(equationSolutions.col(i));
    }
    return true;
}

bool livelyRD::RbfModel::isModelInitialized() const
{
    return modelIsInitialized_;
}

Eigen::MatrixXd livelyRD::RbfModel::evaluateModel(const Eigen::MatrixXd &pointsToEvaluate) const
{
    Eigen::MatrixXd distances = -2 * populationInputs_ * pointsToEvaluate.transpose();
    Eigen::MatrixXd squaredNormColumn = pointsToEvaluate.rowwise().squaredNorm();
    for(Eigen::size_t i = 0; i<distances.rows(); ++i)
    {
        distances.row(i) += squaredNormColumn.transpose();
    }
    squaredNormColumn = populationInputs_.rowwise().squaredNorm();
    for(Eigen::size_t i = 0; i<distances.cols(); ++i)
    {
        distances.col(i) += squaredNormColumn;
    }
    Eigen::MatrixXd rbfCoefficients = distances.transpose().cwiseAbs().cwiseSqrt().array().pow(3.0);

    Eigen::MatrixXd ones = Eigen::MatrixXd::Ones(pointsToEvaluate.rows(), 1);
    Eigen::MatrixXd polynomialCoefficients(ones.rows(), ones.cols() + populationInputs_.cols());
    polynomialCoefficients << ones, pointsToEvaluate;

    Eigen::MatrixXd equationCoefficients(rbfCoefficients.rows(),
                                         rbfCoefficients.cols() + polynomialCoefficients.cols());
    equationCoefficients << rbfCoefficients, polynomialCoefficients;

    Eigen::MatrixXd solution(pointsToEvaluate.rows(), model_.cols());
    for(Eigen::size_t i = 0; i<model_.cols(); ++i)
    {
        solution.col(i) = equationCoefficients * model_.col(i);
    }
    return solution;
}
