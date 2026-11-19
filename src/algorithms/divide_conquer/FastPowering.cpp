#include "algorithms/divide_conquer/FastPowering.hpp"

#include <stdexcept>

namespace warehouse::algorithms {

double fastPower(double base, int exponent) {
    if (base == 0.0 && exponent < 0) {
        throw std::invalid_argument("Zero cannot have a negative exponent.");
    }

    long long power = exponent;
    bool negativeExponent = power < 0;

    if (negativeExponent) {
        power = -power;
    }

    double result = 1.0;

    while (power > 0) {
        if (power % 2 == 1) {
            result *= base;
        }

        base *= base;
        power /= 2;
    }

    return negativeExponent ? 1.0 / result : result;
}

}