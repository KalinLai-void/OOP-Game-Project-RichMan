#include "StartTile.hpp"

StartTile::StartTile(const std::string& n, int b)
    : Tile(n), bonus(b) {}

TileAction StartTile::landOn(std::shared_ptr<Player> player) {
    player->addMoney(bonus);
    return TileAction::START;
}

int StartTile::getBonus() const {
    return bonus;
}
