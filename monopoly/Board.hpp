// #ifndef BOARD_HPP
// #define BOARD_HPP

// #include "GameConfig.hpp"
// #include "Tile.hpp"
// #include <map>
// #include <memory>
// #include <vector>
// class Board {
// private:
//     int mapSize = 0;
//     int tileWidth = 0;
//     std::vector<std::shared_ptr<Tile>> tiles;
//     std::vector<std::vector<std::string>> board;
//     std::vector<std::vector<int>> propertyLevelBoard;
//     std::vector<std::string> propertyLevelIcons;

// public:
//     Board(const GameConfig& config);
//     int getSize() const;
//     std::shared_ptr<Tile> getTile(int index);
//     std::vector<std::shared_ptr<Tile>> getTileList();

//     template <typename T>
//     std::vector<int> findAllTilePositions();

//     template <typename T>
//     int findNextTilePosition();

//     void drawBoard(std::vector<std::shared_ptr<Player>>& players);
//     void updatePropertyLevelBoard(int row, int col, int posIndex);
//     static void clearScreen();
// };
// #endif // BOARD_HPP

#ifndef BOARD_HPP
#define BOARD_HPP

#include "GameConfig.hpp"
#include "Tile.hpp"
#include <map>
#include <memory>
#include <vector>

class Board {
private:
    static Board* instance;
    int mapSize = 0;
    int tileWidth = 0;
    std::vector<std::shared_ptr<Tile>> tiles;
    std::vector<std::vector<std::string>> board;
    std::vector<std::vector<int>> propertyLevelBoard;
    std::vector<std::string> propertyLevelIcons;

    Board(const GameConfig& config);
    ~Board() = default;

public:
    static Board* getInstance(const GameConfig& config);
    static Board& getInstance();

    static void destroyInstance();

    Board(const Board&) = delete;
    Board& operator=(const Board&) = delete;

    int getSize() const;
    std::shared_ptr<Tile> getTile(int index);
    std::vector<std::shared_ptr<Tile>> getTileList();

    template <typename T>
    std::vector<int> findAllTilePositions();

    template <typename T>
    int findNextTilePosition();

    void drawBoard(std::vector<std::shared_ptr<Player>>& players);
    void updatePropertyLevelBoard(int row, int col, int posIndex);
    static void clearScreen();
};

#endif // BOARD_HPP
