#ifndef EVENTTILE_HPP
#define EVENTTILE_HPP

#include "Tile.hpp"

class EventTile : public Tile {
public:
    EventTile(const size_t id, const std::string& n);
    TileAction landOn(std::shared_ptr<Player> player) override;
    void triggerEvent(std::shared_ptr<Player> player);
};

#endif // EVENTTILE_HPP
