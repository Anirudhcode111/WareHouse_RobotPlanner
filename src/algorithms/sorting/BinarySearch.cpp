
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

}
