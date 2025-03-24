#ifndef TILE_HPP
#define TILE_HPP

#include "Game/Player.hpp"
#include <memory>
#include <string>

enum class TileAction { NONE, PURCHASE_PROPERTY, OWN, PAY_TOLL, STORE, SPECIAL_EVENT, TRADE, HOSPITAL, JAIL, START_POINT };

class Tile {
protected:
    std::size_t id;   // Tile ID
    std::string name; // Tile name
    bool blocked;

public:
    Tile(const size_t id, const std::string& n)
        : id(id)
        , name(n)
        , blocked(false) {}
    virtual ~Tile() {}

    std::string getName() const {
        return name;
    }

    virtual std::string getNameWithId() const {
        return std::to_string(id) + " " + name;
    }

    size_t getId() const {
        return id;
    }
    void setBlock(bool b) {
        blocked = b;
    }
    bool isBlocked() const {
        return blocked;
    }

    virtual TileAction landOn(std::shared_ptr<Player> player) = 0;
};

#endif // TILE_HPP
