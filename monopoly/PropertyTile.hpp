// #ifndef PROPERTYTILE_HPP
// #define PROPERTYTILE_HPP

// #include "Tile.hpp"

// // 地產等級
// enum class PropertyLevel {
//     LEVEL1 = 1, // 房屋
//     LEVEL2,     // 大樓
//     LEVEL3      // 豪宅
// };

// class PropertyTile : public Tile {
// private:
//     long long basePrice;           // 基礎地價
//     long long baseToll;            // 1 級過路費
//     std::shared_ptr<Player> owner; // 擁有者
//     PropertyLevel level;

// public:
//     PropertyTile(const std::string& n, long long price, long long toll);
//     long long getToll() const;
//     long long getUpgradeCost() const;
//     void landOn(std::shared_ptr<Player> player) override;
// };

// #endif // PROPERTYTILE_HPP
// PropertyTile.hpp
#ifndef PROPERTYTILE_HPP
#define PROPERTYTILE_HPP

#include "Tile.hpp"
#include <memory>
#include <string>

// 假設你已有 PropertyLevel 定義
enum class PropertyLevel { LEVEL1 = 1, LEVEL2, LEVEL3 };

class Player; // 前向宣告

class PropertyTile : public Tile {
public:
    PropertyTile(const std::string& n, long long price, long long toll);
    virtual void landOn(std::shared_ptr<Player> player) override;
    long long getToll() const;
    long long getUpgradeCost() const;

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
