#ifndef BOARD_HPP
#define BOARD_HPP

#include "GameConfig.hpp"
#include "Tile.hpp"
#include <map>
#include <memory>
#include <vector>
class Board {
private:
    int mapSize = 0;
    int tileWidth = 0;
    std::vector<std::shared_ptr<Tile>> tiles;
    std::vector<std::vector<std::string>> board;
    std::vector<std::vector<int>> propertyLevelBoard;
    std::vector<std::string> propertyLevelIcons;

public:
    Board(const GameConfig& config);
    int getSize() const;
    std::shared_ptr<Tile> getTile(int index);
    void drawBoard(std::vector<std::shared_ptr<Player>>& players);
    void updatePropertyLevelBoard(int row, int col, int posIndex);
};
#endif // BOARD_HPP
