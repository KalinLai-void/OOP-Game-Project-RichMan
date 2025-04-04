#ifndef GAMECONFIG_HPP
#define GAMECONFIG_HPP

#include <map>
#include <string>
#include <vector>

/**
 * @brief Defines different game modes.
 */
enum class GameMode { DEBUG, DUEL, RELEASE, CUSTOM };

struct TileConfig {
    std::size_t id;
    std::string type;
    std::string name;
    int cost;
    int rent;
};

struct CardConfig {
    std::string name;
    std::string icon;
    int price;
    std::string effect;
};

class GameConfig {
private:
    static GameConfig instance;
    GameMode mode = GameMode::DEBUG;
    int playersNum = 2; // [default] number of players
    int startMoney = 0;
    int winMoney = 0;
    int passingStartBonus = 0;
    int mapSize = 8;
    size_t tileWidth = 14;
    long long animationTime = 0;
    bool animation = true;
    std::vector<std::string> playersName;
    std::vector<std::string> playerIcons;
    std::vector<std::string> playerColors;

    std::vector<TileConfig> boardTiles;
    std::vector<CardConfig> cards;
    std::vector<std::string> propertyLevelIcons;
    std::map<int, std::string> locationMap;
    std::map<std::string, std::pair<int, int>> eventValueRange;

    GameConfig() = default; // Private constructor

public:
    static GameConfig& getInstance();

    /**
     * @brief Loads game configuration from an external JSON file.
     * 
     * Loads data from "json/config.json" to initialize game settings including:
     * 
     * - Player icons and colors (`playerIcons`, `playerColors`)
     * - Tile settings such as width and board tile layout (`tileWidth`, `boardTiles`)
     * - Property level icons and animation options (`propertyLevelIcons`, `animation`, `animationTime`)
     * - Map layout and size (`mapSize`)
     * - Card definitions (`cards`)
     * - Event value ranges (`eventValueRange`)
     * - Game mode–specific values (`playersNum`, `playersName`, `startMoney`, `winMoney`, `passingStartBonus`)
     * 
     * The JSON structure must include a top-level `"modes"` object that contains per-mode configurations.
     * 
     * If the file cannot be opened or contains invalid structure, an error message will be printed to stderr.
     */
    void loadConfig();

    // Mode
    void setMode(GameMode newMode);
    GameMode getMode() const;

    // Player data
    void setPlayersNum(int num);
    int getPlayersNum() const;

    void setPlayersName(const std::vector<std::string>& names);
    std::vector<std::string> getPlayerNames() const;

    void setPlayerIcons(const std::vector<std::string>& icons);
    std::vector<std::string> getPlayerIcons() const;

    void setPlayerColors(const std::vector<std::string>& colors);
    std::vector<std::string> getPlayerColors() const;

    // UI
    void setPropertyLevelIcons(const std::vector<std::string>& icons);
    std::vector<std::string> getPropertyLevelIcons() const;

    std::map<int, std::string> getLocationMap() const; // Returns a map of tile IDs to their names.

    // Money
    void setStartMoney(int amount);
    int getStartMoney() const;

    void setWinMoney(int amount); // Sets the win condition based on target money.
    int getWinMoney() const;

    void setPassingStartBonus(int amount); // Sets the bonus for passing the start tile.
    int getPassingStartBonus() const;

    // Tiles and cards
    void setBoardTiles(const std::vector<TileConfig>& tiles);
    std::vector<TileConfig> getBoardTiles() const;

    void setCards(const std::vector<CardConfig>& cards);
    std::vector<CardConfig> getCards() const;

    // Events
    void setEventValueRange(const std::map<std::string, std::pair<int, int>>& range);
    std::map<std::string, std::pair<int, int>> getEventValueRange() const;

    // Animation settings
    void setAnimation(bool status);
    bool getAnimation() const;

    int getMapSize() const;
    int getTileWidth() const;
    long long getAnimationTime() const;
};

#endif // GAMECONFIG_HPP
