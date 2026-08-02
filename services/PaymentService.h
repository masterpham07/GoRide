#pragma once
#include <string>
#include "models/User.h"

class PaymentService {
public:
    // method: "wallet" hoặc "cash"
    static bool pay(Rider& rider, Driver& driver, double amount, const std::string& method);
};
