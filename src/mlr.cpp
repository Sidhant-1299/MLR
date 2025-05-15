#include <iostream>
#include <Eigen/Dense>

int main()
{
    Eigen::Matrix<float, 3, 3> matrixA;
    matrixA.setZero();
    for (int i = 0; i < 3; i++)
    {
        matrixA(i, i) = i + 1;
    }
    std::cout << matrixA << std::endl;
    return 0;
}
