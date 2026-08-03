#include "models/Place.h"

using namespace std;

Place::Place(
    const string& name_,
    const Location& location_,
    PlaceType type_,
    const string& iconResource_
) : name(name_), location(location_), type(type_), iconResource(iconResource_) {}

const string& Place::getName() const {
    return name;
}

const Location& Place::getLocation() const {
    return location;
}

PlaceType Place::getType() const {
    return type;
}

const string& Place::getIconResource() const {
    return iconResource;
}
