#include "services/PaymentService.h"
#include <iostream>

using namespace std;

bool PaymentService::pay(Rider& rider, Driver& driver, double amount, const string& method) {
    if (method == "wallet") {
        if (!rider.deductWallet(amount)) {
            cout << "Ví không đủ tiền! Số dư: " << (long)rider.getWallet() << " VNĐ\n";
            return false;
        }
    }
    driver.addEarnings(amount);
    return true;
}
