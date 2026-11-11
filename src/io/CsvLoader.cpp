

#include "io/CsvLoader.hpp"

#include <cmath>
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

namespace warehouse {
namespace {

std::string trim(const std::string& value) {
    const auto first = value.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) {
        return "";
    }

    const auto last = value.find_last_not_of(" \t\r\n");
    return value.substr(first, last - first + 1);
}

// Parses a CSV line, including quoted fields and escaped quotes ("").
bool parseCsvLine(
    const std::string& line,
    std::vector<std::string>& fields
) {
    fields.clear();

    std::string field;
    bool inQuotes = false;
    bool closedQuote = false;

    for (std::size_t i = 0; i < line.size(); ++i) {
        const char ch = line[i];

        if (inQuotes) {
            if (ch == '"') {
                if (i + 1 < line.size() && line[i + 1] == '"') {
                    field += '"';
                    ++i;
                } else {
                    inQuotes = false;
                    closedQuote = true;
                }
            } else {
                field += ch;
            }
        } else if (ch == ',' ) {
            fields.push_back(trim(field));
            field.clear();
            closedQuote = false;
        } else if (ch == '"' && field.empty() && !closedQuote) {
            inQuotes = true;
        } else {
            if (closedQuote && ch != ' ' && ch != '\t' &&
                ch != '\r') {
                return false;
            }

            if (!closedQuote) {
                field += ch;
            }
        }
    }

    if (inQuotes) {
        return false;
    }

    fields.push_back(trim(field));
    return true;
}

bool readCsv(
    const std::filesystem::path& path,
    std::vector<std::vector<std::string>>& rows,
    std::string& errorMessage
) {
    std::ifstream file(path);

    if (!file.is_open()) {
        errorMessage = "Cannot open file: " + path.string();
        return false;
    }

    rows.clear();
    std::string line;
    std::size_t lineNumber = 0;

    while (std::getline(file, line)) {
        ++lineNumber;

        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        if (trim(line).empty()) {
            continue;
        }

        std::vector<std::string> fields;

        if (!parseCsvLine(line, fields)) {
            errorMessage = "Invalid CSV quoting in " +
                path.string() + " at line " +
                std::to_string(lineNumber);
            return false;
        }

        rows.push_back(std::move(fields));
    }

    if (file.bad()) {
        errorMessage = "Error reading file: " + path.string();
        return false;
    }

    if (rows.empty()) {
        errorMessage = "CSV file is empty: " + path.string();
        return false;
    }

    return true;
}

bool readTable(
    const std::filesystem::path& path,
    const std::vector<std::string>& requiredColumns,
    std::vector<std::unordered_map<std::string, std::string>>& records,
    std::string& errorMessage
) {
    std::vector<std::vector<std::string>> rows;

    if (!readCsv(path, rows, errorMessage)) {
        return false;
    }

    const auto& header = rows.front();
    std::unordered_map<std::string, std::size_t> columnIndex;

    for (std::size_t i = 0; i < header.size(); ++i) {
        const std::string name = trim(header[i]);

        if (name.empty() ||
            !columnIndex.emplace(name, i).second) {
            errorMessage = "Empty or duplicate column in: " +
                path.string();
            return false;
        }
    }

    for (const auto& required : requiredColumns) {
        if (columnIndex.find(required) == columnIndex.end()) {
            errorMessage = "Missing column '" + required +
                "' in: " + path.string();
            return false;
        }
    }

    records.clear();

    for (std::size_t r = 1; r < rows.size(); ++r) {
        const auto& row = rows[r];

        if (row.size() != header.size()) {
            errorMessage = "Column count mismatch in " +
                path.string() + " at data row " +
                std::to_string(r + 1);
            return false;
        }

        std::unordered_map<std::string, std::string> record;

        for (const auto& column : columnIndex) {
            record[column.first] = trim(row[column.second]);
        }

        records.push_back(std::move(record));
    }

    return true;
}

bool parseDouble(
    const std::string& text,
    double& result
) {
    try {
        std::size_t used = 0;
        result = std::stod(text, &used);

        return used == text.size() && std::isfinite(result);
    } catch (const std::exception&) {
        return false;
    }
}

bool parseInt(
    const std::string& text,
    int& result
) {
    try {
        std::size_t used = 0;
        const long value = std::stol(text, &used);

        if (used != text.size() ||
            value < 0 ||
            value > 2147483647L) {
            return false;
        }

        result = static_cast<int>(value);
        return true;
    } catch (const std::exception&) {
        return false;
    }
}

bool requireValue(
    const std::unordered_map<std::string, std::string>& record,
    const std::string& key,
    std::string& value,
    std::string& errorMessage
) {
    const auto it = record.find(key);

    if (it == record.end() || it->second.empty()) {
        errorMessage = "Missing value for column: " + key;
        return false;
    }

    value = it->second;
    return true;
}

bool uniqueId(
    const std::string& id,
    std::unordered_set<std::string>& ids,
    std::string& errorMessage,
    const std::string& entity
) {
    if (id.empty()) {
        errorMessage = entity + " ID cannot be empty.";
        return false;
    }

    if (!ids.insert(id).second) {
        errorMessage = "Duplicate " + entity + " ID: " + id;
        return false;
    }

    return true;
}

} // namespace

bool CsvLoader::loadFromDirectory(
    const std::string& directory,
    Warehouse& warehouse,
    std::string& errorMessage
) {
    errorMessage.clear();

    try {
        const std::filesystem::path root(directory);

        if (!std::filesystem::is_directory(root)) {
            errorMessage = "Directory does not exist: " + directory;
            return false;
        }

        std::vector<std::unordered_map<std::string, std::string>>
            locationRows, robotRows, taskRows, connectionRows,
            obstacleRows;

        if (!readTable(
                root / "locations.csv",
                {"id"}, locationRows, errorMessage) ||
            !readTable(
                root / "robots.csv",
                {"id", "currentLocationId", "capacityKg"},
                robotRows, errorMessage) ||
            !readTable(
                root / "tasks.csv",
                {"id", "pickupLocationId", "weightKg",
                 "priority", "status"},
                taskRows, errorMessage) ||
            !readTable(
                root / "connections.csv",
                {"endpointAId", "endpointBId", "distance"},
                connectionRows, errorMessage) ||
            !readTable(
                root / "obstacles.csv",
                {"endpointAId", "endpointBId"},
                obstacleRows, errorMessage)) {
            return false;
        }

        Warehouse loaded;
        std::unordered_set<std::string> locationIds;
        std::unordered_set<std::string> robotIds;
        std::unordered_set<std::string> taskIds;

        for (const auto& row : locationRows) {
            std::string id;

            if (!requireValue(row, "id", id, errorMessage) ||
                !uniqueId(id, locationIds, errorMessage, "Location")) {
                return false;
            }

            loaded.locations.push_back({id});
        }

        for (const auto& row : robotRows) {
            std::string id, locationId, capacityText;
            double capacity = 0.0;

            if (!requireValue(row, "id", id, errorMessage) ||
                !requireValue(row, "currentLocationId",
                              locationId, errorMessage) ||
                !requireValue(row, "capacityKg",
                              capacityText, errorMessage) ||
                !uniqueId(id, robotIds, errorMessage, "Robot")) {
                return false;
            }

            if (!parseDouble(capacityText, capacity) ||
                capacity < 0.0) {
                errorMessage = "Invalid robot capacity for: " + id;
                return false;
            }

            if (locationIds.find(locationId) == locationIds.end()) {
                errorMessage = "Unknown robot location: " + locationId;
                return false;
            }

            loaded.robots.push_back({id, locationId, capacity});
        }

        for (const auto& row : taskRows) {
            std::string id, locationId, weightText, priorityText, status;
            double weight = 0.0;
            int priority = 0;

            if (!requireValue(row, "id", id, errorMessage) ||
                !requireValue(row, "pickupLocationId",
                              locationId, errorMessage) ||
                !requireValue(row, "weightKg",
                              weightText, errorMessage) ||
                !requireValue(row, "priority",
                              priorityText, errorMessage) ||
                !requireValue(row, "status", status, errorMessage) ||
                !uniqueId(id, taskIds, errorMessage, "Task")) {
                return false;
            }

            if (!parseDouble(weightText, weight) || weight < 0.0 ||
                !parseInt(priorityText, priority)) {
                errorMessage = "Invalid weight or priority for task: " + id;
                return false;
            }

            if (locationIds.find(locationId) == locationIds.end()) {
                errorMessage = "Unknown task pickup location: " + locationId;
                return false;
            }

            TaskStatus taskStatus;

            if (status == "Pending") {
                taskStatus = TaskStatus::Pending;
            } else if (status == "Completed") {
                taskStatus = TaskStatus::Completed;
            } else {
                errorMessage = "Invalid task status for: " + id;
                return false;
            }

            loaded.tasks.push_back(
                {id, locationId, weight, priority, taskStatus});
        }

        std::unordered_set<std::string> connectionKeys;

        for (const auto& row : connectionRows) {
            std::string a, b, distanceText;
            double distance = 0.0;

            if (!requireValue(row, "endpointAId", a, errorMessage) ||
                !requireValue(row, "endpointBId", b, errorMessage) ||
                !requireValue(row, "distance",
                              distanceText, errorMessage)) {
                return false;
            }

            if (locationIds.find(a) == locationIds.end() ||
                locationIds.find(b) == locationIds.end()) {
                errorMessage = "Connection references an unknown location.";
                return false;
            }

            if (a == b) {
                errorMessage = "Self-connections are not allowed: " + a;
                return false;
            }

            if (!parseDouble(distanceText, distance) || distance < 0.0) {
                errorMessage = "Invalid connection distance.";
                return false;
            }

            const std::string key = (a < b)
                ? a + "\n" + b
                : b + "\n" + a;

            if (!connectionKeys.insert(key).second) {
                errorMessage = "Duplicate connection: " + a + " - " + b;
                return false;
            }

            loaded.connections.push_back({a, b, distance});
        }

        std::unordered_set<std::string> obstacleKeys;

        for (const auto& row : obstacleRows) {
            std::string a, b;

            if (!requireValue(row, "endpointAId", a, errorMessage) ||
                !requireValue(row, "endpointBId", b, errorMessage)) {
                return false;
            }

            if (locationIds.find(a) == locationIds.end() ||
                locationIds.find(b) == locationIds.end()) {
                errorMessage = "Obstacle references an unknown location.";
                return false;
            }

            if (a == b) {
                errorMessage = "Obstacle endpoints cannot be identical.";
                return false;
            }

            const std::string key = (a < b)
                ? a + "\n" + b
                : b + "\n" + a;

            if (!obstacleKeys.insert(key).second) {
                errorMessage = "Duplicate obstacle: " + a + " - " + b;
                return false;
            }

            loaded.obstacles.push_back({a, b});
        }

        // Only replace the destination if every file was read successfully.
        warehouse = std::move(loaded);
        return true;
    } catch (const std::exception& exception) {
        errorMessage = std::string("CSV loading failed: ") +
            exception.what();
        return false;
    }
}

} // namespace warehouse
