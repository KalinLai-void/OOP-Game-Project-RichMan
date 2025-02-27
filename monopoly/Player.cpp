#include "Player.hpp"
#include <iostream>

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

void Player::addCard(std::shared_ptr<Card> card) {
    cards.push_back(card);
}

void Player::displayCards(std::vector<std::shared_ptr<Player>>& players, Board& board) {
    if (cards.empty()) {
        std::cout << name << " has no cards." << std::endl;
        return;
    }

    std::cout << std::endl << name << "'s Cards:" << std::endl;
    std::cout << "+-------------------------------------------------+" << std::endl;
    std::cout << "| No. | Card Name  | Effect                       |" << std::endl;
    std::cout << "+-------------------------------------------------+" << std::endl;

    for (size_t i = 0; i < cards.size(); i++) {
        std::cout << "| " << i + 1 << "   | " << cards[i]->getName() << " | " << cards[i]->getEffect() << " |" << std::endl;
    }

    std::cout << "+-------------------------------------------------+" << std::endl;
    std::cout << "[0] to exit" << std::endl << std::endl;
    std::cout << "Enter the number of the card to use: ";

    std::string inputString;
    size_t pos=0;
    int choice=1;
    while (true) {
        std::cin >> choice;

        if (choice < 0 || choice > static_cast<int>(cards.size())) {
            std::cout << "Invalid input. Enter a number between 1 and " << cards.size() << " (or 0 to exit): ";

        } else {
            break;
        }
    }

    if (choice > 0) {
        useCard(choice - 1, players, board); 
    } else {
        std::cout << "Exited without using a card.\n";
    }
}

void Player::useCard(int index, std::vector<std::shared_ptr<Player>>& players, Board& board) {
    if (index >= 0 && index < static_cast<int>(cards.size())) {
        std::cout << "Using " << cards[index]->getName() << "!\n";
        cards[index]->useEffect(players, enable_shared_from_this::shared_from_this(), board); 
        cards.erase(cards.begin() + index);                          
    } else {
        std::cout << "Invalid selection.\n";
    }
}

void Player::setDiceControl(int step) {
    diceControl = step;
}

int Player::getDiceControl() const{
    return diceControl;
}