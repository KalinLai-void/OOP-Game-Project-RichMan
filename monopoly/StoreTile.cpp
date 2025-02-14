#include "StoreTile.hpp"
#include "Bank.hpp"
#include <iostream>
using namespace std;

StoreTile::StoreTile(const std::string& n)
    : Tile(n) {}

void StoreTile::landOn(std::shared_ptr<Player> player) {
    cout << "玩家 " << player->getName() << " 抵達 [" << name << "] (道具商店)，可在此購買道具(僅示範)。" << endl;
    cout << "可購買 [控骰卡]，價格 2000。是否購買？(y/n) ";
    char c;
    cin >> c;
    if (c == 'y' || c == 'Y') {
        if (player->getMoney() >= 2000) {
            player->adjustMoney(-2000);
            Bank::getInstance()->receiveFromPlayer(2000);
            cout << "購買成功！(目前尚未實作道具管理)" << endl;
        } else {
            cout << "金錢不足，無法購買。" << endl;
        }
    }
}
