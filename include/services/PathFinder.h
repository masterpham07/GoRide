#pragma once
#include <vector>
#include "models/Map.h"
#include "models/Location.h"

class PathFinder {
public:
    // Tìm đường bằng BFS, chỉ đi 4 hướng (trên/dưới/trái/phải), NÉ vật cản.
    // Trả về danh sách các ô (không tính ô bắt đầu) từ start đến target. Rỗng nếu không có đường đi.
    static std::vector<Location> findPathBFS(
        const Map& map,
        const Location& start,
        const Location& target
    );
};
