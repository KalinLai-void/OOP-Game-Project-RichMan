#include "Player.hpp"
#include "Card.hpp"
#include "MiniGameManager.hpp"
#include "iomanip"
#include "iostream"

Player::Player(const std::string& n, const std::string& i, const std::string& c, long long m)
    : name(n)
    , icon(i)
    , color(c)
    , money(m)
    , position(0)
    , bankrupt(false)
    , inHospital(false)
    , hospitalRoundLeft(0)
    , diceControl(0) {}

std::string Player::getName() const {
    return name;
}

std::string Player::getIcon() const {
    return icon;
}

std::string Player::getColor() const {
    return color;
}

std::string Player::getIconWithColor() const {
    std::string iconWithColor = color + icon + "\033[0m";
    return iconWithColor;
}

std::string Player::getDisplayName() const {
    return getIconWithColor() + " " + getName();
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

void Player::sendToStart() {
    position = 0;
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

void Player::startMiniGame() {
    MiniGameManager::startMiniGame(shared_from_this());
}
void Player::endMiniGame() {
    MiniGameManager::endMiniGame(shared_from_this());
}

std::vector<std::shared_ptr<Card>> Player::getCards() {
    return cards;
}

void Player::displayCards(std::vector<std::shared_ptr<Player>>& players) {
    if (cards.empty()) {
        std::cout << name << " has no cards." << std::endl;
        return;
    }

    std::vector<int> colWidth = {5, 23, 45};
    std::cout << std::endl << name << "'s Cards:" << std::endl;

    // Print top border
    std::cout << "+" << std::setfill('-') << std::setw(colWidth[0] + 1) << '-' << std::setw(colWidth[1] + 1) << '-' << std::setw(colWidth[2] + 1) << '-' << "+"
              << std::setfill(' ') << std::endl;

    // Print column headers
    std::cout << "| " << std::left << std::setw(colWidth[0] - 1) << "No."
              << "| " << std::setw(colWidth[1] - 1) << "Card Name"
              << "| " << std::setw(colWidth[2] - 1) << "Effect" << " |" << std::endl;

    // Print separator below headers
    std::cout << "+" << std::setfill('-') << std::setw(colWidth[0] + 1) << '-' << std::setw(colWidth[1] + 1) << '-' << std::setw(colWidth[2] + 1) << '-' << "+"
              << std::setfill(' ') << std::endl;

    // Print each card in table format
    for (size_t i = 0; i < cards.size(); i++) {
        std::cout << "| " << std::left << std::setw(colWidth[0] - 1) << i + 1 << "| " << std::setw(colWidth[1] - 1) << cards[i]->getName() << "| "
                  << std::setw(colWidth[2] - 1) << cards[i]->getEffect() << " |" << std::endl;
    }

    // Print bottom border
    std::cout << "+" << std::setfill('-') << std::setw(colWidth[0] + 1) << '-' << std::setw(colWidth[1] + 1) << '-' << std::setw(colWidth[2] + 1) << '-' << "+"
              << std::setfill(' ') << std::endl;

    std::cout << "Enter the number of the card to use (or 0 to exit): ";
    int choice;
    while (true) {

        std::string input;
        try {
            std::cin >> input;
            size_t pos;
            choice = std::stoi(input, &pos); // pos will be set to the position of the first character in the input that is not part of the number
            if (pos != input.size() || choice < 1 || choice > static_cast<int>(cards.size())) {
                if (choice == 0) {
                    return;
                }
                throw std::invalid_argument("Invalid input");
            }
            break;
        } catch (std::exception&) {
            std::cout << "Invalid input. Enter a number between 1 and " << cards.size() << " (or 0 to exit): ";
        }
    }

    if (choice > 0) {
        useCard(choice - 1, players);
    } else {
        std::cout << "Exited without using a card.\n";
    }
}

void Player::useCard(int index, std::vector<std::shared_ptr<Player>>& players) {
    if (index >= 0 && index < static_cast<int>(cards.size())) {
        std::cout << "Using " << cards[index]->getName() << "!\n";
        cards[index]->useEffect(players, enable_shared_from_this::shared_from_this());
        cards.erase(cards.begin() + index);
    } else {
        std::cout << "Invalid selection.\n";
    }
}

void Player::setDiceControl(int step) {
    diceControl = step;
}

int Player::getDiceControl() const {
    return diceControl;
}

int Player::rollDice() {
    if (diceControl != 0) {
        int result = diceControl;
        diceControl = 0; // Reset dice control after use
        return result;
    } else {
        return rand() % 6 + 1; // Roll a standard 6-sided die
    }
}
