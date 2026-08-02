#pragma once
#include <vector>
#include <utility>
#include "Location.h"

class Map {
private:
    std::vector<Location> places;
    std::vector<std::pair<int, int>> obstacles;

public:
    static constexpr int MAP_SIZE = 16;

    Map();

    int getWidth() const { return MAP_SIZE; }
    int getHeight() const { return MAP_SIZE; }

    bool isInside(const Location& loc) const;
    bool isInside(int x, int y) const;

    bool isObstacle(int x, int y) const;
    bool isObstacle(const Location& loc) const;

    const std::vector<Location>& getPlaces() const;
    const std::vector<std::pair<int, int>>& getObstacles() const;
};
