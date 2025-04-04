﻿// filepath: /c:/Code/monopoly/monopoly/EventTile.cpp
#include "EventTile.hpp"
#include "Game/Player.hpp"
#include "MiniGames/MiniGameManager.hpp"
#include "Bank.hpp"
#include "InputManager.hpp"
#include <ctime>
#include <iostream>
#include <map>
#include <random>

//using namespace std;

EventTile::EventTile(const size_t id, const std::string& n, EventType t, const std::map<std::string, std::pair<int, int>>& valueRange)
    : Tile(id, n)
    , type(t)
    , eventValueRange(valueRange) {}

TileAction EventTile::landOn(std::shared_ptr<Player> player) {
    return TileAction::SPECIAL_EVENT;
}

void EventTile::triggerEvent(std::shared_ptr<Player> player) {
    // Generate a random event
    static std::default_random_engine engine(static_cast<unsigned>(time(nullptr)));
    std::uniform_int_distribution<int> dist(1, 5); // Adjusted to include all cases
    int eventType = dist(engine);
    int value = getRandomValue100Multiple(type);

    if (type == EventType::FATE) {
        switch (eventType) {
        case 1:
            std::cout << "Unexpected Inheritance!\nYou receive an inheritance of $" << value << "." << std::endl;
            player->addMoney(value);
            Bank::getInstance()->payToPlayer(player, value);
            break;
        case 2:
            std::cout << "Medical Expenses!\nPay $" << value << " in hospital fees." << std::endl;
            if (player->getMoney() >= value) {
                Bank::getInstance()->receiveFromPlayer(player, value);
            } else {
                player->setBankrupt(true);
                std::cout << "Not enough money, bankrupt!" << std::endl;
            }
            break;
        case 3:
            std::cout << "Charity Donation!\nDonate $" << value << " to charity." << std::endl;
            if (player->getMoney() >= value) {
                Bank::getInstance()->receiveFromPlayer(player, value);
            } else {
                player->setBankrupt(true);
                std::cout << "Not enough money, bankrupt!" << std::endl;
            }
            break;
        case 4:
            std::cout << "Tax Season!\nPay $" << value << " in taxes to the bank." << std::endl;
            if (player->getMoney() >= value) {
                Bank::getInstance()->receiveFromPlayer(player, value);
            } else {
                player->setBankrupt(true);
                std::cout << "Not enough money, bankrupt!" << std::endl;
            }
            break;
        case 5:
            std::cout << "You have entered the playground!\nGet ready to play a mini-game." << std::endl;
            std::cout << "\nPress any key to continue...";
            InputManager::getKey();
            MiniGameManager::startMiniGame(player);
            break;
        default:
            std::cout << "No event triggered." << std::endl;
            break;
        }
    } else if (type == EventType::CHANCE) {

        switch (eventType) {
        case 1:
            std::cout << "Successful Investment!\nYour investment paid off! Collect $" << value << "." << std::endl;
            Bank::getInstance()->payToPlayer(player, value);
            break;
        case 2:
            std::cout << "Stock Market Crash!\nYour stocks lost value. Pay $" << value << " to the bank." << std::endl;
            if (player->getMoney() >= value) {
                Bank::getInstance()->receiveFromPlayer(player, value);
            } else {
                player->setBankrupt(true);
                std::cout << "Not enough money, bankrupt!" << std::endl;
            }
            break;
        case 3:
            std::cout << "Traffic Violation Fine!\nPay a $" << value << " fine, or roll the dice—if you roll a 6, the fine is waived." << std::endl;
            if (player->rollDice() != 6) {
                if (player->getMoney() >= value) {
                    Bank::getInstance()->receiveFromPlayer(player, value);
                } else {
                    player->setBankrupt(true);
                    std::cout << "Not enough money, bankrupt!" << std::endl;
                }
            } else {
                std::cout << "Lucky! You rolled a 6 and avoided the fine!" << std::endl;
            }
            break;
        case 4:
            std::cout << "Advance to [Start]!\nMove directly to [Start] and collect $" << value << "." << std::endl;
            player->sendToStart(); // todo
            break;
        case 5:
            std::cout << "You have entered the playground!\nGet ready to play a mini-game." << std::endl;
            MiniGameManager::startMiniGame(player);
        default:
            std::cout << "No event triggered." << std::endl;
            break;
        }
    }
    std::cout << "\nPress any key to continue...";
    InputManager::getKey();
}

int EventTile::getRandomValue100Multiple(int min, int max) {
    int multiplier = 100;
    static std::default_random_engine engine(static_cast<unsigned>(time(nullptr)));
    std::uniform_int_distribution<int> dist(min / multiplier, max / multiplier);
    return dist(engine) * multiplier; // Return a random value [500, max]
}

int EventTile::getRandomValue100Multiple(EventType type) {
    switch (type) {
    case EventType::FATE:
        return getRandomValue100Multiple(eventValueRange["FATE"].first, eventValueRange["FATE"].second);
    case EventType::CHANCE:
        return getRandomValue100Multiple(eventValueRange["CHANCE"].first, eventValueRange["CHANCE"].second);
    default:
        std::cerr << "Invalid event type." << std::endl;
        return 0;
    }
}

EventType EventTile::getEventType() {
    return type;
}