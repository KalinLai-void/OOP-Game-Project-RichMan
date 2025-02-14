#include "Board.hpp"
#include "EventTile.hpp"
#include "HospitalTile.hpp"
#include "PropertyTile.hpp"
#include "StoreTile.hpp"
#include <memory>

Board::Board() {
    // 初始化棋盤格，這裡依序加入各種格子
    tiles.push_back(std::make_shared<PropertyTile>("地產A", 2000, 200));
    tiles.push_back(std::make_shared<EventTile>("命運"));
    tiles.push_back(std::make_shared<PropertyTile>("地產B", 3000, 300));
    tiles.push_back(std::make_shared<StoreTile>("道具商店"));
    tiles.push_back(std::make_shared<PropertyTile>("地產C", 5000, 500));
    tiles.push_back(std::make_shared<HospitalTile>("醫院"));
    tiles.push_back(std::make_shared<EventTile>("機會"));
    tiles.push_back(std::make_shared<PropertyTile>("地產D", 8000, 800));
}

int Board::getSize() const {
    return static_cast<int>(tiles.size());
}

std::shared_ptr<Tile> Board::getTile(int index) {
    if (index < 0 || index >= static_cast<int>(tiles.size()))
        return nullptr;
    return tiles[index];
}
