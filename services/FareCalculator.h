#pragma once
#include "models/User.h"

class FareCalculator {
public:
    static double baseFare(VehicleType v);
    static double perKmRate(VehicleType v);
    static bool isPeakHour();
    static double calculate(double distanceKm, VehicleType v);
};
