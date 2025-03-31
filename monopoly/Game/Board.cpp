#include "Board.hpp"
#include "Tiles/EventTile.hpp"
#include "Tiles/HospitalTile.hpp"
#include "Tiles/PropertyTile.hpp"
#include "Tiles/StartTile.hpp"
#include "Tiles/StoreTile.hpp"
#include "Utils/Utils.hpp"
#include "SingletonManager.hpp"
#include <iomanip>
#include <iostream>
#include <map>
#include <thread>
#include <vector>
#ifdef _WIN32
#    include <windows.h> // For system("cls")
#endif

Board* Board::instance = nullptr;
std::string resetColor = "\033[0m";

Board::Board(const GameConfig& config) {
    SingletonManager::registerDestructor(CardStore::destroyInstance);
    // this->mapSize = config.getMapSize();
    this->mapSize = 8;
    this->tileWidth = config.getTileWidth();
    this->animation = config.getAnimation();
    this->animationTime = config.getAnimationTime();
}

void Board::init(const GameConfig& config, const std::vector<std::shared_ptr<Player>>& players) {
    // Clear existing tiles
    tiles.clear();
    playersList = players;
    for (const auto& player : players) {
        playersPosition[player] = player->getPosition();
    }
    for (const auto& boardTiles : config.getBoardTiles()) {
        if (boardTiles.type == "property") {
            tiles.push_back(std::make_shared<PropertyTile>(boardTiles.id, boardTiles.name, boardTiles.cost, boardTiles.rent));
        } else if (boardTiles.type == "fate") {
            tiles.push_back(std::make_shared<EventTile>(boardTiles.id, boardTiles.name, EventType::FATE, config.getEventValueRange()));
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

    for (int posIndex = 0; posIndex < 28; ++posIndex) {
        auto [row, col] = getBoardPosition(posIndex, mapSize);
        if (row != -1 && col != -1) {
            if (tiles[posIndex]) {
                std::shared_ptr<StartTile> startTile = std::dynamic_pointer_cast<StartTile>(tiles[posIndex]);
                std::shared_ptr<StoreTile> storeTile = std::dynamic_pointer_cast<StoreTile>(tiles[posIndex]);
                std::shared_ptr<EventTile> eventTile = std::dynamic_pointer_cast<EventTile>(tiles[posIndex]);
                std::shared_ptr<HospitalTile> hospitalTile = std::dynamic_pointer_cast<HospitalTile>(tiles[posIndex]);

                if (startTile) {
                    board[row][col] = "\033[30m\033[48;5;46m" + startTile->getNameWithId() + resetColor; // Green background
                } else if (storeTile) {
                    board[row][col] = "\033[30m\033[48;5;220m" + storeTile->getNameWithId() + resetColor; // Yellow background
                } else if (eventTile) {
                    if (eventTile->getEventType() == EventType::FATE) {
                        board[row][col] = "\033[48;5;57m" + eventTile->getNameWithId() + resetColor; // Blue background
                    } else {
                        board[row][col] = "\033[48;5;53m" + eventTile->getNameWithId() + resetColor; // Blue background
                    }
                } else if (hospitalTile) {
                    board[row][col] = "\033[48;5;124m" + hospitalTile->getNameWithId() + resetColor; // Red background
                } else {
                    board[row][col] = tiles[posIndex]->getNameWithId();
                }
            }
        }
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

void Board::drawBoard() {
    // === Board ===
    do {
        clearScreen();
        // Update player and property levels
        updatePlayerPositions(playersList);
        updateProperty(playersList);
        std::cout << "+";
        for (int j = 0; j < mapSize; j++) {
            std::cout << std::string(this->tileWidth, '-') << "+";
        }
        std::cout << "\n";
        for (size_t i = 0; i < mapSize; i++) {
            // tile name
            std::cout << "| ";
            for (size_t j = 0; j < mapSize; j++) {
                const std::string& cell = board[i][j];
                std::cout << std::left << std::setw(this->tileWidth - 2 + (cell.length() - stripAnsi(cell).length())) << cell << " | ";
            }
            // player icon
            std::cout << "\n| ";
            for (size_t j = 0; j < mapSize; j++) {
                const std::string& cell = playerBoard[i][j];
                std::cout << std::left << std::setw(this->tileWidth - 2 + (cell.length() - stripAnsi(cell).length())) << cell << " | ";
            }
            // property level
            std::cout << "\n| ";
            for (size_t j = 0; j < mapSize; j++) {
                std::cout << "\033[48;5;237m " << std::left << std::setw(this->tileWidth - 3) << propertyLevelIcons[propertyLevelBoard[i][j]] << resetColor
                          << " | ";
            }
            // footer
            std::cout << "\n+";
            for (size_t j = 0; j < mapSize; j++) {
                std::cout << std::string(this->tileWidth, '-') << "+";
            }
            std::cout << "\n";
        }
        if (animation && !animationDone) {
            delayTime(animationTime);
        }

    } while (!animationDone && animation);

    std::cout << std::endl;

    // === Player info ===
    std::cout << "+----------------+------------+--------------------------------------------+-----------------------------+\n";
    std::cout << "| Player Name    | Assets     | Property                                   | Cards                       |\n";
    std::cout << "+----------------+------------+--------------------------------------------+-----------------------------+\n";

    for (const auto& player : playersList) {
        const auto& properties = getPlayerProperty(player);
        std::string propertyIds;
        for (const auto& property : properties) {
            propertyIds += std::to_string(property->getId()) + ", ";
        }
        propertyIds = propertyIds.substr(0, propertyIds.size() - 2); // Remove the last comma

        std::string cardIcons;
        for (const auto& card : player->getCards()) {
            cardIcons += card->getIcon() + ", ";
        }
        cardIcons = cardIcons.substr(0, cardIcons.size() - 2); // Remove the last comma

        std::cout << "| " << player->getIconWithColor() << " " << std::setw(10) << player->getName() << " | " << std::setw(10) << player->getMoney() << " | "
                  << std::setw(42) << propertyIds << " | " << std::setw(27) << cardIcons << " | ";

        std::cout << std::endl;
    }
    std::cout << "+----------------+------------+--------------------------------------------+-----------------------------+\n\n";

    for (auto player : playersList) {
        if (player->checkIsMyTurn()) {
            std::cout << "It's " << player->getName() << "'s turn." << std::endl;
        }
    }
}
void Board::drawMonopolyAscii() {
    std::cout << "\n\n";
    std::cout << R"(                                                                                          
                                                                                          
            .---.            ,--,                                ____                     
           /. ./|          ,--.'|                              ,'  , `.                   
       .--'.  ' ;          |  | :               ,---.       ,-+-,.' _ |                   
      /__./ \ : |          :  : '              '   ,'\   ,-+-. ;   , ||                   
  .--'.  '   \' .   ,---.  |  ' |      ,---.  /   /   | ,--.'|'   |  || ,---.             
 /___/ \ |    ' '  /     \ '  | |     /     \.   ; ,. :|   |  ,', |  |,/     \            
 ;   \  \;      : /    /  ||  | :    /    / ''   | |: :|   | /  | |--'/    /  |           
  \   ;  `      |.    ' / |'  : |__ .    ' / '   | .; :|   : |  | ,  .    ' / |           
   .   \    .\  ;'   ;   /||  | '.'|'   ; :__|   :    ||   : |  |/   '   ;   /|           
    \   \   ' \ |'   |  / |;  :    ;'   | '.'|\   \  / |   | |`-'    '   |  / |           
     :   '  |--" |   :    ||  ,   / |   :    : `----'  |   ;/        |   :    |           
      \   \ ;     \   \  /  ---`-'   \   \  /          '---'          \   \  /            
       '---"       `----'             `----'                           `----'             
           ____                                                                           
         ,'  , `.                                                      ,--,               
      ,-+-,.' _ |                                ,-.----.            ,--.'|               
   ,-+-. ;   , ||   ,---.        ,---,    ,---.  \    /  \    ,---.  |  | :               
  ,--.'|'   |  ;|  '   ,'\   ,-+-. /  |  '   ,'\ |   :    |  '   ,'\ :  : '               
 |   |  ,', |  ': /   /   | ,--.'|'   | /   /   ||   | .\ : /   /   ||  ' |        .--,   
 |   | /  | |  ||.   ; ,. :|   |  ,"' |.   ; ,. :.   : |: |.   ; ,. :'  | |      /_ ./|   
 '   | :  | :  |,'   | |: :|   | /  | |'   | |: :|   |  \ :'   | |: :|  | :   , ' , ' :   
 ;   . |  ; |--' '   | .; :|   | |  | |'   | .; :|   : .  |'   | .; :'  : |__/___/ \: |   
 |   : |  | ,    |   :    ||   | |  |/ |   :    |:     |`-'|   :    ||  | '.'|.  \  ' |   
 |   : '  |/      \   \  / |   | |--'   \   \  / :   : :    \   \  / ;  :    ; \  ;   :   
 ;   | |`-'        `----'  |   |/        `----'  |   | :     `----'  |  ,   /   \  \  ;   
 |   ;/                    '---'                 `---'.|              ---`-'     :  \  \  
 '---'                                             `---`                          \  ' ;  
                                                                                   `--`   
)";
    delayTime(animationTime * 10);
    clearScreen();
    std::cout << "\n\n";
    std::cout << R"(-----------------------------------------==+*+---------------------=#******#*-------------
-------------------------------------=+##%@@@@#--------------------***##+##%+-------------
----------------------------------+#%*****%@@@@%=------------------#**#*------------------
-------------------------------=#%*********%@@@@%=-----------------=%***%=----------------
-----------------------------+%@@%**********%@@@@%=------------------+#***%=--------------
---------------------------+%@@@@@@#*********%%%%%@*------==----------=##**#*=------------
-------------------------=%@@@@@@@@@%****#%####@@@@@%=---+@%------------=%***%+-----------
------------------------=@@@@@@@@@@@@@#@########%@@@@@*=@@*---------------+#***%=---------
------------------------+%##@@@@@@@@@%%#############%%@%#=------------------*#**##--------
-------------------------=*%#%@@@@@%%@@%##%%%%@@@%#*+==*%%%=-----------------=%***%+--=---
---------------------------=%%#@@%%%%%@@@%###++============#+------------------+#***%#+%==
-----------------------------*%%%%@@%##%*==*@*+=============%+-----------------=-#@#===@+=
---------=+#*-----------------=%@%##%%+===+*============+%%==%=-----------------#*===+%==+
--------=%*=#----------------=%%##@%+==========+=============#*---------------=%%====+#*%*
--------%*==%=--------------=@%#%@@+*#+=======%@+=====+*=====+#------------*@%=-+#===**%%%
--=+#%@%#===+%=-------------+@%%@@@+**=================#+=====%=##------+%@@@@@+-=%====++=
#%*==========***%%*=--------=@@%@@@%+=============++++##===*###+=#--=+%@@@@@@@@@*-=#+===+#
+#%#*+======**-=@@@@@%+=------+***+==============##=-----------=%#%@@@@@@@@@@@@@@*=++=++=-
=*%%#======+#-=@@@@@@@@@@@%#+=---+*=============#+-----------=#@@@@@@@@@@@@@@@@@@%==------
====++**##*%==%@@@@@@@@@@@@@@@@@@@@%++%+==*++###=-----=@%#%%@@@@@@@@@@@@@@@@@@@@@%+-------
**+=======+*=*@@@@@@@@@@@@@@@@@@@@@@@@@@#==+#*=-----+#%%===#@@@@@@@@@@@@@@@@@@@#=---------
-----------=+%@@@@@@@@@@@@@@@@@@@@@@@@@@@@%+==+*###*+====*%@@@@@@@@@@@@@@@@@#=------------
------------=*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%@#+=====+#@%+=%@@@@@@@@@@@@#+----------------
----------------=*%@@@@@@@@@@@@@@@@@@@@@@@@@%--%%%%%%%%%%+-*@@@@@@@%*=--------------------
---------------------==*#%%@@@@@@@@@@@@@@@@@%-=%%%%%%%%%%+-*@@*---------------------------
-------------------------------=@@@@@@@@@@@@#-=@%*=-===+#=-*@#----------------------------
-------------------------------+@@@@@@@@@@@@+--=----*=-----*@=----------------------------
-------------------------------#@@@@@@@@@@@@=-------------=%%=----------------------------
------------------------------#@@@@@@@@@@@@+-------+#=----+@#-----------------------------
----------------------------=#@@@@@@@@@@@@%=--------------#@%=----------------------------
---------------------------=%@@@@@@@@@@@@%--------*@=----*@@@=----------------------------
--------------------------*@@@@@@@@@@@@@%*--------------=%@@@@=---------------------------
-----------------------=#@@@@@@@@@@@@@%#*#%+---=*##%=--=%#%@@@@%+=------------------------
---------------------*@@@@@@@@@@@@@%#*****#*%#%#*#**#%*#***#@@@@@*------------------------
-------------------------==++=#@%*****************#**********#%=--------------------------
---------------------------=*@%#*#**************#************##%=-------------------------
------------------------=#@%#****#***********#*%#**************#@=------------------------
-----------------------=@%*******************#%*****************#@=-----------------------
-----------------------+@#******************%**%%%##*************%*-----------------------
--------==------------=%%*****************#%+----=+@%************#%-----------------------
--------#@*=---------=%%*****************%%=------*%%#*#**********%=----------------------
--------*@@@+------+@@#***************##@*----------=%%***********%=----------------------
---------#@@@@@@%%%#*#*##************#@#=-----------%%************%-----------------------
---------=#@@@@@@#*****************#@%=------------*@#***********#*-----------------------
-----------+%@@@@@%**************%@#=-------------=@#***********#%=-----------------------
--------------*@@@@@#********##@@*----------------+@##**********%=------------------------
-----------------=%@@%*****#%@#=------------------+@#*******###*=======+=-----------------
-----------------=%@@@@#%%%*=---------------------=@@@@@@@@@@@@@@@@@@@@%=-----------------
------------------=*@@@%+=-------------------------*@@@@**%@@@@@@@@@%*=-------------------)";
    std::cout << "\n\n";
}

void Board::updatePlayerPositions(const std::vector<std::shared_ptr<Player>>& players) {
    for (auto& row : playerBoard) {
        for (auto& cell : row) {
            cell = "";
        }
    }
    // Update barrier positions
    for (int posIndex = 0; posIndex < 28; ++posIndex) {
        if (tiles[posIndex]->isBlocked()) {
            auto [rowOut, colOut] = getBoardPosition(posIndex, mapSize);
            if (rowOut != -1 && colOut != -1) {
                playerBoard[rowOut][colOut] += "\033[1;31mX\033[0m";
            }
        }
    }

    // Update player positions (animation or direct)
    for (const auto& player : players) {
        int pPos = player->getPosition() % 28; // limit the index range
        if (animation) {
            if (playersPosition[player] != pPos) {
                playersPosition[player]++;
                playersPosition[player] %= 28; // limit the index range
            }
            auto [rowOut, colOut] = getBoardPosition(playersPosition[player], mapSize);
            if (rowOut != -1 && colOut != -1) {
                playerBoard[rowOut][colOut] += player->getIconWithColor();
            }
        } else {
            playersPosition[player] = pPos;
            auto [rowOut, colOut] = getBoardPosition(pPos, mapSize);
            if (rowOut != -1 && colOut != -1) {
                playerBoard[rowOut][colOut] += player->getIconWithColor();
            }
        }
    }
    animationDone = true;
    for (const auto& player : players) {
        if (playersPosition[player] != player->getPosition() % 28) {
            animationDone = false;
            break;
        }
    }
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
                std::shared_ptr<EventTile> eventTile = std::dynamic_pointer_cast<EventTile>(tiles[posIndex]);

                if (propertyTile) { // make sure it is a property tile
                    int level = static_cast<int>(propertyTile->getPropertyLevel());
                    propertyLevelBoard[row][col] = level; // Like level 1, level 2, level 3

                    // Update the board with the owner's color
                    auto owner = propertyTile->getPropertyOwner();
                    board[row][col] = propertyTile->getNameWithId();
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
