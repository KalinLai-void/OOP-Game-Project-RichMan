#ifndef BOARD_HPP
#define BOARD_HPP

#include "Tile.hpp"
#include <memory>
#include <vector>

class Board {
private:
    std::vector<std::shared_ptr<Tile>> tiles;

public:
    Board();
    int getSize() const;
    std::shared_ptr<Tile> getTile(int index);
};

#endif // BOARD_HPP
