﻿#ifndef STORETILE_HPP
#define STORETILE_HPP

#include "CardStore.hpp"
#include "Tile.hpp"

class StoreTile : public Tile {
private:
    CardStore* store;

public:
    StoreTile(const size_t id, const std::string& n, const GameConfig& config);
    CardStore* getStore();
    TileAction landOn(std::shared_ptr<Player> player) override;
    void enterStore(std::shared_ptr<Player> player);
};

#endif // STORETILE_HPP
