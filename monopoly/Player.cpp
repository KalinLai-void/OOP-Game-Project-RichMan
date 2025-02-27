#include "Player.hpp"
#include "MiniGameManager.hpp"
#include "iostream"
Player::Player(const std::string& n, const std::string& i, long long m)
    : name(n), icon(i), money(m), position(0), bankrupt(false), inHospital(false), hospitalRoundLeft(0) {}

std::string Player::getName() const {
    return name;
}
std::string Player::getIcon() const {
    return icon;
}
long long Player::getMoney() const {
    return money;
}
int Player::getPosition() const {
    return position;
}
bool Player::isBankrupt() const {
    return bankrupt;
}
bool Player::isInHospital() const {
    return inHospital;
}
void Player::setPosition(int pos) {
    position = pos;
}
/**
 * Adds a specified amount to the player's balance.
 *
 * @param amount The amount of money to be added.
 *               For example, if amount = 100, then the player's balance will be increased by 100 (balance + amount).
 * @return Returns true if the addition is successful; returns false if the amount is negative
 */
bool Player::addMoney(long long amount) {
    if (amount < 0) {
        std::cerr << "Error: addMoney() should not be called with a negative amount. Received: " << amount << std::endl;
        return false;
    }
    money += amount;
    return true;
}

/**
 * Deducts a specified amount from the player's balance.
 *
 * @param amount The amount of money to be deducted.
 *               For example, if amount = 100, then the player's balance will be reduced by 100 (balance - amount).
 * @return Returns true if the deduction is successful;
 *         returns false if the player's balance is insufficient or if the amount is negative
 */
bool Player::deductMoney(long long amount) {
    if (amount < 0) {
        std::cerr << "Error: deductMoney() should not be called with a negative amount. Received: " << amount << std::endl;
        return false;
    }
    if (money - amount < 0) {
        std::cerr << "Error: deductMoney() should not be called with an amount greater than the player's money. Received: " << amount << std::endl;
        return false;
    }
    money -= amount;
    return true;
}

void Player::setBankrupt(bool b) {
    bankrupt = b;
}

void Player::sendToHospital(int rounds) {
    inHospital = true;
    hospitalRoundLeft = rounds;
}

void Player::recoverFromHospital() {
    inHospital = false;
    hospitalRoundLeft = 0;
}

void Player::updateHospitalStatus() {
    if (inHospital) {
        hospitalRoundLeft--;
        if (hospitalRoundLeft <= 0) {
            recoverFromHospital();
        }
    }
}

void Player::addCard(const Card& card) {
    cards.push_back(card);
}

void Player::startMiniGame(MiniGameManager& manager) {
    manager.startMiniGame(shared_from_this());
}

void Player::endMiniGame(MiniGameManager& manager) {
    manager.endMiniGame(shared_from_this());
}
