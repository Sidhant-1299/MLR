#pragma once
#include <Eigen/Dense>

class MLR
{
private:
    Eigen::MatrixXd coeffs;

public:
    void fit(const Eigen::MatrixXd &, const Eigen::MatrixXd &);
    Eigen::MatrixXd predict(const Eigen::MatrixXd &) const;
    Eigen::MatrixXd coefficients() const;
};