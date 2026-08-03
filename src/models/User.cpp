#include "models/User.h"

using namespace std;

string vehicleTypeToStr(VehicleType v) {
    switch (v) {
        case VehicleType::BIKE: return "Xe máy";
        case VehicleType::CAR4: return "Ô tô 4 chỗ";
        case VehicleType::CAR7: return "Ô tô 7 chỗ";
    }
    return "?";
}