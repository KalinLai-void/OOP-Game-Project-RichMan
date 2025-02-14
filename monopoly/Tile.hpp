#ifndef TILE_HPP
#define TILE_HPP

#include "Player.hpp"
#include <memory>
#include <string>

class Tile {
protected:
    std::string name; // 格子名稱
public:
    Tile(const std::string& n)
        : name(n) {}
    virtual ~Tile() {}
    std::string getName() const {
        return name;
    }

    // 當玩家停留在此格子上觸發的行為
    virtual void landOn(std::shared_ptr<Player> player) = 0;
};

#endif // TILE_HPP
