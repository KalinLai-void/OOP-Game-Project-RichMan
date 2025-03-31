#ifndef BOARD_HPP
#define BOARD_HPP

#include "GameConfig.hpp"
#include "Tiles/Tile.hpp"
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
    size_t tileWidth = 0;
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
    std::vector<int> findAllTilePositions() {
        std::vector<int> positions;
        for (size_t i = 0; i < tiles.size(); ++i) {
            if (std::dynamic_pointer_cast<T>(tiles[i])) { // Check if it's of type T
                positions.push_back(static_cast<int>(i)); // Store index
            }
        }
        return positions;
    }

    template <typename T>
    int findNextTilePosition() {

        auto it = std::find_if(tiles.begin(), tiles.end(), [](const std::shared_ptr<Tile>& tile) {
            return std::dynamic_pointer_cast<T>(tile) != nullptr;
        });
        if (it != tiles.end()) {
            return static_cast<int>(std::distance(tiles.begin(), it));
        }
        return -1;
    }

    void drawBoard();
    void drawMonopolyAscii();

    std::vector<std::shared_ptr<PropertyTile>> getPlayerProperty(const std::shared_ptr<Player>& player);
    static void clearScreen();
};

#endif // BOARD_HPP
