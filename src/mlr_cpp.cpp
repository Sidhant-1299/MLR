#include <iostream>
#include <Eigen/Dense>
#include "mlr.hpp"

using Eigen::FullPivLU;
using Eigen::MatrixXd;

bool MLR::test_collinearity(const MatrixXd &X)
{
    MatrixXd X_aug(X.rows(), X.cols() + 1);
    X_aug << MatrixXd::Ones(X.rows(), 1), X;
    FullPivLU<MatrixXd> lu_decomposed_Xaug(X_aug);
    std::cout << X_aug << std::endl;
    return (lu_decomposed_Xaug.rank() == X_aug.cols());
}

void MLR::fit(const MatrixXd &X, const MatrixXd &Y)
{

    // Add column of 1s for intercept
    MatrixXd X_aug(X.rows(), X.cols() + 1);
    X_aug << MatrixXd::Ones(X.rows(), 1), X;

    std::cout << "C++ fit\n";
    std::cout << "X_aug:\n"
              << X_aug << "\n";
    std::cout << "Y:\n"
              << Y << "\n";

    // Fit: coeffs = (X^T X)^-1 X^T y
    // XTXb = XTY
    coeffs = (X_aug.transpose() * X_aug).ldlt().solve(X_aug.transpose() * Y);
    std::cout << "coeffs: " << coeffs << std::endl;
}

MatrixXd MLR::predict(const MatrixXd &X) const
{
    MatrixXd X_aug(X.rows(), X.cols() + 1);
    X_aug << MatrixXd::Ones(X.rows(), 1), X;
    return X_aug * coeffs;
}

MatrixXd MLR::coefficients() const
{
    return coeffs;
}
