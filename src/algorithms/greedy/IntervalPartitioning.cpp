#include "algorithms/greedy/IntervalPartitioning.hpp"

#include <algorithm>
#include <queue>
#include <stdexcept>
#include <tuple>
#include <vector>

namespace warehouse::algorithms {

PartitioningResult intervalPartitioning(
    const std::vector<Interval>& intervals
) {
    std::vector<Interval> sorted = intervals;

    for (const auto& interval : sorted) {
        if (interval.start > interval.finish) {
            throw std::invalid_argument(
                "Interval start cannot exceed finish."
            );
        }
    }

    std::sort(sorted.begin(), sorted.end(),
        [](const Interval& a, const Interval& b) {
            if (a.start != b.start)
                return a.start < b.start;

            return a.finish < b.finish;
        }
    );

    // Min-heap: {finish time, room ID}
    using Room = std::pair<int, int>;

    std::priority_queue<
        Room,
        std::vector<Room>,
        std::greater<Room>
    > rooms;

    PartitioningResult result{0, {}};

    for (const auto& interval : sorted) {
        int roomId;

        if (!rooms.empty() && rooms.top().first <= interval.start) {
            roomId = rooms.top().second;
            rooms.pop();
        } else {
            roomId = result.roomCount++;
        }

        result.assignments.push_back({interval.id, roomId});

        rooms.push({interval.finish, roomId});
    }

    return result;
}

}