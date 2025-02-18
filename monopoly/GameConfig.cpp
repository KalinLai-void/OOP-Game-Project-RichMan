#include "GameConfig.hpp"
#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

// Initialize the unique `GameConfig` instance
GameConfig GameConfig::instance;

GameConfig& GameConfig::getInstance() {
    return instance;
}

void GameConfig::loadConfig() {
    std::ifstream file("config.json", std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open config.json" << std::endl;
        return;
    }

    json config;
    file >> config;
    file.close();
  
    try {
        this->playerIcons = config["playerIcons"].get<std::vector<std::string>>();

        std::string modeStr = (mode == GameMode::DEBUG) ? "DEBUG" : "DUEL";
        auto& modeConfig = config["modes"][modeStr];

        this->playersNum = modeConfig["playersNum"].get<int>();
        this->playersName = modeConfig["playersName"].get<std::vector<std::string>>();
        this->startMoney = modeConfig["startMoney"].get<int>();
        this->winMoney = modeConfig["winMoney"].get<int>();
        this->passingStartBonus = modeConfig["passingStartBonus"].get<int>();
        this->mapSize = modeConfig["mapSize"].get<int>();

        this->boardTiles.clear();
        for (const auto& tile : modeConfig["boardTiles"]) {
            this->boardTiles.push_back({tile["type"].get<std::string>(), tile["name"].get<std::string>(), tile["price"].get<int>(),
            tile["toll"].get<int>()});
        }
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
    if (icons.size() == playersNum) {
        playerIcons = icons;
    } else {
        std::cerr << "Error: The number of player icons does not match the number of players!" << std::endl;
    }
}

std::vector<std::string> GameConfig::getPlayerIcons() const {
    return playerIcons;
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

int GameConfig::getMapSize() const {
    return mapSize;
}
