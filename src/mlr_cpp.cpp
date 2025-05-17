#include <iostream>
#include <Eigen/Dense>
#include "mlr.hpp"

using Eigen::MatrixXd;
using Eigen::VectorXd;

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
    coeffs = (X_aug.transpose() * X_aug).ldlt().solve(X_aug.transpose() * Y);
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

int main()
{
    MatrixXd X(3, 2); // 3 samples, 2 features
    X << 1, 0,
        3, 4,
        5, 6;
    MatrixXd Y(3, 1); // y = 5 + 1·x1 + 2·x2
    Y << 6, 16, 22;

    MLR model;
    model.fit(X, Y);
    std::cout << "Coefficients:\n";
    std::cout << model.coefficients() << "\n";
    std::cout << model.coefficients().rows() << "\n";
    std::cout << model.coefficients().cols() << "\n";
}