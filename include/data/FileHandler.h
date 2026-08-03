#pragma once
#include <string>
#include "models/Trip.h"

class FileHandler {
public:
    static std::string csvEscape(const std::string& field);
    static void appendTripToFile(const Trip& t, const std::string& path = "data/trips.csv");
    static void ensureHeader(const std::string& path = "data/trips.csv");
};
