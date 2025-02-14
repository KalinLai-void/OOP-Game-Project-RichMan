#ifndef PROPERTYTILE_HPP
#define PROPERTYTILE_HPP

#include "Tile.hpp"

// 地產等級
enum class PropertyLevel {
    LEVEL1 = 1, // 房屋
    LEVEL2,     // 大樓
    LEVEL3      // 豪宅
};

class PropertyTile : public Tile {
private:
    long long basePrice;           // 基礎地價
    long long baseToll;            // 1 級過路費
    std::shared_ptr<Player> owner; // 擁有者
    PropertyLevel level;

public:
    PropertyTile(const std::string& n, long long price, long long toll);
    long long getToll() const;
    long long getUpgradeCost() const;
    void landOn(std::shared_ptr<Player> player) override;
};

#endif // PROPERTYTILE_HPP
