#include <iostream>
#include <Eigen/Dense>
#include "mlr.hpp"

using Eigen::MatrixXd;
using Eigen::VectorXd;

void MLR::fit(const MatrixXd &X, const MatrixXd &Y)
{
    // XTXB  = XTy
    coeffs = (X.transpose() * X).ldlt().solve(X.transpose() * Y);
    // eigen uses ldlt to solve the system
}

VectorXd MLR::predict(const MatrixXd &X) const
{
    return X * coeffs;
}

MatrixXd MLR::coefficients() const
{
    return coeffs;
}