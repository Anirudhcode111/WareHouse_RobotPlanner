#pragma once

#include <vector>

namespace warehouse::algorithms {

struct Activity {
    int id;
    int start;
    int finish;
};

std::vector<Activity> activitySelection(
    const std::vector<Activity>& activities
);

}