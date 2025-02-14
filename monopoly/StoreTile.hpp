#ifndef STORETILE_HPP
#define STORETILE_HPP

#include "Tile.hpp"

class StoreTile : public Tile {
public:
    StoreTile(const std::string& n);
    void landOn(std::shared_ptr<Player> player) override;
};

#endif // STORETILE_HPP
