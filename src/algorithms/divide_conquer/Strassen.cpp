
#include "algorithms/divide_conquer/Strassen.hpp"

#include <algorithm>
#include <stdexcept>

namespace warehouse::algorithms {
namespace {

using Size = std::size_t;

Matrix add(const Matrix& A, const Matrix& B) {
    Size n = A.size();
    Matrix result(n, std::vector<int>(n));

    for (Size i = 0; i < n; ++i)
        for (Size j = 0; j < n; ++j)
            result[i][j] = A[i][j] + B[i][j];

    return result;
}

Matrix subtract(const Matrix& A, const Matrix& B) {
    Size n = A.size();
    Matrix result(n, std::vector<int>(n));

    for (Size i = 0; i < n; ++i)
        for (Size j = 0; j < n; ++j)
            result[i][j] = A[i][j] - B[i][j];

    return result;
}

Matrix multiplyRecursive(const Matrix& A, const Matrix& B) {
    Size n = A.size();

    if (n <= 2) {
        Matrix result(n, std::vector<int>(n, 0));

        for (Size i = 0; i < n; ++i)
            for (Size j = 0; j < n; ++j)
                for (Size k = 0; k < n; ++k)
                    result[i][j] += A[i][k] * B[k][j];

        return result;
    }

    Size half = n / 2;

    Matrix A11(half, std::vector<int>(half));
    Matrix A12 = A11, A21 = A11, A22 = A11;
    Matrix B11 = A11, B12 = A11, B21 = A11, B22 = A11;

    for (Size i = 0; i < half; ++i) {
        for (Size j = 0; j < half; ++j) {
            A11[i][j] = A[i][j];
            A12[i][j] = A[i][j + half];
            A21[i][j] = A[i + half][j];
            A22[i][j] = A[i + half][j + half];

            B11[i][j] = B[i][j];
            B12[i][j] = B[i][j + half];
            B21[i][j] = B[i + half][j];
            B22[i][j] = B[i + half][j + half];
        }
    }

    Matrix M1 = multiplyRecursive(add(A11, A22), add(B11, B22));
    Matrix M2 = multiplyRecursive(add(A21, A22), B11);
    Matrix M3 = multiplyRecursive(A11, subtract(B12, B22));
    Matrix M4 = multiplyRecursive(A22, subtract(B21, B11));
    Matrix M5 = multiplyRecursive(add(A11, A12), B22);
    Matrix M6 = multiplyRecursive(subtract(A21, A11), add(B11, B12));
    Matrix M7 = multiplyRecursive(subtract(A12, A22), add(B21, B22));

    Matrix C11 = add(subtract(add(M1, M4), M5), M7);
    Matrix C12 = add(M3, M5);
    Matrix C21 = add(M2, M4);
    Matrix C22 = add(subtract(add(M1, M3), M2), M6);

    Matrix result(n, std::vector<int>(n));

    for (Size i = 0; i < half; ++i) {
        for (Size j = 0; j < half; ++j) {
            result[i][j] = C11[i][j];
            result[i][j + half] = C12[i][j];
            result[i + half][j] = C21[i][j];
            result[i + half][j + half] = C22[i][j];
        }
    }

    return result;
}

void validateMatrix(const Matrix& matrix) {
    if (matrix.empty() || matrix[0].empty())
        throw std::invalid_argument("Matrices cannot be empty.");

    for (const auto& row : matrix) {
        if (row.size() != matrix.size())
            throw std::invalid_argument("Strassen requires square matrices.");
    }
}

} // namespace

Matrix strassenMatrixMultiplication(const Matrix& A, const Matrix& B) {
    validateMatrix(A);
    validateMatrix(B);

    if (A.size() != B.size())
        throw std::invalid_argument("Matrix dimensions must match.");

    Size n = A.size();
    Size paddedSize = 1;

    while (paddedSize < n)
        paddedSize *= 2;

    Matrix paddedA(paddedSize, std::vector<int>(paddedSize, 0));
    Matrix paddedB(paddedSize, std::vector<int>(paddedSize, 0));

    for (Size i = 0; i < n; ++i) {
        for (Size j = 0; j < n; ++j) {
            paddedA[i][j] = A[i][j];
            paddedB[i][j] = B[i][j];
        }
    }

    Matrix paddedResult = multiplyRecursive(paddedA, paddedB);

    Matrix result(n, std::vector<int>(n));

    for (Size i = 0; i < n; ++i)
        for (Size j = 0; j < n; ++j)
            result[i][j] = paddedResult[i][j];

    return result;
}

}
