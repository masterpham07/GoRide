#include "services/FareCalculator.h"
#include <ctime>
#include <cmath>

using namespace std;

double FareCalculator::baseFare(VehicleType v) {
    switch (v) {
        case VehicleType::BIKE: return 12000;
        case VehicleType::CAR4: return 25000;
        case VehicleType::CAR7: return 32000;
    }
    return 15000;
}

double FareCalculator::perKmRate(VehicleType v) {
    switch (v) {
        case VehicleType::BIKE: return 4500;
        case VehicleType::CAR4: return 10500;
        case VehicleType::CAR7: return 13500;
    }
    return 5000;
}

bool FareCalculator::isPeakHour() {
    time_t now = time(nullptr);
    tm* lt = localtime(&now);
    int h = lt->tm_hour;
    return (h >= 7 && h <= 9) || (h >= 17 && h <= 19);
}

double FareCalculator::calculate(double distanceKm, VehicleType v) {
    double fare = baseFare(v) + perKmRate(v) * distanceKm;
    if (isPeakHour()) fare *= 1.2;
    return round(fare / 500.0) * 500.0;
}
