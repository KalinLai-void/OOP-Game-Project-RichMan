#include "StartTile.hpp"

StartTile::StartTile(const std::string& n, int b)
    : Tile(n), bonus(b) {}

void StartTile::landOn(std::shared_ptr<Player> player) {
    std::cout << player->getName() << " 停在 " << name << "，獲得 $" << bonus << "！" << std::endl;
    player->adjustMoney(bonus);
}
