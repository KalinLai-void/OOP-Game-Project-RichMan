#ifndef PROPERTYTILE_HPP
#define PROPERTYTILE_HPP

#include "Bank.hpp"
#include "Tile.hpp"
#include <iostream>
#include <string>

enum class PropertyLevel {EMPTY, LEVEL1, LEVEL2, LEVEL3 };

class PropertyTile : public Tile {
private:
    long long basePrice;
    long long baseToll;
    long long currentPrice;
    std::shared_ptr<Player> owner;
    PropertyLevel level;

public:
    PropertyTile(const std::string& n, long long price, long long toll);

    long long getToll() const;
    long long getUpgradeCost() const;
    PropertyLevel getPropertyLevel() const;
    long long getCurrentPrice() const;

    TileAction landOn(std::shared_ptr<Player> player) override;

    void purchase(std::shared_ptr<Player> player);
    void upgrade(std::shared_ptr<Player> player);
    void downgrade();
    void payToll(std::shared_ptr<Player> player);
    void sell(std::shared_ptr<Player> player);

    void updateCurrentPrice();
    std::shared_ptr<Player> getPropertyOwner() const;
};

#endif // PROPERTYTILE_HPP
