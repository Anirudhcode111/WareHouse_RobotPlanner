
#include "algorithms/sorting/MergeSort.hpp"

#include <vector>

namespace warehouse::algorithms {

namespace {

void merge(std::vector<int>& data, int left, int mid, int right) {
    std::vector<int> temp;

    int i = left;
    int j = mid + 1;

    // Compare elements from both halves.
    while (i <= mid && j <= right) {
        if (data[i] <= data[j]) {
            temp.push_back(data[i]);
            i++;
        } else {
            temp.push_back(data[j]);
            j++;
        }
    }

    // Copy remaining elements from the left half.
    while (i <= mid) {
        temp.push_back(data[i]);
        i++;
    }

    // Copy remaining elements from the right half.
    while (j <= right) {
        temp.push_back(data[j]);
        j++;
    }

    // Copy merged elements back into the original array.
    for (int k = 0; k < static_cast<int>(temp.size()); k++) {
        data[left + k] = temp[k];
    }
}

void mergeSortRecursive(std::vector<int>& data, int left, int right) {
    if (left >= right) {
        return;
    }

    int mid = left + (right - left) / 2;

    mergeSortRecursive(data, left, mid);
    mergeSortRecursive(data, mid + 1, right);

    merge(data, left, mid, right);
}

} // anonymous namespace

void mergeSort(std::vector<int>& data) {
    if (data.size() <= 1) {
        return;
    }

    mergeSortRecursive(data, 0, static_cast<int>(data.size()) - 1);
}

} // namespace warehouse::algorithms
