#include "services/MatchingEngine.h"

using namespace std;

vector<Driver*> MatchingEngine::findEligibleDrivers(const vector<Driver*>& drivers, VehicleType vt) {
    vector<Driver*> eligible;
    for (auto* d : drivers) {
        if (d && d->isAvailable() && d->getVehicleType() == vt) {
            eligible.push_back(d);
        }
    }
    return eligible;
}

Driver* MatchingEngine::findNearestDriver(const Location& pickup, const vector<Driver*>& candidates) {
    Driver* best = nullptr;
    double bestDist = numeric_limits<double>::max();
    for (auto* d : candidates) {
        if (!d || !d->isAvailable()) continue;
        double dist = d->location().distanceTo(pickup);
        if (dist < bestDist) {
            bestDist = dist;
            best = d;
        }
    }
    return best;
}
