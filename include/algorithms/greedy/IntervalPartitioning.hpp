#pragma once

#include <vector>

namespace warehouse::algorithms {

struct Interval {
    int id;
    int start;
    int finish;
};

struct IntervalAssignment {
    int intervalId;
    int roomId;
};

struct PartitioningResult {
    int roomCount;
    std::vector<IntervalAssignment> assignments;
};

PartitioningResult intervalPartitioning(
    const std::vector<Interval>& intervals
);

}