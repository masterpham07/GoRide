#include "services/PathFinder.h"
#include <queue>
#include <vector>
#include <utility>
#include <algorithm>

using namespace std;

vector<Location> PathFinder::findPathBFS(const Map& map, const Location& start, const Location& target) {
    int mapSize = map.getWidth();
    vector<vector<bool>> visited(mapSize, vector<bool>(mapSize, false));
    vector<vector<pair<int, int>>> parent(
        mapSize, vector<pair<int, int>>(mapSize, {-1, -1}));

    queue<pair<int, int>> q;
    q.push(make_pair(start.x, start.y));
    visited[start.y][start.x] = true;

    const int dx[4] = {1, -1, 0, 0};
    const int dy[4] = {0, 0, 1, -1};
    bool found = (start.x == target.x && start.y == target.y);

    while (!q.empty() && !found) {
        auto curr = q.front();
        int cx = curr.first, cy = curr.second;
        q.pop();
        for (int d = 0; d < 4; d++) {
            int nx = cx + dx[d], ny = cy + dy[d];
            if (nx < 0 || nx >= mapSize || ny < 0 || ny >= mapSize)
                continue;
            if (visited[ny][nx] || map.isObstacle(nx, ny))
                continue;
            visited[ny][nx] = true;
            parent[ny][nx] = {cx, cy};
            if (nx == target.x && ny == target.y) {
                found = true;
                break;
            }
            q.push(make_pair(nx, ny));
        }
    }

    vector<Location> path;
    if (!found)
        return path;

    int cx = target.x, cy = target.y;
    while (!(cx == start.x && cy == start.y)) {
        path.push_back(Location(cx, cy));
        auto p = parent[cy][cx];
        cx = p.first;
        cy = p.second;
    }
    reverse(path.begin(), path.end());
    return path;
}
