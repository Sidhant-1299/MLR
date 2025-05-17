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
        .def(py::init<>())
        .def("is_collinear", &MLR::is_collinear, py::arg("X"), "Test for collinear data in X data")
        .def("fit", &MLR::fit, py::arg("X"), py::arg("Y"), "Fit the model")     // py:arg allows for keyword argument in python
        .def("predict", &MLR::predict, py::arg("X"), "Predict target variable") // The string is returned using help(predict)
        .def("coefficients", &MLR::coefficients, "Get the model coeff");
}