#include "Player.hpp"
#include "MiniGameManager.hpp"
#include "iostream"

Player::Player(const std::string& n, const PlayerIcon& i, long long m)
    : name(n), icon(i), money(m), position(0), bankrupt(false), inHospital(false), hospitalRoundLeft(0) {}

std::string Player::getName() const {
    return name;
}

std::string Player::getIcon() const {
    return icon.icon;
}

std::string Player::getIconWithColor() const {
    std::string iconWithColor = icon.color + icon.icon + "\033[0m";
    return iconWithColor;
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

void Player::addCard(std::shared_ptr<Card> card) {
    cards.push_back(card);
}

void Player::startMiniGame(MiniGameManager& manager) {
    manager.startMiniGame(shared_from_this());
}

void Player::endMiniGame(MiniGameManager& manager) {
    manager.endMiniGame(shared_from_this());
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
    std::cout << "Enter the number of the card to use (or 0 to exit): ";

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