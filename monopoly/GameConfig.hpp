#ifndef GAMECONFIG_HPP
#define GAMECONFIG_HPP

#include <string>
#include <vector>

// 🔥 定義遊戲模式
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
    GameMode mode;
    GameConfig() = default; // 私有建構函式，避免外部創建物件

public:
    int players;
    int start_money;
    int win_money;
    std::vector<std::string> player_names;
    std::vector<std::string> player_icons;
    std::vector<TileConfig> boardTiles; // 地圖格子資訊

    static GameConfig& getInstance();
    void setMode(GameMode newMode);
    GameMode getMode() const;
    void loadConfig();
};

#endif // GAMECONFIG_HPP
