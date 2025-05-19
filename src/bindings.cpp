// What Happens in bindings.cpp
//  This file uses Pybind11 to tell Python:
//  "Here's a class called MLR"
//  "It has a method called fit() that takes a matrix and a vector"

#include <pybind11/pybind11.h>
// numpy ndarray to Eigen::matrix
#include <pybind11/eigen.h>
#include "mlr.hpp"

namespace py = pybind11;

PYBIND11_MODULE(mlr_cpp, m)
{
    m.doc() = "Fit the linear regression model using python bindings to C++";
    // <MLR> is our class in cpp
    // "MLR" is our class in python
    py::class_<MLR>(m, "MLR")
        .def(py::init<const Eigen::MatrixXd &, const Eigen::MatrixXd &>())
        .def("isCollinear", &MLR::isCollinear, "Test for collinear data in X data")
        .def("hasSufficentData", &MLR::hasSufficientData, "Check if the model has sufficient data")
        .def("fit", &MLR::fit, "Fit the model")                                 // py:arg allows for keyword argument in python
        .def("predict", &MLR::predict, py::arg("X"), "Predict target variable") // The string is returned using help(predict)
        .def("getCoefficients", &MLR::getCoefficients, "Get the model coeff")
        .def("getResiduals", &MLR::getResiduals, "Get model residuals")
        .def("getRSS", &MLR::getRSS, "Get the residual sum of squares")
        .def("getTSS", &MLR::getTSS, "Get the total sum of squares")
        .def("getR2", &MLR::getR2, "Get R squared of the model")
        .def("getAdjustedR2", &MLR::getAdjustedR2, "Get Adjusted R2 of the model")
        .def("getMAE", &MLR::getMAE, "Get Mean absolute error")
        .def("getMSE", &MLR::getMSE, "Get Mean Squared Error")
        .def("Ftest", &MLR::Ftest, "FTest for model significance")
        .def("getTStatistics", &MLR::getTStatistics, "Get t statistics of regressors")
        .def("getPValues", &MLR::getPValues, "Get PValues of the regressor's signifiance");
}