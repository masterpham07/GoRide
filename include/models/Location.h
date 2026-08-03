#pragma once
#include <cmath>
#include <string>

// Toạ độ trên bản đồ dạng lưới (grid) MAP_SIZE x MAP_SIZE
// Mô phỏng bản đồ thành phố đơn giản hoá thành lưới ô vuông
struct Location {
    int x = 0;
    int y = 0;
    std::string label; // tên địa điểm, ví dụ "Vincom Đồng Khởi"

    Location() = default;
    Location(int x_, int y_, std::string label_ = "")
        : x(x_), y(y_), label(std::move(label_)) {}

    // Khoảng cách Euclid quy đổi ra km (mỗi ô lưới = 0.5km)
    double distanceTo(const Location& other) const {
        double dx = x - other.x;
        double dy = y - other.y;
        return std::sqrt(dx * dx + dy * dy) * 0.5;
    }

    bool operator==(const Location& other) const {
        return x == other.x && y == other.y;
    }
};
