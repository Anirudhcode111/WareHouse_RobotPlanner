#include "algorithms/divide_conquer/FastPowering.hpp"

#include <cassert>
#include <cmath>
#include <iostream>
#include <stdexcept>

using warehouse::algorithms::fastPower;

int main() {
    // Positive exponent
    assert(std::abs(fastPower(2, 10) - 1024.0) < 1e-9);

    // Exponent zero
    assert(std::abs(fastPower(5, 0) - 1.0) < 1e-9);

    // Base one
    assert(std::abs(fastPower(1, 100) - 1.0) < 1e-9);

    // Negative exponent
    assert(std::abs(fastPower(2, -3) - 0.125) < 1e-9);

    // Negative base
    assert(std::abs(fastPower(-2, 3) - (-8.0)) < 1e-9);

    // Zero base with positive exponent
    assert(std::abs(fastPower(0, 5)) < 1e-9);

    // Zero base with negative exponent must throw
    bool exceptionThrown = false;

    try {
        fastPower(0, -2);
    } catch (const std::invalid_argument&) {
        exceptionThrown = true;
    }

    assert(exceptionThrown);

    std::cout << "All Fast Powering tests passed!\n";

    return 0;
}