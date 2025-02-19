// PropertyTile.cpp
#include "PropertyTile.hpp"
#include "Bank.hpp"
#include "InputManager.hpp" // 使用統一的按鍵管理
#include "Player.hpp"
#include <iostream>
#include <string>

using namespace std;

PropertyTile::PropertyTile(const std::string& n, long long price, long long toll)
    : Tile(n), basePrice(price), baseToll(toll), owner(nullptr), level(PropertyLevel::LEVEL1) {}

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
    return static_cast<long long>(basePrice * 0.5);
}

PropertyLevel PropertyTile::getPropertyLevel() const {
    return level;
}

void PropertyTile::purchase(std::shared_ptr<Player> player) {
    cout << "玩家 " << player->getName() << " 抵達 [" << name << "] - 無人擁有的地產" << endl;
    cout << "請按 R 購買 (花費 " << basePrice << ")，或按其他鍵跳過: ";
    char key = InputManager::getKey();
    cout << key << endl;
    if (key == 'R' || key == 'r') {
        if (player->getMoney() >= basePrice) {
            player->adjustMoney(-basePrice);
            owner = player;
            cout << "購買成功！已購買 " << name << "，剩餘金錢：" << player->getMoney() << endl;
        } else {
            cout << "金錢不足，無法購買。" << endl;
        }
    } else {
        cout << "玩家選擇不購買。" << endl;
    }
}

void PropertyTile::upgrade(std::shared_ptr<Player> player) {
    cout << "玩家 " << player->getName() << " 抵達自己擁有的 [" << name << "] (等級 " << static_cast<int>(level) << ")" << endl;
    if (level == PropertyLevel::LEVEL3) {
        cout << "已達最高等級，無法繼續升級。" << endl;
        return;
    }
    cout << "請按 R 升級 (花費 " << getUpgradeCost() << ")，或按其他鍵跳過: ";
    char key = InputManager::getKey();
    cout << key << endl;
    if (key == 'R' || key == 'r') {
        if (player->getMoney() >= getUpgradeCost()) {
            player->adjustMoney(-getUpgradeCost());
            if (level == PropertyLevel::LEVEL1)
                level = PropertyLevel::LEVEL2;
            else if (level == PropertyLevel::LEVEL2)
                level = PropertyLevel::LEVEL3;
            cout << "升級成功！目前等級：" << static_cast<int>(level) << "，剩餘金錢：" << player->getMoney() << endl;
        } else {
            cout << "金錢不足，無法升級。" << endl;
        }
    } else {
        cout << "玩家選擇不升級。" << endl;
    }
}

void PropertyTile::payToll(std::shared_ptr<Player> player) {
    long long fee = getToll();
    cout << "玩家 " << player->getName() << " 抵達 [" << name << "]，該房產擁有者為 " << owner->getName() << "，需支付過路費 " << fee << " 元" << endl;
    if (player->getMoney() >= fee) {
        player->adjustMoney(-fee);
        owner->adjustMoney(fee);
        cout << "支付成功！" << endl;
    } else {
        cout << "金錢不足，" << player->getName() << " 破產！" << endl;
        player->setBankrupt(true);
    }
}

void PropertyTile::landOn(std::shared_ptr<Player> player) {
    if (!owner) {
        purchase(player);
    } else {
        if (owner == player) {
            upgrade(player);
        } else {
            payToll(player);
        }
    }
}

// #include "PropertyTile.hpp"
// #include "Bank.hpp"
// #include <iostream>
// #include <string>
// using namespace std;

// PropertyTile::PropertyTile(const std::string& n, long long price, long long toll)
//     : Tile(n), basePrice(price), baseToll(toll), owner(nullptr), level(PropertyLevel::LEVEL1) {}

// long long PropertyTile::getToll() const {
//     switch (level) {
//     case PropertyLevel::LEVEL1:
//         return baseToll;
//     case PropertyLevel::LEVEL2:
//         return baseToll * 2;
//     case PropertyLevel::LEVEL3:
//         return baseToll * 3;
//     }
//     return baseToll;
// }

// long long PropertyTile::getUpgradeCost() const {
//     return static_cast<long long>(basePrice * 0.5);
// }

// void PropertyTile::landOn(std::shared_ptr<Player> player) {
//     if (!owner) {
//         cout << "玩家 " << player->getName() << " 抵達 [" << name << "] - 無人擁有的地產，是否花費 " << basePrice << " 購買？(y/n) ";
//         char c;
//         cin >> c;
//         if (c == 'y' || c == 'Y') {
//             if (player->getMoney() >= basePrice) {
//                 player->adjustMoney(-basePrice);
//                 owner = player;
//                 cout << "已購買 " << name << "，剩餘金錢：" << player->getMoney() << endl;
//             } else {
//                 cout << "金錢不足，無法購買。" << endl;
//             }
//         } else {
//             cout << "玩家選擇不購買。" << endl;
//         }
//     } else {
//         if (owner == player) {
//             cout << "玩家 " << player->getName() << " 抵達自己擁有的 [" << name << "]。" << endl;
//             if (level != PropertyLevel::LEVEL3) {
//                 cout << "是否升級 (花費 " << getUpgradeCost() << " )？(y/n) ";
//                 char c;
//                 cin >> c;
//                 if (c == 'y' || c == 'Y') {
//                     if (player->getMoney() >= getUpgradeCost()) {
//                         player->adjustMoney(-getUpgradeCost());
//                         if (level == PropertyLevel::LEVEL1) {
//                             level = PropertyLevel::LEVEL2;
//                         } else if (level == PropertyLevel::LEVEL2) {
//                             level = PropertyLevel::LEVEL3;
//                         }
//                         cout << "升級成功！目前等級：" << static_cast<int>(level) << "，剩餘金錢：" << player->getMoney() << endl;
//                     } else {
//                         cout << "金錢不足，無法升級。" << endl;
//                     }
//                 }
//             } else {
//                 cout << "已達最高等級，無法繼續升級。" << endl;
//             }
//         } else {
//             long long fee = getToll();
//             cout << "玩家 " << player->getName() << " 抵達 [" << name << "] - 擁有者：" << owner->getName() << "，需支付過路費 " << fee << endl;
//             if (player->getMoney() >= fee) {
//                 player->adjustMoney(-fee);
//                 owner->adjustMoney(fee);
//             } else {
//                 cout << "金錢不足，" << player->getName() << " 破產！" << endl;
//                 player->setBankrupt(true);
//             }
//         }
//     }
// }
