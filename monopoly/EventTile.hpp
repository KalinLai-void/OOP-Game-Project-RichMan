#ifndef EVENTTILE_HPP
#define EVENTTILE_HPP

#include "Tile.hpp"

class EventTile : public Tile {
public:
    EventTile(const std::string& n);
    void landOn(std::shared_ptr<Player> player) override;
};

#endif // EVENTTILE_HPP
