
#include "algorithms/divide_conquer/MaximumSubarray.hpp"

#include <algorithm>
#include <climits>

namespace warehouse::algorithms {

int maxCrossingSum(
    const std::vector<int>& data,
    int left,
    int mid,
    int right
) {
    int leftSum = INT_MIN;
    int sum = 0;

    for (int i = mid; i >= left; --i) {
        sum += data[i];
        leftSum = std::max(leftSum, sum);
    }

    int rightSum = INT_MIN;
    sum = 0;

    for (int i = mid + 1; i <= right; ++i) {
        sum += data[i];
        rightSum = std::max(rightSum, sum);
    }

    return leftSum + rightSum;
}

int maximumSubarrayRecursive(
    const std::vector<int>& data,
    int left,
    int right
) {
    if (left == right) {
        return data[left];
    }

    int mid = left + (right - left) / 2;

    int leftMax = maximumSubarrayRecursive(data, left, mid);
    int rightMax = maximumSubarrayRecursive(data, mid + 1, right);
    int crossMax = maxCrossingSum(data, left, mid, right);

    return std::max({leftMax, rightMax, crossMax});
}

int maximumSubarraySum(const std::vector<int>& data) {
    if (data.empty()) {
        return 0;
    }

    return maximumSubarrayRecursive(
        data, 0, static_cast<int>(data.size()) - 1
    );
}

}
