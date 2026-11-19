
#include "algorithms/greedy/IntervalPartitioning.hpp"

#include <cassert>
#include <iostream>
#include <stdexcept>
#include <unordered_map>
#include <vector>

using namespace warehouse::algorithms;

void verifyNoOverlap(
    const std::vector<Interval>& intervals,
    const PartitioningResult& result
) {
    assert(intervals.size() == result.assignments.size());

    std::unordered_map<int, Interval> intervalById;

    for (const auto& interval : intervals) {
        assert(intervalById.emplace(interval.id, interval).second);
    }

    std::unordered_map<int, std::vector<Interval>> rooms;

    for (const auto& assignment : result.assignments) {
        auto it = intervalById.find(assignment.intervalId);
        assert(it != intervalById.end());

        rooms[assignment.roomId].push_back(it->second);
    }

    assert(static_cast<int>(rooms.size()) == result.roomCount);

    for (auto& [roomId, assigned] : rooms) {
        (void)roomId;

        for (std::size_t i = 0; i < assigned.size(); ++i) {
            for (std::size_t j = i + 1; j < assigned.size(); ++j) {
                bool overlap =
                    assigned[i].start < assigned[j].finish &&
                    assigned[j].start < assigned[i].finish;

                assert(!overlap);
            }
        }
    }
}

int main() {
    // Normal case
    std::vector<Interval> intervals = {
        {1, 1, 4},
        {2, 2, 5},
        {3, 3, 6},
        {4, 4, 7},
        {5, 5, 8}
    };

    auto result = intervalPartitioning(intervals);

    assert(result.roomCount == 3);
    verifyNoOverlap(intervals, result);

    // All intervals compatible
    std::vector<Interval> compatible = {
        {1, 1, 2},
        {2, 2, 3},
        {3, 3, 4}
    };

    result = intervalPartitioning(compatible);
    assert(result.roomCount == 1);
    verifyNoOverlap(compatible, result);

    // All intervals overlap
    std::vector<Interval> overlapping = {
        {1, 1, 10},
        {2, 2, 9},
        {3, 3, 8}
    };

    result = intervalPartitioning(overlapping);
    assert(result.roomCount == 3);
    verifyNoOverlap(overlapping, result);

    // Empty input
    result = intervalPartitioning({});
    assert(result.roomCount == 0);
    assert(result.assignments.empty());

    // Single interval
    std::vector<Interval> single = {{1, 5, 10}};
    result = intervalPartitioning(single);
    assert(result.roomCount == 1);
    verifyNoOverlap(single, result);

    // Invalid interval
    bool exceptionThrown = false;

    try {
        intervalPartitioning({{1, 10, 5}});
    } catch (const std::invalid_argument&) {
        exceptionThrown = true;
    }

    assert(exceptionThrown);

    std::cout << "All Interval Partitioning tests passed!\n";

    return 0;
}
