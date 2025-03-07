#ifndef BOARD_HPP
#define BOARD_HPP

#include "GameConfig.hpp"
#include "Tile.hpp"
#include <map>
#include <memory>
#include <vector>

class PropertyTile;
class Board {
private:
    static Board* instance;
    Board(const GameConfig& config);
    ~Board() = default;

    std::vector<std::shared_ptr<Player>> playersList;
    std::map<std::shared_ptr<Player>, int> playersPosition; // Pair(First: Init round position, Second: Current round position)
    int mapSize = 0;
    int tileWidth = 0;
    bool animation = true;
    bool animationDone = false;
    int animationTime = 0;
    std::vector<std::shared_ptr<Tile>> tiles;
    std::vector<std::vector<std::string>> board;
    std::vector<std::vector<std::string>> playerBoard;
    std::vector<std::vector<int>> propertyLevelBoard;
    std::vector<std::string> propertyLevelIcons;
    void updatePlayerPositions(const std::vector<std::shared_ptr<Player>>& players);
    void updateProperty(const std::vector<std::shared_ptr<Player>>& players);

public:
    static Board* getInstance(const GameConfig& config);
    static Board* getInstance();
    static void destroyInstance();
    Board(const Board&) = delete;
    Board& operator=(const Board&) = delete;

    void init(const GameConfig& config, const std::vector<std::shared_ptr<Player>>& players);
    int getSize() const;
    std::shared_ptr<Tile> getTile(int index);
    std::vector<std::shared_ptr<Tile>> getTileList();
    template <typename T>
    std::vector<int> findAllTilePositions();

    template <typename T>
    int findNextTilePosition();

    void drawBoard();

    std::vector<std::shared_ptr<PropertyTile>> getPlayerProperty(const std::shared_ptr<Player>& player);
    static void clearScreen();
};

#endif // BOARD_HPP
