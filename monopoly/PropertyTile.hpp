#ifndef PROPERTYTILE_HPP
#define PROPERTYTILE_HPP

#include "Tile.hpp"
#include <memory>
#include <string>

// 假設你已有 PropertyLevel 定義
enum class PropertyLevel { LEVEL1 = 1, LEVEL2 = 2, LEVEL3 = 3 };

class Player; // 前向宣告

class PropertyTile : public Tile {
public:
    PropertyTile(const std::string& n, long long price, long long toll);
    virtual void landOn(std::shared_ptr<Player> player) override;
    long long getToll() const;
    long long getUpgradeCost() const;
    PropertyLevel getPropertyLevel() const;

private:
    void purchase(std::shared_ptr<Player> player);
    void upgrade(std::shared_ptr<Player> player);
    void payToll(std::shared_ptr<Player> player);

    long long basePrice;
    long long baseToll;
    std::shared_ptr<Player> owner;
    PropertyLevel level;
};

#endif // PROPERTYTILE_HPP
