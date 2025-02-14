#include "HospitalTile.hpp"
#include <iostream>
using namespace std;

HospitalTile::HospitalTile(const std::string& n)
    : Tile(n) {}

void HospitalTile::landOn(std::shared_ptr<Player> player) {
    cout << "玩家 " << player->getName() << " 抵達 [" << name << "] (醫院)，若為受傷狀態則需要在這裡停留。" << endl;
}
