#pragma once
#include <vector>
#include <limits>
#include "models/User.h"
#include "models/Location.h"

class MatchingEngine {
public:
    // Trả về toàn bộ danh sách tài xế đang rảnh và đúng loại xe
    static std::vector<Driver*> findEligibleDrivers(
        const std::vector<Driver*>& drivers,
        VehicleType vt
    );

    // Tìm tài xế gần điểm đón nhất trong danh sách ứng viên
    static Driver* findNearestDriver(
        const Location& pickup,
        const std::vector<Driver*>& candidates
    );
};
