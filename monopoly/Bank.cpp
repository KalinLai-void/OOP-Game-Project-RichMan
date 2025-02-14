#include "Bank.hpp"

Bank* Bank::instance = nullptr;

Bank::Bank()
    : totalMoneyInBank(1000000000LL) {}

Bank* Bank::getInstance() {
    if (!instance) {
        instance = new Bank();
    }
    return instance;
}

void Bank::payToPlayer(long long amount) {
    totalMoneyInBank -= amount;
}

void Bank::receiveFromPlayer(long long amount) {
    totalMoneyInBank += amount;
}

long long Bank::getTotalMoneyInBank() const {
    return totalMoneyInBank;
}
