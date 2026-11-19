#pragma once

#include <vector>

namespace warehouse::algorithms {

using Matrix = std::vector<std::vector<int>>;

Matrix classicalMatrixMultiplication(
    const Matrix& A,
    const Matrix& B
);

}