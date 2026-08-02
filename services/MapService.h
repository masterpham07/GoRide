#pragma once
#include <vector>
#include <utility>
#include "models/Map.h"
#include "models/Location.h"

class MapService {
private:
    static Map mapInstance;

public:
    static const int MAP_SIZE = Map::MAP_SIZE;

    static const Map& getMap();
    static const std::vector<Location>& places();
    static const std::vector<std::pair<int, int>>& obstacles();
    static bool isBlocked(int x, int y);

    static std::vector<Location> findPath(const Location& start, const Location& target);
};
