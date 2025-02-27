#include "Player.hpp"
#include "MiniGameManager.hpp"

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
bool Player::adjustMoney(long long delta) {
    if (money + delta < 0) {
        return false;
    }
    money += delta;
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
