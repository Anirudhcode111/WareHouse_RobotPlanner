
#include "algorithms/divide_conquer/Strassen.hpp"

#include <cassert>
#include <iostream>
#include <stdexcept>

using namespace warehouse::algorithms;

int main() {
    // 1x1 matrix
    Matrix A1 = {{5}};
    Matrix B1 = {{4}};

    assert(strassenMatrixMultiplication(A1, B1) ==
           classicalMatrixMultiplication(A1, B1));

    // 2x2 matrix
    Matrix A2 = {
        {1, 2},
        {3, 4}
    };

    Matrix B2 = {
        {5, 6},
        {7, 8}
    };

    assert(strassenMatrixMultiplication(A2, B2) ==
           classicalMatrixMultiplication(A2, B2));

    // 3x3 matrix (requires padding)
    Matrix A3 = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    Matrix B3 = {
        {9, 8, 7},
        {6, 5, 4},
        {3, 2, 1}
    };

    assert(strassenMatrixMultiplication(A3, B3) ==
           classicalMatrixMultiplication(A3, B3));

    // 4x4 matrix
    Matrix A4 = {
        {1, 2, 0, 1},
        {0, 1, 3, 2},
        {2, 0, 1, 1},
        {1, 1, 1, 0}
    };

    Matrix B4 = {
        {2, 1, 0, 1},
        {1, 0, 2, 1},
        {0, 1, 1, 2},
        {1, 2, 0, 1}
    };

    assert(strassenMatrixMultiplication(A4, B4) ==
           classicalMatrixMultiplication(A4, B4));

    // Invalid non-square matrix
    bool exceptionThrown = false;

    try {
        Matrix invalid = {
            {1, 2, 3},
            {4, 5, 6}
        };

        strassenMatrixMultiplication(invalid, invalid);
    } catch (const std::invalid_argument&) {
        exceptionThrown = true;
    }

    assert(exceptionThrown);

    // Different matrix dimensions
    exceptionThrown = false;

    try {
        Matrix X = {{1, 2}, {3, 4}};
        Matrix Y = {{1}};

        strassenMatrixMultiplication(X, Y);
    } catch (const std::invalid_argument&) {
        exceptionThrown = true;
    }

    assert(exceptionThrown);

    std::cout << "All Strassen Matrix Multiplication tests passed!\n";

    return 0;
}
