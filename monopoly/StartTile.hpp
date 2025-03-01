#ifndef STARTTILE_HPP
#define STARTTILE_HPP

#include "Tile.hpp"
#include <iostream>

class StartTile : public Tile {
private:
    int bonus; // The bonus amount players receive when passing or landing on this tile

public:
    // Constructor for `StartTile`, default bonus is $2000
    StartTile(const size_t id, const std::string& n, int b = 2000);

    // Behavior when a player lands on the start tile
    TileAction landOn(std::shared_ptr<Player> player) override;

    // Get the bonus amount
    int getBonus() const;
};

#endif // STARTTILE_HPP
