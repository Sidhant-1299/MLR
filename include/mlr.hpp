#pragma once
#include <Eigen/Dense>

class MLR
{
private:
    Eigen::MatrixXd coeffs;
    Eigen::MatrixXd X;
    Eigen::MatrixXd Y;
    Eigen::MatrixXd X_aug;

public:
    MLR(const Eigen::MatrixXd &, const Eigen::MatrixXd &);
    bool hasSufficientData() const;
    bool isCollinear();
    void fit();
    Eigen::MatrixXd predict(const Eigen::MatrixXd &) const;
    Eigen::MatrixXd getCoefficients() const;
    Eigen::MatrixXd getResiduals() const;
    double getRSS() const;
    double getTSS() const;
    double getR2() const;
    double getAdjustedR2() const;
    double getMSE() const;
    double getMAE() const;
    double Ftest() const;
};