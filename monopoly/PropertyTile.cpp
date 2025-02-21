// PropertyTile.cpp
#include "PropertyTile.hpp"
#include "Bank.hpp"
#include "InputManager.hpp" // 使用統一的按鍵管理
#include "Player.hpp"
#include <iostream>
#include <string>

using namespace std;

PropertyTile::PropertyTile(const std::string& n, long long price, long long toll)
    : Tile(n), basePrice(price), baseToll(toll), owner(nullptr), level(PropertyLevel::EMPTY), currentPrice(price) {}

long long PropertyTile::getToll() const {
    switch (level) {
    case PropertyLevel::LEVEL1:
        return baseToll;
    case PropertyLevel::LEVEL2:
        return baseToll * 2;
    case PropertyLevel::LEVEL3:
        return baseToll * 3;
    }
    return baseToll;
}

long long PropertyTile::getUpgradeCost() const {
    return static_cast<long long>(currentPrice * 0.5);
}

PropertyLevel PropertyTile::getPropertyLevel() const {
    return level;
}

void PropertyTile::purchase(std::shared_ptr<Player> player) {
    if (player->getMoney() >= currentPrice) {
        player->adjustMoney(-currentPrice);
        owner = player;
        level = PropertyLevel::LEVEL1;
        cout << "Purchase successful! Bought " << name << ", remaining money: " << player->getMoney() << endl;
    } else {
        cout << "Not enough money to purchase." << endl;
    }
}

void PropertyTile::upgrade(std::shared_ptr<Player> player) {
    if (level == PropertyLevel::LEVEL3) {
        cout << "Already at the highest level, cannot upgrade further." << endl;
        return;
    }
    if (player->getMoney() >= getUpgradeCost()) {
        player->adjustMoney(-getUpgradeCost());
        if (level == PropertyLevel::LEVEL1)
            level = PropertyLevel::LEVEL2;
        else if (level == PropertyLevel::LEVEL2)
            level = PropertyLevel::LEVEL3;
        updateCurrentPrice();
        cout << "Upgrade successful! Current Property level: " << static_cast<int>(level) << ", remaining money: " << player->getMoney() << endl;
    } else {
        cout << "Not enough money to upgrade." << endl;
    }
}

void PropertyTile::payToll(std::shared_ptr<Player> player) {
    long long fee = getToll();
    if (player->getMoney() >= fee) {
        player->adjustMoney(-fee);
        owner->adjustMoney(fee);
        cout << "Payment successful! Paid " << fee << " dollars." << endl;
    } else {
        cout << "Not enough money, " << player->getName() << " is bankrupt!" << endl;
        player->setBankrupt(true);
    }
}

void PropertyTile::sell(std::shared_ptr<Player> player) {
    if (owner != player) {
        cout << "You do not own this property, so you cannot sell it." << endl;
        return;
    }

    long long sellPrice = currentPrice;
    player->adjustMoney(sellPrice);
    owner = nullptr;
    level = PropertyLevel::LEVEL1;
    updateCurrentPrice();
    cout << "Sell successful! Sold " << name << " for " << sellPrice << ", current money: " << player->getMoney() << endl;
}

TileAction PropertyTile::landOn(std::shared_ptr<Player> player) {
    if (!owner) {
        return TileAction::PURCHASE_PROPERTY;
    } else if (owner == player) {
        return TileAction::OWN;
    } else {
        return TileAction::PAY_TOLL; // No further action needed
    }
}

long long PropertyTile::getCurrentPrice() const {
    return currentPrice;
}

void PropertyTile::updateCurrentPrice() {
    switch (level) {
    case PropertyLevel::LEVEL1:
        currentPrice = basePrice;
        break;
    case PropertyLevel::LEVEL2:
        currentPrice = basePrice * 2;
        break;
    case PropertyLevel::LEVEL3:
        currentPrice = basePrice * 3;
        break;
    default:
        currentPrice = basePrice;
        break;
    }
}

std::shared_ptr<Player> PropertyTile::getPropertyOwner() const {
    return owner;
}