#ifndef TILE_HPP
#define TILE_HPP

#include "Player.hpp"
#include <memory>
#include <string>

enum class TileAction { NONE, PURCHASE_PROPERTY, OWN, PAY_TOLL, STORE, SPECIAL_EVENT, TRADE, HOSPITAL, JAIL, START };

class Tile {
protected:
    std::string name; // Tile name

public:
    Tile(const std::string& n)
        : name(n) {}
    virtual ~Tile() {}

    std::string getName() const {
        return name;
    }

    virtual TileAction landOn(std::shared_ptr<Player> player) = 0;
};

#endif // TILE_HPP
