
#include "algorithms/divide_conquer/MatrixMultiplication.hpp"

#include <cassert>
#include <iostream>
#include <stdexcept>

using namespace warehouse::algorithms;

int main() {
    // Normal 2x2 multiplication
    Matrix A = {
        {1, 2},
        {3, 4}
    };

    Matrix B = {
        {5, 6},
        {7, 8}
    };

    Matrix expected = {
        {19, 22},
        {43, 50}
    };

    assert(classicalMatrixMultiplication(A, B) == expected);

    // Rectangular matrix multiplication: 2x3 * 3x2
    Matrix C = {
        {1, 2, 3},
        {4, 5, 6}
    };

    Matrix D = {
        {7, 8},
        {9, 10},
        {11, 12}
    };

    Matrix expectedRectangular = {
        {58, 64},
        {139, 154}
    };

    assert(classicalMatrixMultiplication(C, D) == expectedRectangular);

    // Identity matrix
    Matrix identity = {
        {1, 0},
        {0, 1}
    };

    assert(classicalMatrixMultiplication(A, identity) == A);

    // Incompatible dimensions
    bool exceptionThrown = false;

    try {
        Matrix invalid = {{1, 2, 3}};
        classicalMatrixMultiplication(A, invalid);
    } catch (const std::invalid_argument&) {
        exceptionThrown = true;
    }

    assert(exceptionThrown);

    // Empty matrix
    exceptionThrown = false;

    try {
        Matrix empty;
        classicalMatrixMultiplication(empty, A);
    } catch (const std::invalid_argument&) {
        exceptionThrown = true;
    }

    assert(exceptionThrown);

    // Non-rectangular matrix
    exceptionThrown = false;

    try {
        Matrix invalid = {
            {1, 2},
            {3}
        };
        classicalMatrixMultiplication(invalid, A);
    } catch (const std::invalid_argument&) {
        exceptionThrown = true;
    }

    assert(exceptionThrown);

    std::cout << "All Classical Matrix Multiplication tests passed!\n";

    return 0;
}
