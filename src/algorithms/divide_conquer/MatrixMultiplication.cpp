#include "algorithms/divide_conquer/MatrixMultiplication.hpp"

#include <stdexcept>

namespace warehouse::algorithms {

Matrix classicalMatrixMultiplication(
    const Matrix& A,
    const Matrix& B
) {
    if (A.empty() || B.empty()) {
        throw std::invalid_argument("Matrices cannot be empty.");
    }

    const std::size_t rowsA = A.size();
    const std::size_t colsA = A[0].size();
    const std::size_t rowsB = B.size();
    const std::size_t colsB = B[0].size();

    for (const auto& row : A) {
        if (row.size() != colsA) {
            throw std::invalid_argument("Matrix A is not rectangular.");
        }
    }

    for (const auto& row : B) {
        if (row.size() != colsB) {
            throw std::invalid_argument("Matrix B is not rectangular.");
        }
    }

    if (colsA != rowsB || colsA == 0 || colsB == 0) {
        throw std::invalid_argument("Incompatible matrix dimensions.");
    }

    Matrix result(rowsA, std::vector<int>(colsB, 0));

    for (std::size_t i = 0; i < rowsA; ++i) {
        for (std::size_t j = 0; j < colsB; ++j) {
            for (std::size_t k = 0; k < colsA; ++k) {
                result[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    return result;
}

}