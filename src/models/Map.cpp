#include "models/Map.h"

using namespace std;

Map::Map() {
    places = {
        Location(1, 1, "Bến xe Miền Đông"),
        Location(14, 2, "Sân bay Tân Sơn Nhất"),
        Location(7, 8, "Chợ Bến Thành"),
        Location(12, 4, "Landmark 81"),
        Location(2, 13, "Đại học Bách Khoa"),
        Location(13, 12, "Vincom Đồng Khởi"),
        Location(5, 3, "Công viên Tào Đàn"),
        Location(9, 14, "Bến Nhà Rồng")
    };

    obstacles = {
        {8, 4}, {9, 4}, {10, 4}, {3, 9}, {4, 9}, {5, 9}, {12, 6}, {13, 6}, {6, 11}, {7, 11}, {8, 11}, {7, 2}, {8, 2}, {9, 13}, {10, 13}, {1, 8}, {2, 8}, {11, 14}, {12, 14}
    };
}

bool Map::isInside(int x, int y) const {
    return x >= 0 && x < MAP_SIZE && y >= 0 && y < MAP_SIZE;
}

bool Map::isInside(const Location& loc) const {
    return isInside(loc.x, loc.y);
}

bool Map::isObstacle(int x, int y) const {
    for (const auto& o : obstacles) {
        if (o.first == x && o.second == y) return true;
    }
    return false;
}

bool Map::isObstacle(const Location& loc) const {
    return isObstacle(loc.x, loc.y);
}

const vector<Location>& Map::getPlaces() const {
    return places;
}

const vector<pair<int, int>>& Map::getObstacles() const {
    return obstacles;
}
