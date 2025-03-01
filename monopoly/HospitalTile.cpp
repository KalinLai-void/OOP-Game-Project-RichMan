#include "HospitalTile.hpp"
#include <iostream>
using namespace std;

HospitalTile::HospitalTile(const size_t id, const std::string& n)
    : Tile(id, n) {
    fee = 100;
}

TileAction HospitalTile::landOn(std::shared_ptr<Player> player) {
    handleHospitalChoice(player);
    return TileAction::HOSPITAL;
}

void HospitalTile::handleHospitalChoice(std::shared_ptr<Player> player) {
    if (player->getMoney() >= fee) {
        player->deductMoney(fee);
        cout << "You paid a fee of " << fee << " and try to leave the hospital." << endl;
        // 50% chance to leave the hospital
        if (rand() % 2 == 0) {
            player->recoverFromHospital();
            cout << "You got lucky and left the hospital." << endl;
        } else {
            cout << "You are still in the hospital." << endl;
        }
    } else {
        cout << "You do not have enough money to pay the fee and stay in the hospital." << endl;
    }
}
