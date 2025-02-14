#include "GameConfig.hpp"

// 🔥 初始化唯一的 `GameConfig` 實例
GameConfig GameConfig::instance;

GameConfig& GameConfig::getInstance() {
    return instance;
}

void GameConfig::setMode(GameMode newMode) {
    this->mode = newMode;
    loadConfig();
}
GameMode GameConfig::getMode() const {
    return mode;
}

void GameConfig::loadConfig() {
    GameConfig config;
    switch (mode) {
    case GameMode::DEBUG:
        players = 4;
        player_names = {"Alice", "Bob", "Charlie", "David"};
        player_icons = {"Red", "Blue", "Green", "Yellow"};
        start_money = 100000;
        win_money = 1000000;
        boardTiles = {
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
        players = 2;
        player_names = {"Player1", "Player2"};
        player_icons = {"Black", "White"};
        start_money = 10000;
        win_money = 100000;
        boardTiles = {
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
