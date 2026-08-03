#pragma once
#include <string>
#include "Location.h"

enum class TripStatus { PENDING, ACCEPTED, ONGOING, COMPLETED, CANCELLED };

inline std::string statusToStr(TripStatus s) {
    switch (s) {
        case TripStatus::PENDING:   return "Chờ tìm tài xế";
        case TripStatus::ACCEPTED:  return "Đã nhận chuyến";
        case TripStatus::ONGOING:   return "Đang di chuyển";
        case TripStatus::COMPLETED: return "Hoàn thành";
        case TripStatus::CANCELLED: return "Đã hủy";
    }
    return "?";
}

struct Trip {
    std::string tripId;
    std::string riderId;
    std::string driverId;
    Location pickup;
    Location dropoff;
    double distanceKm = 0;
    double fare = 0;
    TripStatus status = TripStatus::PENDING;
    int ratingGiven = 0;

    // Thông tin bổ sung để hiển thị đầy đủ trong lịch sử chuyến đi
    std::string driverName;
    std::string driverPlate;
    std::string vehicleTypeStr;
    std::string paymentMethod; // "Ví điện tử" hoặc "Tiền mặt"
};