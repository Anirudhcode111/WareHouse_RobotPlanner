#pragma once

#include "algorithms/divide_conquer/MatrixMultiplication.hpp"

namespace warehouse::algorithms {

Matrix strassenMatrixMultiplication(
    const Matrix& A,
    const Matrix& B
);

}