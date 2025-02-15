#include "GameConfig.hpp"
#include <iostream>

// 🔥 初始化唯一的 `GameConfig` 實例
GameConfig GameConfig::instance;

GameConfig& GameConfig::getInstance() {
    return instance;
}

void GameConfig::setMode(GameMode newMode) {
    this->mode = newMode;
    loadConfig(); // 重新載入配置
}

GameMode GameConfig::getMode() const {
    return mode;
}

void GameConfig::setPlayersNum(int num) {
    if (num > 0 && num <= 4) { // 假設最多 4 名玩家
        playersNum = num;
    } else {
        std::cerr << "玩家數量不合法，應介於 1 到 4 之間" << std::endl;
    }
}

int GameConfig::getPlayersNum() const {
    return playersNum;
}

void GameConfig::setPlayerNames(const std::vector<std::string>& names) {
    if (names.size() == playersNum) {
        playerNames = names;
    } else {
        std::cerr << "錯誤: 玩家名稱數量與玩家數量不匹配！" << std::endl;
    }
}

std::vector<std::string> GameConfig::getPlayerNames() const {
    return playerNames;
}

void GameConfig::setplayerIcons(const std::vector<std::string>& icons) {
    if (icons.size() == playersNum) {
        playerIcons = icons;
    } else {
        std::cerr << "錯誤: 玩家ICON數量與玩家數量不匹配！" << std::endl;
    }
}
std::vector<std::string> GameConfig::getplayerIcons() const {
    return playerIcons;
}

void GameConfig::setStartMoney(int amount) {
    if (amount >= 0) {
        startMoney = amount;
    } else {
        std::cerr << "錯誤: 起始金額不能為負數！" << std::endl;
    }
}

int GameConfig::getStartMoney() const {
    return startMoney;
}

void GameConfig::setWinMoney(int amount) {
    if (amount >= startMoney) {
        winMoney = amount;
    } else {
        std::cerr << "錯誤: 勝利金額應該大於等於起始金額！" << std::endl;
    }
}

int GameConfig::getWinMoney() const {
    return winMoney;
}

void GameConfig::setPassingStartBonus(int amount) {
    this->passingStartBonus = amount;
}
int GameConfig::getPassingStartBonus() const {
    return passingStartBonus;
}

void GameConfig::setBoardTiles(const std::vector<TileConfig>& tiles) {
    this->boardTiles = tiles;
}

std::vector<TileConfig> GameConfig::getBoardTiles() const {
    return boardTiles; // 返回整個 `boardTiles` 向量
}

// 🔥 直接修改 `this` 指向的 `GameConfig`，刪除錯誤的 `GameConfig config;`
void GameConfig::loadConfig() {
    this->playerIcons = {
        "◆ ", // the space is for better alignment
        "◇ ",
        "■ ",
        "□ ",
    };
    switch (mode) {
    case GameMode::DEBUG:
        this->playersNum = 4;
        this->playerNames = {"Alice", "Bob", "Charlie", "David"};
        this->startMoney = 100000;
        this->winMoney = 300000;
        this->passingStartBonus = 10000;
        this->boardTiles = {
            {   "start",     "起點",     0,    0},
            {"property",     "美國",  2000,  200},
            {   "event",     "命運",     0,    0},
            {"property",     "法國",  3000,  300},
            {   "store", "道具商店",     0,    0},
            {"property",     "日本",  5000,  500},
            {"hospital",     "醫院",     0,    0},
            {   "event",     "機會",     0,    0},
            {"property",     "德國",  8000,  800},
            {"property",     "英國",  8500,  850},
            {   "event",     "命運",     0,    0},
            {"property",   "加拿大",  9000,  900},
            {"property", "澳大利亞",  9500,  950},
            {   "store", "道具商店",     0,    0},
            {"property",   "義大利", 10000, 1000},
            {"hospital",     "醫院",     0,    0},
            {   "event",     "機會",     0,    0},
            {"property",   "西班牙", 11000, 1100},
            {"property",     "中國", 11500, 1150},
            {   "event",     "命運",     0,    0},
            {"property",     "南韓", 12000, 1200},
            {   "store", "道具商店",     0,    0},
            {"property",     "巴西", 12500, 1250},
            {"hospital",     "醫院",     0,    0},
            {"property",   "俄羅斯", 13000, 1300},
            {"property",     "印度", 13500, 1350},
            {   "event",     "機會",     0,    0},
            {"property",     "南非", 14000, 1400}
        };
        break;

    case GameMode::DUEL:
        this->playersNum = 2;
        this->playerNames = {"Player1", "Player2"};
        this->startMoney = 10000;
        this->winMoney = 30000;
        this->passingStartBonus = 1000;
        this->boardTiles = {
            {   "start",     "起點",     0,    0},
            {"property",     "美國",  2000,  200},
            {   "event",     "命運",     0,    0},
            {"property",     "法國",  3000,  300},
            {   "store", "道具商店",     0,    0},
            {"property",     "日本",  5000,  500},
            {"hospital",     "醫院",     0,    0},
            {   "event",     "機會",     0,    0},
            {"property",     "德國",  8000,  800},
            {"property",     "英國",  8500,  850},
            {   "event",     "命運",     0,    0},
            {"property",   "加拿大",  9000,  900},
            {"property", "澳大利亞",  9500,  950},
            {   "store", "道具商店",     0,    0},
            {"property",   "義大利", 10000, 1000},
            {"hospital",     "醫院",     0,    0},
            {   "event",     "機會",     0,    0},
            {"property",   "西班牙", 11000, 1100},
            {"property",     "中國", 11500, 1150},
            {   "event",     "命運",     0,    0},
            {"property",     "南韓", 12000, 1200},
            {   "store", "道具商店",     0,    0},
            {"property",     "巴西", 12500, 1250},
            {"hospital",     "醫院",     0,    0},
            {"property",   "俄羅斯", 13000, 1300},
            {"property",     "印度", 13500, 1350},
            {   "event",     "機會",     0,    0},
            {"property",     "南非", 14000, 1400}
        };
        break;
    }
}
