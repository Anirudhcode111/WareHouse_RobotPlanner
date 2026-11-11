
#pragma once

#include "domain/Warehouse.hpp"

#include <string>

namespace warehouse {

class CsvLoader {
public:
    // Loads warehouse data from a directory containing CSV files.
    // Returns true if all files are read successfully.
    // On failure, errorMessage explains the problem.
    static bool loadFromDirectory(
        const std::string& directory,
        Warehouse& warehouse,
        std::string& errorMessage
    );
};

} // namespace warehouse
