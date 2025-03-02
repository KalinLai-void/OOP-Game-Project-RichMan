// filepath: /c:/Code/monopoly/monopoly/EventTile.cpp
#include "EventTile.hpp"
#include "Bank.hpp"
#include "MiniGameManager.hpp"
#include "Player.hpp"
#include <ctime>
#include <iostream>
#include <map>
#include <random>

using namespace std;

EventTile::EventTile(const size_t id, const std::string& n, EventType t, const std::map<std::string, std::pair<int, int>>& valueRange)
    : Tile(id, n), type(t), eventValueRange(valueRange) {}

TileAction EventTile::landOn(std::shared_ptr<Player> player) {
    return TileAction::SPECIAL_EVENT;
}

void EventTile::triggerEvent(std::shared_ptr<Player> player) {
    // Generate a random event
    static default_random_engine engine(static_cast<unsigned>(time(nullptr)));
    uniform_int_distribution<int> dist(1, 5); // Adjusted to include all cases
    int eventType = dist(engine);
    int value = getRandomValue100Multiple(type);

    if (type == EventType::FATE) {
        switch (eventType) {
        case 1:
            cout << "Unexpected Inheritance!\nYou receive an inheritance of $" << value << "." << endl;
            player->addMoney(value);
            Bank::getInstance()->payToPlayer(player, value);
            break;
        case 2:
            cout << "Medical Expenses!\nPay $" << value << " in hospital fees." << endl;
            if (player->getMoney() >= value) {
                Bank::getInstance()->receiveFromPlayer(player, value);
            } else {
                player->setBankrupt(true);
                cout << "Not enough money, bankrupt!" << endl;
            }
            break;
        case 3:
            cout << "Charity Donation!\nDonate $" << value << " to charity." << endl;
            if (player->getMoney() >= value) {
                Bank::getInstance()->receiveFromPlayer(player, value);
            } else {
                player->setBankrupt(true);
                cout << "Not enough money, bankrupt!" << endl;
            }
            break;
        case 4:
            cout << "Tax Season!\nPay $" << value << " in taxes to the bank." << endl;
            if (player->getMoney() >= value) {
                Bank::getInstance()->receiveFromPlayer(player, value);
            } else {
                player->setBankrupt(true);
                cout << "Not enough money, bankrupt!" << endl;
            }
            break;
        case 5:
            cout << "You have entered the playground!\nGet ready to play a mini-game." << endl;
            MiniGameManager::startMiniGame(player);
            break;
        default:
            cout << "No event triggered." << endl;
            break;
        }
    } else if (type == EventType::CHANCE) {

        switch (eventType) {
        case 1:
            cout << "Successful Investment!\nYour investment paid off! Collect $" << value << "." << endl;
            Bank::getInstance()->payToPlayer(player, value);
            break;
        case 2:
            cout << "Stock Market Crash!\nYour stocks lost value. Pay $" << value << " to the bank." << endl;
            if (player->getMoney() >= value) {
                Bank::getInstance()->receiveFromPlayer(player, value);
            } else {
                player->setBankrupt(true);
                cout << "Not enough money, bankrupt!" << endl;
            }
            break;
        case 3:
            cout << "Traffic Violation Fine!\nPay a $" << value << " fine, or roll the dice—if you roll a 6, the fine is waived." << endl;
            if (player->rollDice() != 6) {
                if (player->getMoney() >= value) {
                    Bank::getInstance()->receiveFromPlayer(player, value);
                } else {
                    player->setBankrupt(true);
                    cout << "Not enough money, bankrupt!" << endl;
                }
            } else {
                cout << "Lucky! You rolled a 6 and avoided the fine!" << endl;
            }
            break;
        case 4:
            cout << "Advance to [Start]!\nMove directly to [Start] and collect $" << value << "." << endl;
            player->sendToStart(); // todo
            break;
        case 5:
            cout << "You have entered the playground!\nGet ready to play a mini-game." << endl;
            MiniGameManager::startMiniGame(player);
        default:
            cout << "No event triggered." << endl;
            break;
        }
    }
}

int EventTile::getRandomValue100Multiple(int min, int max) {
    int multiplier = 100;
    static default_random_engine engine(static_cast<unsigned>(time(nullptr)));
    uniform_int_distribution<int> dist(min / multiplier, max / multiplier);
    return dist(engine) * multiplier; // Return a random value [500, max]
}

int EventTile::getRandomValue100Multiple(EventType type) {
    switch (type) {
    case EventType::FATE:
        return getRandomValue100Multiple(eventValueRange["FATE"].first, eventValueRange["FATE"].second);
    case EventType::CHANCE:
        return getRandomValue100Multiple(eventValueRange["CHANCE"].first, eventValueRange["CHANCE"].second);
    }
}

EventType EventTile::getEventType() {
    return type;
}