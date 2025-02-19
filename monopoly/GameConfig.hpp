#ifndef GAMECONFIG_HPP
#define GAMECONFIG_HPP

#include "Utils.hpp"
#include "nlohmann/json.hpp"
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

    GameConfig() = default; // 私有建構函式，避免外部創建物件

public:
    static GameConfig& getInstance();

    // 設定與獲取遊戲模式
    void setMode(GameMode newMode);
    GameMode getMode() const;

    // 設定與獲取玩家數量
    void setPlayersNum(int num);
    int getPlayersNum() const;

    // 設定與獲取玩家名稱
    void setPlayersName(const std::vector<std::string>& names);
    std::vector<std::string> getPlayerNames() const;

    // 設定與獲取玩家ICON
    void setPlayerIcons(const std::vector<std::string>& icons);
    std::vector<std::string> getPlayerIcons() const;

    // 設定與獲取地產等級ICON
    void setPropertyLevelIcons(const std::vector<std::string>& icons);
    std::vector<std::string> getPropertyLevelIcons() const;

    // 設定與獲取金錢資訊
    void setStartMoney(int amount);
    int getStartMoney() const;

    void setWinMoney(int amount);
    int getWinMoney() const;

    void setPassingStartBonus(int amount);
    int getPassingStartBonus() const;

    // 設定與獲取棋盤格資訊
    void setBoardTiles(const std::vector<TileConfig>& tiles);
    std::vector<TileConfig> getBoardTiles() const;

    int getMapSize() const;

    int getTileWidth() const;

    long long getAnimationSecond() const;

    void loadConfig();
};

#endif // GAMECONFIG_HPP
