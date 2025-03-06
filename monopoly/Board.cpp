#include "Board.hpp"
#include "EventTile.hpp"
#include "HospitalTile.hpp"
#include "PropertyTile.hpp"
#include "SingletonManager.hpp"
#include "StartTile.hpp"
#include "StoreTile.hpp"
#include "Utils.hpp"
#include <iomanip>
#include <iostream>
#include <map>
#include <vector>
#ifdef _WIN28
#    include <windows.h> // For system("cls")
#endif

Board* Board::instance = nullptr;

Board::Board(const GameConfig& config) {
    SingletonManager::registerDestructor(CardStore::destroyInstance);
    // this->mapSize = config.getMapSize();
    this->mapSize = 8;
    this->tileWidth = config.getTileWidth();
}

void Board::init(const GameConfig& config) {
    // Clear existing tiles
    tiles.clear();
    for (const auto& boardTiles : config.getBoardTiles()) {
        if (boardTiles.type == "property") {
            tiles.push_back(std::make_shared<PropertyTile>(boardTiles.id, boardTiles.name, boardTiles.cost, boardTiles.rent));
        } else if (boardTiles.type == "fate") {
            // test barrier
            std::shared_ptr<Tile> tilea = std::make_shared<EventTile>(boardTiles.id, boardTiles.name, EventType::FATE, config.getEventValueRange());
            tilea->setBlock(true);
            tiles.push_back(tilea);
            // tiles.push_back(std::make_shared<EventTile>(boardTiles.id, boardTiles.name, EventType::FATE, config.getEventValueRange()));
        } else if (boardTiles.type == "chance") {
            tiles.push_back(std::make_shared<EventTile>(boardTiles.id, boardTiles.name, EventType::CHANCE, config.getEventValueRange()));
        } else if (boardTiles.type == "store") {
            tiles.push_back(std::make_shared<StoreTile>(boardTiles.id, boardTiles.name, config));
        } else if (boardTiles.type == "hospital") {
            tiles.push_back(std::make_shared<HospitalTile>(boardTiles.id, boardTiles.name));
        } else if (boardTiles.type == "start") {
            tiles.push_back(std::make_shared<StartTile>(boardTiles.id, boardTiles.name, config.getPassingStartBonus()));
        } else {
            std::cout << "Unknown Tile Type: " << boardTiles.id << " " << boardTiles.type << std::endl;
        }
    }

    // Create an 8x8 empty board
    board = std::vector<std::vector<std::string>>(mapSize, std::vector<std::string>(mapSize, "   "));
    playerBoard = std::vector<std::vector<std::string>>(mapSize, std::vector<std::string>(mapSize, ""));
    propertyLevelBoard = std::vector<std::vector<int>>(mapSize, std::vector<int>(mapSize, 0));
    propertyLevelIcons = config.getPropertyLevelIcons();
    int posIndex = 0; // Tile index (0~31)
    for (int col = 0; col < mapSize; ++col) {
        // Top
        if (tiles[posIndex]) {
            board[0][col] = tiles[posIndex]->getNameWithId();
        } else {
            board[0][col] = "P" + std::to_string(posIndex);
        }
        posIndex++;
    }
    for (int row = 1; row < mapSize; ++row) {
        // Right
        if (tiles[posIndex]) {
            board[row][mapSize - 1] = tiles[posIndex]->getNameWithId();
        } else {
            board[row][mapSize - 1] = "P" + std::to_string(posIndex);
        }
        posIndex++;
    }
    for (int col = mapSize - 2; col >= 0; --col) {
        // Bottom
        if (tiles[posIndex]) {
            board[mapSize - 1][col] = tiles[posIndex]->getNameWithId();
        } else {
            board[mapSize - 1][col] = "P" + std::to_string(posIndex);
        }
        posIndex++;
    }
    for (int row = mapSize - 2; row > 0; --row) {
        // Left
        if (tiles[posIndex]) {
            board[row][0] = tiles[posIndex]->getNameWithId();
        } else {
            board[row][0] = "P" + std::to_string(posIndex);
        }
        posIndex++;
    }
}

Board* Board::getInstance(const GameConfig& config) {
    if (instance == nullptr) {
        instance = new Board(config);
    }
    return instance;
}

Board* Board::getInstance() {
    if (instance == nullptr) {
        throw std::runtime_error("Board instance is not initialized!");
    }
    return instance;
}

void Board::destroyInstance() {
    if (instance) {
        delete instance;
        instance = nullptr;
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

std::vector<std::shared_ptr<Tile>> Board::getTileList() {
    return tiles;
}

template <typename T>
std::vector<int> Board::findAllTilePositions() {
    std::vector<int> positions;
    for (size_t i = 0; i < tiles.size(); ++i) {
        if (std::dynamic_pointer_cast<T>(tiles[i])) { // Check if it's of type T
            positions.push_back(static_cast<int>(i)); // Store index
        }
    }
    return positions;
}

template <typename T>
int Board::findNextTilePosition() {

    auto it = std::find_if(tiles.begin(), tiles.end(), [](const std::shared_ptr<Tile>& tile) {
        return std::dynamic_pointer_cast<T>(tile) != nullptr;
    });
    if (it != tiles.end()) {
        return static_cast<int>(std::distance(tiles.begin(), it));
    }
    return -1;
}

void Board::drawBoard(const std::vector<std::shared_ptr<Player>>& players) {
    clearScreen();

    // Update player and property levels
    updatePlayerPositions(players);
    updateProperty(players);

    // Output the board
    std::cout << "+";
    for (int j = 0; j < mapSize; j++) {
        std::cout << std::string(this->tileWidth, '-') << "+";
    }
    std::cout << "\n";
    for (int i = 0; i < mapSize; i++) {
        // tile name
        std::cout << "| ";
        for (int j = 0; j < mapSize; j++) {
            const std::string& cell = board[i][j];
            std::cout << std::left << std::setw(this->tileWidth - 2 + (cell.length() - stripAnsi(cell).length())) << cell << " | ";
        }
        // player icon
        std::cout << "\n| ";

        for (int j = 0; j < mapSize; j++) {
            const std::string& cell = playerBoard[i][j];
            std::cout << std::left << std::setw(this->tileWidth - 2 + (cell.length() - stripAnsi(cell).length())) << cell << " | ";
        }
        // property level
        std::cout << "\n| ";
        for (int j = 0; j < mapSize; j++) {
            std::cout << std::left << std::setw(this->tileWidth - 2) << propertyLevelIcons[propertyLevelBoard[i][j]] << " | ";
        }
        // footer
        std::cout << "\n+";
        for (int j = 0; j < mapSize; j++) {
            std::cout << std::string(this->tileWidth, '-') << "+";
        }
        std::cout << "\n";
    }
    std::cout << std::endl;
    // === Player info ===
    std::cout << "+----------------+------------+--------------------------------+\n";
    std::cout << "| Player Name    | Assets     | Property                       |\n";
    std::cout << "+----------------+------------+--------------------------------+\n";

    for (const auto& player : players) {
        const auto& properties = getPlayerProperty(player);
        std::string propertyIds;
        for (const auto& property : properties) {
            propertyIds += std::to_string(property->getId()) + ", ";
        }
        propertyIds = propertyIds.substr(0, propertyIds.size() - 2); // Remove the last comma

        std::cout << "| " << player->getIconWithColor() << " " << std::setw(10) << player->getName() << " | " << std::setw(10) << player->getMoney() << " | "
                  << std::setw(30) << propertyIds << " | ";

        std::cout << std::endl;
    }
    std::cout << "+----------------+------------+--------------------------------+\n";
}

void Board::updatePlayerPositions(const std::vector<std::shared_ptr<Player>>& players) {
    for (auto& row : playerBoard) {
        for (auto& cell : row) {
            cell = "";
        }
    }
    std::vector<int> blockedPos = getTileBlockPos();
    for (const auto& pos : blockedPos) {
        auto [rowOut, colOut] = getBoardPosition(pos, mapSize);
        if (rowOut != -1 && colOut != -1) {
            playerBoard[rowOut][colOut] = "\033[1;31mX\033[0m";
        }
    }

    for (const auto& player : players) {
        int pPos = player->getPosition() % 28; // 限制索引範圍
        auto [rowOut, colOut] = getBoardPosition(pPos, mapSize);
        if (rowOut != -1 && colOut != -1) {
            playerBoard[rowOut][colOut] += player->getIconWithColor();
        }
    }
}

std::vector<int> Board::getTileBlockPos() {
    std::vector<int> blockedPos;
    for (int posIndex = 0; posIndex < 28; ++posIndex) {
        if (tiles[posIndex]->isBlocked()) {
            blockedPos.push_back(posIndex);
        }
    }
    return blockedPos;
}

std::vector<std::shared_ptr<PropertyTile>> Board::getPlayerProperty(const std::shared_ptr<Player>& player) {
    std::vector<std::shared_ptr<PropertyTile>> playerProperties;
    for (const auto& tile : tiles) {
        auto propertyTile = std::dynamic_pointer_cast<PropertyTile>(tile);
        if (propertyTile && propertyTile->getPropertyOwner() == player) {
            playerProperties.push_back(propertyTile);
        }
    }
    return playerProperties;
}

void Board::updateProperty(const std::vector<std::shared_ptr<Player>>& players) {
    for (int posIndex = 0; posIndex < 28; ++posIndex) {
        auto [row, col] = getBoardPosition(posIndex, mapSize);
        if (row != -1 && col != -1) {
            if (tiles[posIndex]) {
                // Convert Tile class to PropertyTile class
                std::shared_ptr<PropertyTile> propertyTile = std::dynamic_pointer_cast<PropertyTile>(tiles[posIndex]);

                if (propertyTile) { // make sure it is a property tile
                    int level = static_cast<int>(propertyTile->getPropertyLevel());
                    propertyLevelBoard[row][col] = level; // Like level 1, level 2, level 3

                    // Update the board with the owner's color
                    auto owner = propertyTile->getPropertyOwner();
                    if (owner) {
                        board[row][col] = propertyTile->getNameWithId();
                    }
                }
            }
        }
    }
}

void Board::clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    std::cout << "\033[2J\033[H"; // ANSI Escape Code to clear the screen and move the cursor to the top-left corner
#endif
}
