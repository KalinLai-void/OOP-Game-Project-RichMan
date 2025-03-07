#include "StartTile.hpp"

StartTile::StartTile(const size_t id, const std::string& n, int b)
    : Tile(id, n)
    , bonus(b) {}

TileAction StartTile::landOn(std::shared_ptr<Player> player) {
    player->addMoney(bonus);
    return TileAction::START_POINT;
}

int StartTile::getBonus() const {
    return bonus;
}
