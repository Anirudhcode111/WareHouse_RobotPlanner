
#include "algorithms/sorting/QuickSort.hpp"

#include <vector>

namespace warehouse::algorithms {

namespace {

int partition(std::vector<int>& data, int low, int high) {
    // Select the middle element as the pivot.
    int pivot = data[low + (high - low) / 2];

    int i = low - 1;
    int j = high + 1;

    while (true) {
        // Move from the left until an element >= pivot is found.
        do {
            i++;
        } while (data[i] < pivot);

        // Move from the right until an element <= pivot is found.
        do {
            j--;
        } while (data[j] > pivot);

        // The pointers have crossed.
        if (i >= j) {
            return j;
        }

        // Swap misplaced elements.
        std::swap(data[i], data[j]);
    }
}

void quickSortRecursive(std::vector<int>& data, int low, int high) {
    if (low >= high) {
        return;
    }

    int pivotIndex = partition(data, low, high);

    quickSortRecursive(data, low, pivotIndex);
    quickSortRecursive(data, pivotIndex + 1, high);
}

} // anonymous namespace

void quickSort(std::vector<int>& data) {
    if (data.size() <= 1) {
        return;
    }

    quickSortRecursive(data, 0, static_cast<int>(data.size()) - 1);
}

} // namespace warehouse::algorithms
