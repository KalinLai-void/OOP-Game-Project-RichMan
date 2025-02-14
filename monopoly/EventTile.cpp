#include "EventTile.hpp"
#include "Bank.hpp"
#include <ctime>
#include <iostream>
#include <random>
using namespace std;

EventTile::EventTile(const std::string& n)
    : Tile(n) {}

void EventTile::landOn(std::shared_ptr<Player> player) {
    cout << "玩家 " << player->getName() << " 停在事件格 [" << name << "]，觸發隨機事件！" << endl;
    // 透過隨機產生事件
    static default_random_engine engine(static_cast<unsigned>(time(nullptr)));
    uniform_int_distribution<int> dist(1, 3);
    int eventType = dist(engine);
    switch (eventType) {
    case 1:
        cout << "幸運事件：獲得銀行獎勵 5000！" << endl;
        player->adjustMoney(5000);
        Bank::getInstance()->payToPlayer(5000);
        break;
    case 2:
        cout << "倒楣事件：被罰款 3000！" << endl;
        if (player->getMoney() >= 3000) {
            player->adjustMoney(-3000);
            Bank::getInstance()->receiveFromPlayer(3000);
        } else {
            player->setBankrupt(true);
            cout << "金錢不足，破產！" << endl;
        }
        break;
    case 3:
        cout << "意外受傷，送往醫院，需住院 2 回合！" << endl;
        player->sendToHospital(2);
        break;
    }
}
