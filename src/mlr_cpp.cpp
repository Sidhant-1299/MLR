#include <iostream>
#include <Eigen/Dense>
#include "mlr.hpp"

using Eigen::FullPivLU;
using Eigen::MatrixXd;
using Eigen::VectorXd;

MLR::MLR(const Eigen::MatrixXd &X, const Eigen::MatrixXd &Y) : X(X), Y(Y), X_aug(X.rows(), X.cols() + 1)
{
    // Add column of 1s for intercept (b0)
    X_aug << MatrixXd::Ones(X.rows(), 1), X;
};

bool MLR::hasSufficientData() const
{
    // results in divide by zero error for Ftest
    // Also n == k+1 means that the training data is statistically meaningless
    // overfits the data
    return X_aug.rows() > X_aug.cols(); // i.e., n > k + 1
}

bool MLR::isCollinear()
{
    // checks if the columns of the matrix are collinear
    //  ax1 = bx2 + c
    //  In that case the augemented matrix with column 1 with ones
    //  will become a linearly dependent system and there will be no
    //  unique solution for the equation XTXb = XTy

    FullPivLU<MatrixXd> lu_decomposed_Xaug(X_aug);
    return (lu_decomposed_Xaug.rank() < X_aug.cols());
}

void MLR::fit()
{
    // debugging
    // std::cout << "C++ fit\n";
    // std::cout << "X_aug:\n"
    //           << X_aug << "\n";
    // std::cout << "Y:\n"
    //           << Y << "\n";

    // Fit: coeffs = (X^T X)^-1 X^T y
    // XTXb = XTY

    // coeffs = (X_aug.transpose() * X_aug).ldlt().solve(X_aug.transpose() * Y);
    coeffs = X_aug.householderQr().solve(Y);
    // std::cout << "coeffs: " << coeffs << std::endl;
}

MatrixXd MLR::predict(const MatrixXd &X) const
{
    // makes predicition ie returns y_pred for a matrix X
    // using the equation y_n = b0 + (summation(i=1;i<=k) b_k*x_n
    // where k = number of predictors
    // n is the nth observation
    MatrixXd X_new(X.rows(), X.cols() + 1);
    X_new << MatrixXd::Ones(X.rows(), 1), X;
    return X_new * coeffs;
}

MatrixXd MLR::getCoefficients() const
{
    // returns the coefficients after the linear regression model is fit
    return coeffs;
}

MatrixXd MLR::getResiduals() const
{
    return Y - X_aug * coeffs;
}

double MLR::getRSS() const
{
    // returns the residual sum of squares
    MatrixXd e = getResiduals();
    return (e.transpose() * e)(0, 0); // extract the scalar value from the 1x1 matrix
}

double MLR::getTSS() const
{
    MatrixXd y_bar = MatrixXd::Constant(Y.rows(), 1, Y.mean()); // Y is a nx1 matrix where n is the number of observation
    return ((Y - y_bar).transpose() * (Y - y_bar))(0, 0);
}

double MLR::getR2() const
{
    double RSS = getRSS();
    double TSS = getTSS();
    return 1 - (RSS / TSS);
}

double MLR::getAdjustedR2() const
{
    int n = X_aug.rows();     // observations
    int p = X_aug.cols() - 1; // exclude intercept
    double r2 = getR2();
    return 1 - (1 - r2) * ((n - 1.0) / (n - p - 1.0));
}

double MLR::getMSE() const
{
    double rss = getRSS();
    int n = X_aug.rows();
    int p = X_aug.cols(); // includes intercept
    return rss / (n - p);
}

double MLR::getMAE() const
{
    MatrixXd residuals = getResiduals();
    return residuals.cwiseAbs().mean();
}

double MLR::Ftest() const
{
    double RSS = getRSS();
    double TSS = getTSS();
    int k = X.cols(); // number of predictors
    int n = X.rows(); // number of observations
    return ((TSS - RSS) / k) / (RSS / (n - k - 1));
}

VectorXd MLR::getTStatistics() const
{

    double mse = getMSE();

    // (X^T X)^-1
    MatrixXd XTX_inv = (X_aug.transpose() * X_aug).inverse();

    // Standard errors = sqrt(MSE * diag(X^T X)^-1)
    // .diagonal() returns the values of the main diagonal
    VectorXd se = XTX_inv.diagonal(); // returns a vector of shape p x 1
    for (int i = 0; i < se.size(); ++i)
    {
        se(i) = std::sqrt(mse * se(i));
    }

    // Compute t-statistics for each coefficient:
    // t_i = B_i / SE(B_i)
    // This measures how many standard errors the coefficient is away from 0.
    // Used to test H0: B_i = 0 (null hypothesis).
    VectorXd t_stats = coeffs.array() / se.array();
    return t_stats;
}