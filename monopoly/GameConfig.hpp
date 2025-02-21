#ifndef GAMECONFIG_HPP
#define GAMECONFIG_HPP

#include "Utils.hpp"
#include <string>
#include <vector>
// 定義遊戲模式
enum class GameMode { DEBUG, DUEL, RELEASE, CUSTOM };

struct TileConfig {
    std::string type;
    std::string name;
    int cost;
    int rent;
};

class GameConfig {
private:
    static GameConfig instance;
    GameMode mode = GameMode::DEBUG;
    int playersNum = 2;
    int startMoney = 0;
    int winMoney = 0;
    int passingStartBonus = 0;
    int mapSize = 8;
    int tileWidth = 12;
    long long animationSecond = 0;
    std::vector<std::string> playersName;
    std::vector<std::string> playerIcons;
    std::vector<TileConfig> boardTiles;
    std::vector<std::string> propertyLevelIcons;

    GameConfig() = default; // Private constructor

public:
    static GameConfig& getInstance();

    // Set and get game mode
    void setMode(GameMode newMode);
    GameMode getMode() const;

    // Set and get number of players
    void setPlayersNum(int num);
    int getPlayersNum() const;

    // Set and get player names
    void setPlayersName(const std::vector<std::string>& names);
    std::vector<std::string> getPlayerNames() const;

    // Set and get player icons
    void setPlayerIcons(const std::vector<std::string>& icons);
    std::vector<std::string> getPlayerIcons() const;

    // Set and get property level icons
    void setPropertyLevelIcons(const std::vector<std::string>& icons);
    std::vector<std::string> getPropertyLevelIcons() const;

    // Set and get money information
    void setStartMoney(int amount);
    int getStartMoney() const;

    void setWinMoney(int amount);
    int getWinMoney() const;

    void setPassingStartBonus(int amount);
    int getPassingStartBonus() const;

    // Set and get board tile information
    void setBoardTiles(const std::vector<TileConfig>& tiles);
    std::vector<TileConfig> getBoardTiles() const;

    int getMapSize() const;

    int getTileWidth() const;

    long long getAnimationSecond() const;

    void loadConfig();
};

#endif // GAMECONFIG_HPP
