#include "Board.hpp"
#include "EventTile.hpp"
#include "HospitalTile.hpp"
#include "PropertyTile.hpp"
#include "StartTile.hpp"
#include "StoreTile.hpp"
#include <iomanip>
#include <iostream>
#include <map>
#include <vector>

Board::Board(const GameConfig& config) {
    this->mapSize = 8; // todo case: set on GameConfig
    // 根據 GameConfig 的 boardTiles 來初始化棋盤
    for (const auto& boardTiles : config.getBoardTiles()) {
        if (boardTiles.type == "property") {
            tiles.push_back(std::make_shared<PropertyTile>(boardTiles.name, boardTiles.cost, boardTiles.rent));
        } else if (boardTiles.type == "event") {
            tiles.push_back(std::make_shared<EventTile>(boardTiles.name));
        } else if (boardTiles.type == "store") {
            tiles.push_back(std::make_shared<StoreTile>(boardTiles.name));
        } else if (boardTiles.type == "hospital") {
            tiles.push_back(std::make_shared<HospitalTile>(boardTiles.name));
        } else if (boardTiles.type == "start") {
            tiles.push_back(std::make_shared<StartTile>(boardTiles.name, config.getPassingStartBonus()));
        } else {
            std::cout << "未知的格子類型：" << boardTiles.type << std::endl;
        }
    }

    // 建立 8x8 的空白棋盤
    board = std::vector<std::vector<std::string>>(mapSize, std::vector<std::string>(mapSize, "   "));
    playerBoard = std::vector<std::vector<std::string>>(mapSize, std::vector<std::string>(mapSize, ""));

    int posIndex = 0; // 格子索引 (0~31)
    for (int col = 0; col < mapSize; ++col) {
        // 上排
        if (tiles[posIndex]) {
            board[0][col] = tiles[posIndex]->getName();
        } else {
            board[0][col] = "P" + std::to_string(posIndex);
        }
        posIndex++;
    }
    for (int row = 1; row < mapSize; ++row) {
        // 右排
        if (tiles[posIndex]) {
            board[row][mapSize - 1] = tiles[posIndex]->getName();
        } else {
            board[row][mapSize - 1] = "P" + std::to_string(posIndex);
        }
        posIndex++;
    }
    for (int col = mapSize - 2; col >= 0; --col) {
        // 下排
        if (tiles[posIndex]) {
            board[mapSize - 1][col] = tiles[posIndex]->getName();
        } else {
            board[mapSize - 1][col] = "P" + std::to_string(posIndex);
        }
        posIndex++;
    }
    for (int row = mapSize - 2; row > 0; --row) {
        // 左排
        if (tiles[posIndex]) {
            board[row][0] = tiles[posIndex]->getName();
        } else {
            board[row][0] = "P" + std::to_string(posIndex);
        }
        posIndex++;
    }
}

int Board::getSize() const {
    return static_cast<int>(tiles.size());
}

std::shared_ptr<Tile> Board::getTile(int index) {
    if (index < 0 || index >= static_cast<int>(tiles.size()))
        return nullptr;
    return tiles[index];
}

void Board::drawBoard(std::vector<std::shared_ptr<Player>>& players) {
    std::vector<std::vector<std::string>> tempPlayerBoard = playerBoard;
    int cellWidth = 12;

    // 更新玩家位置
    for (const auto& player : players) {
        int pPos = player->getPosition() % 32; // 確保索引不超過 31
        int tempIndex = 0, rowOut = 0, colOut = 0;
        bool found = false;

        // 找到該索引對應的 row, col
        for (int col = 0; col < mapSize && !found; ++col) {
            if (tempIndex == pPos) {
                rowOut = 0;
                colOut = col;
                found = true;
                break;
            }
            tempIndex++;
        }
        for (int row = 1; row < mapSize && !found; ++row) {
            if (tempIndex == pPos) {
                rowOut = row;
                colOut = mapSize - 1;
                found = true;
                break;
            }
            tempIndex++;
        }
        for (int col = mapSize - 2; col >= 0 && !found; --col) {
            if (tempIndex == pPos) {
                rowOut = mapSize - 1;
                colOut = col;
                found = true;
                break;
            }
            tempIndex++;
        }
        for (int row = mapSize - 2; row > 0 && !found; --row) {
            if (tempIndex == pPos) {
                rowOut = row;
                colOut = 0;
                found = true;
                break;
            }
            tempIndex++;
        }

        // 在棋盤上標記玩家位置
        tempPlayerBoard[rowOut][colOut] += player->getIcon();
    }

    // // 輸出棋盤
    // for (int i = 0; i < players.size(); i++) {
    //     std::cout << std::setw(2) << players[i]->getIcon() << ":" << players[i]->getName() << "   ";
    // }

    std::cout << "+";
    for (int j = 0; j < mapSize; j++) {
        std::cout << std::string(cellWidth, '-') << "+";
    }
    std::cout << "\n";
    for (int i = 0; i < mapSize; i++) {
        std::cout << "| ";
        for (int j = 0; j < mapSize; j++) {
            std::cout << std::left << std::setw(cellWidth - 2) << board[i][j] << " | ";
        }
        std::cout << "\n| ";
        for (int j = 0; j < mapSize; j++) {
            std::cout << std::left << std::setw(cellWidth - 2) << tempPlayerBoard[i][j] << " | ";
        }
        std::cout << "\n+";
        for (int j = 0; j < mapSize; j++) {
            std::cout << std::string(cellWidth, '-') << "+";
        }
        std::cout << "\n";
    }
    std::cout << std::endl;

    // === 顯示玩家資訊表格 ===
    std::cout << "+------------+------------+------------------------+\n";
    std::cout << "| 玩家名稱   | 持有金額   | 手牌                   |\n";
    std::cout << "+------------+------------+------------------------+\n";

    for (const auto& player : players) {
        std::cout << "| " << player->getIcon() << std::setw(10) << player->getName() << " | " << std::setw(10) << player->getMoney() << " | ";
        // todo: card system
        //     const auto& cards = player->getCards(); // 假設有 getCards() 方法返回 std::vector<std::string>
        //     if (cards.empty()) {
        //         std::cout << "無";
        //     } else {
        //         for (size_t i = 0; i < cards.size(); ++i) {
        //             std::cout << cards[i];
        //             if (i != cards.size() - 1) {
        //                 std::cout << ", ";
        //             }
        //         }
        //     }
        //     std::cout << std::setw(22 - (cards.empty() ? 1 : (cards.size() * 5))) << " |" << std::endl;
        std::cout << std::setw(24) << " |" << std::endl;
    }
    std::cout << "+------------+------------+------------------------+\n";
}