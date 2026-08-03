#pragma once
#include <string>
#include "Location.h"

enum class PlaceType
{
    Building,
    School,
    Airport,
    Park,
    Harbor,
    BusStation,
    Mall,
    Pickup,
    Destination
};

class Place
{
public:
    Place(
        const std::string& name,
        const Location& location,
        PlaceType type,
        const std::string& iconResource
    );

    const std::string& getName() const;
    const Location& getLocation() const;
    PlaceType getType() const;
    const std::string& getIconResource() const;

private:
    std::string name;
    Location location;
    PlaceType type;
    std::string iconResource;
};