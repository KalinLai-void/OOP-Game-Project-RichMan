// filepath: /c:/Code/monopoly/monopoly/EventTile.cpp
#include "EventTile.hpp"
#include "Bank.hpp"
#include "Player.hpp"
#include <ctime>
#include <iostream>
#include <random>

using namespace std;

EventTile::EventTile(const std::string& n)
    : Tile(n) {}

TileAction EventTile::landOn(std::shared_ptr<Player> player) {
    return TileAction::SPECIAL_EVENT;
}

void EventTile::triggerEvent(std::shared_ptr<Player> player) {
    // Generate a random event
    static default_random_engine engine(static_cast<unsigned>(time(nullptr)));
    uniform_int_distribution<int> dist(1, 3);
    int eventType = dist(engine);

    switch (eventType) {
    case 1:
        cout << "Lucky event: Received a bank reward of 5000!" << endl;
        player->adjustMoney(5000);
        Bank::getInstance()->payToPlayer(5000);
        break;
    case 2:
        cout << "Unlucky event: Fined 3000!" << endl;
        if (player->getMoney() >= 3000) {
            player->adjustMoney(-3000);
            Bank::getInstance()->receiveFromPlayer(3000);
        } else {
            player->setBankrupt(true);
            cout << "Not enough money, bankrupt!" << endl;
        }
        break;
    case 3:
        cout << "Accidentally injured, sent to the hospital for 2 turns!" << endl;
        player->sendToHospital(2);
        break;
    default:
        cout << "No event triggered." << endl;
        break;
    }
}
