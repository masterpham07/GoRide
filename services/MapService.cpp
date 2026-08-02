#include "services/MapService.h"
#include "services/PathFinder.h"

using namespace std;

Map MapService::mapInstance;

const Map& MapService::getMap() {
    return mapInstance;
}

const vector<Location>& MapService::places() {
    return mapInstance.getPlaces();
}

const vector<pair<int, int>>& MapService::obstacles() {
    return mapInstance.getObstacles();
}

bool MapService::isBlocked(int x, int y) {
    return mapInstance.isObstacle(x, y);
}

vector<Location> MapService::findPath(const Location& start, const Location& target) {
    return PathFinder::findPathBFS(mapInstance, start, target);
}
