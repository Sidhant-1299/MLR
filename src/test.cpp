#include <iostream>
#include <Eigen/Dense>

int main()
{
    Eigen::VectorXd v(5);
    v << 1, 4, 9, 16, 25;
    v = v.array().sqrt();
    std::cout << v;
}