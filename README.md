# Warehouse Robot Planner — Phase I / P1

This starter implements only the first slice: typed domain records and basic validation.

## Build with CMake
```bash
cmake -S . -B build
cmake --build build
```
Run on Linux/WSL: `./build/warehouse_planner`
On Windows, run the generated `warehouse_planner.exe` inside `build` (or `build/Debug` for a Visual Studio generator).

## Direct MinGW/g++ build
```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic -Iinclude src/main.cpp src/domain/validation.cpp -o warehouse_planner
```

## P1 scope
Included: Location, Robot, Task, Obstacle, Warehouse records; duplicate-ID, invalid numeric-value and invalid-reference validation; CLI sample.
Not yet included: weighted graph, Dijkstra, Greedy, Branch & Bound, oracle, benchmark framework, frontend.
