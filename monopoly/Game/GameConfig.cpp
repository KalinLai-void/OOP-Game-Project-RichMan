﻿#include "GameConfig.hpp"
#include "Utils/Utils.hpp"
#include <fstream>
#include <iostream>

// Initialize the unique `GameConfig` instance
GameConfig GameConfig::instance;

GameConfig& GameConfig::getInstance() {
    return instance;
}

void GameConfig::loadConfig() {
    std::ifstream file("json/config.json", std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open config.json" << std::endl;
        return;
    }

    nlohmann::json config;
    file >> config;
    file.close();

    try {
        this->playerIcons = config["playerIcons"].get<std::vector<std::string>>();
        this->playerColors = config["playerColors"].get<std::vector<std::string>>();
        this->tileWidth = config["tileWidth"].get<int>();
        this->propertyLevelIcons = config["propertyLevelIcons"].get<std::vector<std::string>>();
        this->animation = config["animation"].get<bool>();
        this->animationTime = config["animationTime"].get<long long>();
        this->mapSize = config["mapSize"].get<int>();
        this->boardTiles.clear();
        std::size_t id = 0;
        for (const auto& tile : config["boardTiles"]) {
            this->boardTiles.push_back(
                {id++, tile["type"].get<std::string>(), tile["name"].get<std::string>(), tile["price"].get<int>(), tile["toll"].get<int>()});
        }
        for (const auto& card : config["cards"]) {
            this->cards.push_back(
                {card["name"].get<std::string>(), card["icon"].get<std::string>(), card["price"].get<int>(), card["effect"].get<std::string>()});
        }
        this->eventValueRange.clear();
        for (const auto& event : config["eventValueRange"].items()) {
            this->eventValueRange[event.key()] = {event.value()[0].get<int>(), event.value()[1].get<int>()};
        }
        // -----------------------------
        // Mode Specific Configurations
        // ----------------------------
        std::string modeStr = (mode == GameMode::DEBUG) ? "DEBUG" : "DUEL";
        auto& modeConfig = config["modes"][modeStr];

        this->playersNum = modeConfig["playersNum"].get<int>();
        this->playersName = modeConfig["playersName"].get<std::vector<std::string>>();
        this->startMoney = modeConfig["startMoney"].get<int>();
        this->winMoney = modeConfig["winMoney"].get<int>();
        this->passingStartBonus = modeConfig["passingStartBonus"].get<int>();

        // 設定 Windows Console 視窗大小 # TODO: but now failure
        //int consoleWidth = 1000;
        //int consoleHeight = 1000;
        //setConsoleSize(consoleWidth, consoleHeight);
    } catch (const std::exception& e) {
        std::cerr << "Error: Failed to load configuration - " << e.what() << std::endl;
    }
}

void GameConfig::setMode(GameMode newMode) {
    this->mode = newMode;
    loadConfig(); // Reload configuration
}

GameMode GameConfig::getMode() const {
    return mode;
}

void GameConfig::setPlayersNum(int num) {
    if (num > 0 && num <= 4) { // Assume a maximum of 4 players
        playersNum = num;
    } else {
        std::cerr << "Invalid number of players. It should be between 1 and 4." << std::endl;
    }
}

int GameConfig::getPlayersNum() const {
    return playersNum;
}

void GameConfig::setPlayersName(const std::vector<std::string>& names) {
    if (names.size() == playersNum) {
        playersName = names;
    } else {
        std::cerr << "Error: The number of player names does not match the number of players!" << std::endl;
    }
}

std::vector<std::string> GameConfig::getPlayerNames() const {
    return playersName;
}

void GameConfig::setPlayerIcons(const std::vector<std::string>& icons) {
    for (int i = 0; i < playersNum; ++i) {
        playerIcons.push_back(icons[i]);
    }
}

std::vector<std::string> GameConfig::getPlayerIcons() const {
    return playerIcons;
}

void GameConfig::setPlayerColors(const std::vector<std::string>& icons) {
    for (int i = 0; i < playersNum; ++i) {
        playerColors.push_back(icons[i]);
    }
}

std::vector<std::string> GameConfig::getPlayerColors() const {
    return playerColors;
}

void GameConfig::setPropertyLevelIcons(const std::vector<std::string>& icons) {
    propertyLevelIcons = icons;
}

std::vector<std::string> GameConfig::getPropertyLevelIcons() const {
    return propertyLevelIcons;
}

std::map<int, std::string> GameConfig::getLocationMap() const {
    return locationMap;
}

void GameConfig::setStartMoney(int amount) {
    if (amount >= 0) {
        startMoney = amount;
    } else {
        std::cerr << "Error: Starting money cannot be negative!" << std::endl;
    }
}

int GameConfig::getStartMoney() const {
    return startMoney;
}

void GameConfig::setWinMoney(int amount) {
    if (amount >= startMoney) {
        winMoney = amount;
    } else {
        std::cerr << "Error: Winning money should be greater than or equal to the starting money!" << std::endl;
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
    return boardTiles;
}

void GameConfig::setCards(const std::vector<CardConfig>& cards) {
    this->cards = cards;
}

std::vector<CardConfig> GameConfig::getCards() const {
    return cards;
}

void GameConfig::setEventValueRange(const std::map<std::string, std::pair<int, int>>& range) {
    this->eventValueRange = range;
}

std::map<std::string, std::pair<int, int>> GameConfig::getEventValueRange() const {
    return eventValueRange;
}

void GameConfig::setAnimation(bool status) {
    this->animation = status;
}

bool GameConfig::getAnimation() const {
    return animation;
}

int GameConfig::getMapSize() const {
    return mapSize;
}

int GameConfig::getTileWidth() const {
    return tileWidth;
}

long long GameConfig::getAnimationTime() const {
    return animationTime;
}
