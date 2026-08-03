#pragma once
#include <string>
#include <vector>
#include <utility>
#include "Location.h"
#include "Trip.h"

// ===================== USER (lớp cơ sở - trừu tượng) =====================
class User {
protected:
    std::string id;
    std::string name;
    std::string phone;
    std::string password;

public:
    User(std::string id_, std::string name_, std::string phone_, std::string pass_)
        : id(std::move(id_)), name(std::move(name_)), phone(std::move(phone_)), password(std::move(pass_)) {}

    virtual ~User() = default;

    virtual std::string role() const = 0;

    const std::string& getId() const { return id; }
    const std::string& getName() const { return name; }
    const std::string& getPhone() const { return phone; }
    bool checkPassword(const std::string& p) const { return password == p; }
};

// ===================== RIDER =====================
class Rider : public User {
private:
    double walletBalance;
    std::vector<Trip> tripHistory;

public:
    Rider(std::string id_, std::string name_, std::string phone_, std::string pass_, double wallet_ = 200000)
        : User(std::move(id_), std::move(name_), std::move(phone_), std::move(pass_)), walletBalance(wallet_) {}

    std::string role() const override { return "RIDER"; }

    double getWallet() const { return walletBalance; }
    void addToWallet(double amount) { walletBalance += amount; }
    bool deductWallet(double amount) {
        if (walletBalance < amount) return false;
        walletBalance -= amount;
        return true;
    }
    void addTrip(const Trip& trip) { tripHistory.push_back(trip); }
    const std::vector<Trip>& getHistory() const { return tripHistory; }
};

// ===================== DRIVER =====================
enum class VehicleType { BIKE, CAR4, CAR7 };

std::string vehicleTypeToStr(VehicleType v);

class Driver : public User {
private:
    std::string licensePlate;
    VehicleType vehicleType;
    Location currentLocation;
    bool available = true;
    double rating = 5.0;
    int ratingCount = 0;
    double totalEarnings = 0;

public:
    Driver(std::string id_, std::string name_, std::string phone_, std::string pass_,
           std::string plate_, VehicleType vt_, Location loc_)
        : User(std::move(id_), std::move(name_), std::move(phone_), std::move(pass_)),
          licensePlate(std::move(plate_)), vehicleType(vt_), currentLocation(loc_) {}

    std::string role() const override { return "DRIVER"; }

    const std::string& getPlate() const { return licensePlate; }
    VehicleType getVehicleType() const { return vehicleType; }
    Location& location() { return currentLocation; }
    const Location& location() const { return currentLocation; }

    bool isAvailable() const { return available; }
    void setAvailable(bool v) { available = v; }

    double getRating() const { return rating; }
    void addRating(double star) {
        rating = (rating * ratingCount + star) / (ratingCount + 1);
        ratingCount++;
    }

    void addEarnings(double amount) { totalEarnings += amount; }
    double getEarnings() const { return totalEarnings; }
};