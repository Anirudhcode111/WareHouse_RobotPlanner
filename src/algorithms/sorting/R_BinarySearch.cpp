
#include "algorithms/sorting/BinarySearch.hpp"

namespace warehouse::algorithms {

int binarySearchIterative(const std::vector<int>& data, int target) {
    int left = 0;
    int right = static_cast<int>(data.size()) - 1;
    int result = -1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (data[mid] == target) {
            result = mid;
            right = mid - 1;
        }
        else if (data[mid] < target) {
            left = mid + 1;
        }
        else {
            right = mid - 1;
        }
    }

    return result;
}

namespace {

int recursiveSearch(
    const std::vector<int>& data,
    int target,
    int left,
    int right,
    int result
) {
    if (left > right) {
        return result;
    }

    int mid = left + (right - left) / 2;

    if (data[mid] == target) {
        result = mid;
        return recursiveSearch(data, target, left, mid - 1, result);
    }

    if (data[mid] < target) {
        return recursiveSearch(data, target, mid + 1, right, result);
    }

    return recursiveSearch(data, target, left, mid - 1, result);
}

} // anonymous namespace

int binarySearchRecursive(const std::vector<int>& data, int target) {
    return recursiveSearch(
        data,
        target,
        0,
        static_cast<int>(data.size()) - 1,
        -1
    );
}

}
