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

void Bank::payToPlayer(std::shared_ptr<Player> player, long long amount) {
    totalMoneyInBank -= amount;
    player->addMoney(amount);
}
void Bank::receiveFromPlayer(std::shared_ptr<Player> player, long long amount) {
    totalMoneyInBank += amount;
    player->deductMoney(amount);
}

long long Bank::getTotalMoneyInBank() const {
    return totalMoneyInBank;
}
