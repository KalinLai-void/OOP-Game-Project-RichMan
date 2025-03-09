#include "Game.hpp"
#include "Bank.hpp"
#include "EventTile.hpp"
#include "HospitalTile.hpp"
#include "InputManager.hpp"
#include "MiniGameManager.hpp"
#include "PropertyTile.hpp"
#include "StartTile.hpp"
#include "StoreTile.hpp"
#include "Utils.hpp"
#include <algorithm>
#include <ctime>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <unordered_map>

std::mt19937 Game::engine;
std::shared_ptr<Game> Game::instance = nullptr;

Game::Game(const GameConfig& cfg)
    : config(cfg)
    , currentState(State::INIT)
    , gameForceControl(false) {
    engine.seed(static_cast<unsigned>(time(nullptr)));

    // load dialogue.json and commandData.json
    std::ifstream file("dialogue.json");
    if (!file) {
        std::cerr << "Error: Could not open dialogue.json" << std::endl;
        return;
    }
    file >> dialogueData;
    file.close();

    std::ifstream file2("command.json");
    if (!file2) {
        std::cerr << "Error: Could not open command.json" << std::endl;
        return;
    }
    file2 >> commandData;
    file2.close();
}

std::shared_ptr<Game> Game::getInstance(const GameConfig& config) {
    if (instance == nullptr) {
        instance = std::shared_ptr<Game>(new Game(config));
    }
    return instance;
}

void Game::initGame() {

    players.clear();
    if (config.getMode() == GameMode::RELEASE) {
        int playerCount;
        std::cout << dialogueData["input_player_num"]["prompt"].get<std::string>();
        std::cin >> playerCount;
        if (playerCount < 2)
            playerCount = 2;
        if (playerCount > 4)
            playerCount = 4;

        for (int i = 0; i < playerCount; i++) {
            std::cout << "Please enter the name of player " << (i + 1) << ": ";
            std::string name;
            std::cin >> name;
            players.push_back(std::make_shared<Player>(name, config.getPlayerIcons()[i], config.getPlayerColors()[i], config.getStartMoney()));
        }
    } else {
        for (int i = 0; i < config.getPlayersNum(); i++) {
            players.push_back(
                std::make_shared<Player>(config.getPlayerNames()[i], config.getPlayerIcons()[i], config.getPlayerColors()[i], config.getStartMoney()));
            for (auto card : CardStore::getInstance(config)->getCards()) {
                players[i]->addCard(card);
            }
        }
    }
    board = Board::getInstance(config);
    board->init(config, players);
}

void Game::start() {
    // Drawing the opening animation
    if (config.getAnimation()) {
        board->drawMonopolyAscii();
    }
    // Display dialogue
    std::cout << playerAction()["prompt"].get<std::string>() << std::endl;
    for (const auto& option : playerAction()["options"]) {
        std::cout << option["key"].get<std::string>() << ": " << option["description"].get<std::string>() << std::endl;
    }
    setState("start");
    std::cout << "Press any key to start the game...";
    InputManager::getKey();

    // Main game loop
    while (isRoundState()) {
        for (auto& p : players) {
            p->setMyTurn(true);
            setState("start");
            board->drawBoard();
            // std::cout << "It's " << p->getName() << "'s turn." << std::endl;
            if (p->isInHospital()) {
                p->updateHospitalStatus();
                setState("moved");
            }
            // Player Round
            while (this->isActivateState()) {
                processPlayerAction(p, board->getTile(p->getPosition()));
                if (currentState == State::START) {
                    board->drawBoard();
                }
            }
            p->setMyTurn(false);

            if (p->isBankrupt()) {
                std::cout << "player " << p->getName() << " Bankrupt, skip the action." << std::endl;
                continue;
            }
            checkGameOver();
            if (!isRoundState()) {
                break;
            }
        }
    }
    if (currentState == State::INIT) {
        this->initGame();
        this->start();
    }
    if (currentState == State::FINISH) {
        endGame();
    }
}

void Game::processPlayerAction(std::shared_ptr<Player> player, std::shared_ptr<Tile> tile) {
    if (!isActivateState()) {
        return;
    }
    TileAction action = TileAction::NONE;
    nlohmann::json nowPlayerAction = playerAction();
    //----------------------------------
    // State::MOVED, get the action to be performed on the current tile
    if (currentState == State::MOVED) {
        action = tile->landOn(player);
        switch (action) {
        case TileAction::PURCHASE_PROPERTY:
            nowPlayerAction = playerAction("property_unowned");
            std::cout << "\n" << nowPlayerAction["prompt"].get<std::string>() << std::endl;
            std::cout << "Property price: " << std::static_pointer_cast<PropertyTile>(tile)->getCurrentPrice() << std::endl;
            break;
        case TileAction::OWN:
        {
            nowPlayerAction = playerAction("property_owned");
            std::cout << "\n" << nowPlayerAction["prompt"].get<std::string>() << std::endl;
            std::shared_ptr<PropertyTile> propertyTile = std::static_pointer_cast<PropertyTile>(tile);
            if (propertyTile->getPropertyLevel() != PropertyLevel::LEVEL3) {
                std::cout << "Upgrade cost: " << propertyTile->getUpgradeCost() << std::endl;
            }
            std::cout << "Sell value: " << propertyTile->getCurrentPrice() << std::endl;
            break;
        }
        case TileAction::PAY_TOLL:
            nowPlayerAction = playerAction("property_toll");
            std::cout << "\n" << nowPlayerAction["prompt"].get<std::string>() << std::endl;
            std::cout << "tile owner: " << std::static_pointer_cast<PropertyTile>(tile)->getPropertyOwner()->getName() << std::endl;
            break;
        case TileAction::HOSPITAL:
            if (player->isInHospital()) {
                nowPlayerAction = playerAction("hospital");
                std::cout << "\n" << nowPlayerAction["prompt"].get<std::string>() << std::endl;
            } else {
                action = TileAction::NONE;
                nowPlayerAction = playerAction("default");
                std::cout << "You're not sick, so why are you in the hospital? Got lost on your way to victory?" << std::endl;
            }
            break;
        case TileAction::SPECIAL_EVENT:
            nowPlayerAction = playerAction("event");
            std::cout << "\n" << nowPlayerAction["prompt"].get<std::string>() << std::endl;
            break;
        case TileAction::START_POINT:
            nowPlayerAction = playerAction("start_point");
            std::cout << "\n" << nowPlayerAction["prompt"].get<std::string>() << std::endl;
            std::cout << "Receive bonus: " << std::static_pointer_cast<StartTile>(tile)->getBonus() << std::endl;
            break;
        case TileAction::STORE:
            nowPlayerAction = playerAction("store");
            std::cout << "\n" << nowPlayerAction["prompt"].get<std::string>() << std::endl;
            break;
        default:
            std::cout << tile->getName() << std::endl;
            break;
        }
    }
    //----------------------------------
    // Player input
    std::cout << "\n" << playerAction()["option_prompt"].get<std::string>() << std::endl;

    // Display current tile which key can be pressed
    for (const auto& option : nowPlayerAction["options"]) {
        std::cout << option["key"].get<std::string>() << ": " << option["description"].get<std::string>() << std::endl;
    }

    // Check player input
    char key;

    bool validInput = false;
    std::string input;
    while (!validInput) {
        input.clear();
        key = InputManager::getKey();
        std::cout << key;
        if (key == '/') {
            std::getline(std::cin, input);
            validInput = processCommand(player, input);
            if (!validInput) {
                std::cout << commandData["invalid_command"]["prompt"].get<std::string>() << std::endl;
                std::cout << "\nPress any key to continue...";
                InputManager::getKey();
            }
            // processPlayerAction(player, tile);
            return;
        } else {
            std::cout << std::endl;
            for (const auto& option : playerAction()["options"]) {
                if (option["key"].get<std::string>()[0] == key || option["key"].get<std::string>()[0] == '*') {
                    validInput = true;
                    break;
                }
            }
            if (!validInput) {
                std::cout << dialogueData["invalid_input"]["prompt"].get<std::string>() << std::endl;
            }
        }
    }
    //----------------------------------
    // Process the player action based on the input key
    switch (key) {
    case 'R':
        ++currentState;
        switch (action) {
        case TileAction::PURCHASE_PROPERTY:
            std::static_pointer_cast<PropertyTile>(tile)->purchase(player);
            break;
        case TileAction::OWN:
            std::static_pointer_cast<PropertyTile>(tile)->upgrade(player);
            break;
        case TileAction::PAY_TOLL:
            std::static_pointer_cast<PropertyTile>(tile)->payToll(player);
            break;
        case TileAction::HOSPITAL:
            std::static_pointer_cast<HospitalTile>(tile)->handleHospitalChoice(player);
            break;
        }
        std::cout << "\nPress any key to continue...";
        InputManager::getKey();
        break;
    case 'E':
        ++currentState;
        if (action == TileAction::STORE) {
            std::static_pointer_cast<StoreTile>(tile)->enterStore(player);
        }
        break;
    case 'S':
        ++currentState;
        if (action == TileAction::OWN) {
            std::static_pointer_cast<PropertyTile>(tile)->sell(player);
        }
        break;
    case 'I':

        player->displayCards(players);
        break;
    case 'P':
        std::cout << "Opening the player trading interface (to be implemented)." << std::endl;
        break;
    case 'T':
        if (currentState == State::START) {
            ++currentState;
            throwDice(player);
            break;
        }
    // input any key to continue
    default:
        ++currentState;
        if (action == TileAction::SPECIAL_EVENT) {
            std::static_pointer_cast<EventTile>(tile)->triggerEvent(player);
            break;
        } else if (action == TileAction::START_POINT) {
            player->addMoney(std::static_pointer_cast<StartTile>(tile)->getBonus());
            std::cout << "You received a bonus of " << std::static_pointer_cast<StartTile>(tile)->getBonus() << " dollars!" << std::endl;
            break;
        }
        std::cout << "Action Pass." << std::endl;
        break;
    }
}

bool Game::processCommand(std::shared_ptr<Player> player, const std::string& input) {
    std::istringstream iss(input);
    std::vector<std::string> tokens;
    std::string token;
    while (iss >> token) {
        tokens.push_back(token);
    }

    if (tokens.empty()) {
        return false;
    }

    // Check if the command exists in JSON
    std::string command = tokens[0];
    if (commandData.contains(command) || command == "test") {
        nlohmann::json currCommandData = commandData[command];
        if (command == "move") {
            if (tokens.size() < 2) {
                std::cout << "Usage: " << currCommandData["usage"].get<std::string>() << std::endl;
                return false;
            }

            std::string location;

            // Handle "/move to X" format
            if (tokens.size() == 3 && tokens[1] == "to") {
                location = tokens[2]; // Extract the actual location
            } else {
                location = tokens[1]; // Regular "/move X" case
            }

            int newPos = -1; // Invalid position by default

            // Check if input is a number (direct position)
            if (std::all_of(location.begin(), location.end(), ::isdigit)) {
                newPos = std::stoi(location);
            }

            // Check if input is a named location (e.g., "USA")
            std::vector<std::shared_ptr<Tile>> tmpTileList = board->getTileList();
            auto it = std::find_if(tmpTileList.begin(), tmpTileList.end(), [&](const std::shared_ptr<Tile>& tile) {
                return tile->getName() == location;
            });
            if (it != tmpTileList.end()) {
                newPos = static_cast<int>(std::distance(tmpTileList.begin(), it));
            }

            // If the location is still invalid, return an error
            if (newPos == -1) {
                std::cout << "Error: Invalid location. Please enter a valid number (0-31) or a named location (USA, START, etc.).\n";
                return false;
            }

            // Replace {location} in the prompt message
            std::string prompt = currCommandData["prompt"].get<std::string>();
            size_t pos = prompt.find("{location}");
            if (pos != std::string::npos) {
                prompt.replace(pos, std::string("{location}").length(), location);
            }

            std::cout << prompt << std::endl;

            // Move the player to the new position
            setState("moved");
            player->setPosition(newPos);
            processPlayerAction(player, board->getTile(newPos));
            board->drawBoard();
            return true;
        } else if (command == "give") {
            if (tokens.size() < 3) {
                std::cout << "Usage: " << currCommandData["usage"].get<std::string>() << std::endl;
                return false;
            }

            std::string playerName = tokens[1];
            int amount;

            try {
                amount = std::stoi(tokens[2]);
            } catch (const std::invalid_argument&) {
                std::cout << "Error: Invalid amount. Please enter a valid number." << std::endl;
                return false;
            } catch (const std::out_of_range&) {
                std::cout << "Error: Amount out of range. Please enter a valid number." << std::endl;
                return false;
            }

            auto it = std::find_if(players.begin(), players.end(), [&](const std::shared_ptr<Player>& p) {
                return p->getName() == playerName;
            });
            if (player->deductMoney(amount)) {
                if (it != players.end()) {
                    (*it)->addMoney(amount);

                    std::string prompt = currCommandData["prompt"].get<std::string>();
                    prompt.replace(prompt.find("{playerName}"), 12, playerName);
                    prompt.replace(prompt.find("{money}"), 7, tokens[2]);
                    std::cout << prompt << std::endl;
                    return true;
                } else {
                    std::cout << "Error: Player not found." << std::endl;
                    return false;
                }
            } else {
                std::cout << "Error: Not enough money." << std::endl;
            }
            return true;
        } else if (command == "get") {
            if (tokens.size() < 2) {
                std::cout << "Usage: " << currCommandData["usage"].get<std::string>() << std::endl;
                return false;
            }

            std::string playerName;
            int amount;

            if (tokens.size() == 2) {
                playerName = player->getName();
                try {
                    amount = std::stoi(tokens[1]);
                } catch (const std::invalid_argument&) {
                    std::cout << "Error: Invalid amount. Please enter a valid number." << std::endl;
                    return false;
                } catch (const std::out_of_range&) {
                    std::cout << "Error: Amount out of range. Please enter a valid number." << std::endl;
                    return false;
                }
            } else {
                playerName = tokens[1];
                try {
                    amount = std::stoi(tokens[2]);
                } catch (const std::invalid_argument&) {
                    std::cout << "Error: Invalid amount. Please enter a valid number." << std::endl;
                    return false;
                } catch (const std::out_of_range&) {
                    std::cout << "Error: Amount out of range. Please enter a valid number." << std::endl;
                    return false;
                }
            }

            auto it = std::find_if(players.begin(), players.end(), [&](const std::shared_ptr<Player>& p) {
                return p->getName() == playerName;
            });

            if (it != players.end()) {
                (*it)->addMoney(amount);

                std::string prompt = currCommandData["prompt"].get<std::string>();
                prompt.replace(prompt.find("{playerName}"), 12, playerName);
                prompt.replace(prompt.find("{money}"), 7, std::to_string(amount));
                std::cout << prompt << std::endl;
                return true;
            } else {
                std::cout << "Error: Player not found." << std::endl;
                return false;
            }
        } else if (command == "card") { // todo
            if (tokens.size() < 2) {
                std::cout << "Usage: " << currCommandData["usage"].get<std::string>() << std::endl;
                return false;
            }

            std::string cardName = "";
            for (int i = 1; i < tokens.size(); i++) {
                if (i > 1)
                    cardName += " ";
                cardName += tokens[i];
            }

            std::shared_ptr<Card> targetCard = nullptr;
            for (const auto& card : CardStore::getInstance()->getCards()) {
                if (card->getName() == cardName) {
                    targetCard = card;
                    break;
                }
            }

            if (!targetCard) {

                std::cout << "Available cards name:" << std::endl;
                std::vector<std::shared_ptr<Card>> availableCards = CardStore::getInstance()->getCards();
                for (int i = 0; i < availableCards.size(); i++) {
                    std::cout << i + 1 << ". " << availableCards[i]->getName() << std::endl;
                }
                return false;
            }

            player->addCard(targetCard);
            std::string prompt = currCommandData["prompt"].get<std::string>();
            prompt.replace(prompt.find("{card_name}"), 11, cardName);
            std::cout << prompt << std::endl << std::endl;
            return true;
        } else if (command == "minigame") {
            MiniGameManager::startMiniGame(player);
            return true;
        } else if (command == "gamestate") {
            if (tokens.size() < 2) {
                std::cout << "Usage: " << currCommandData["usage"].get<std::string>() << std::endl;
                return false;
            }
            std::string newState = tokens[1];
            std::string prompt = currCommandData["prompt"].get<std::string>();
            prompt.replace(prompt.find("{state}"), 7, newState);
            gameForceControl = true;
            setState(newState);
            std::cout << prompt << std::endl;
            std::cout << "Current state: " << getStateString() << std::endl;
            return true;
        } else if (command == "info") {
            for (const auto& p : players) {
                std::cout << "+----------------+------------+------------+------------+\n";
                std::cout << "| Player         | Money      | Position   | Status     |\n";
                std::cout << "+----------------+------------+------------+------------+\n";

                for (const auto& p : players) {
                    std::string playerName = p->getDisplayName();
                    std::cout << "| " << std::setw(15 + (playerName.length() - stripAnsi(playerName).length())) << std::left << playerName << "| " << std::setw(11) << std::left
                              << p->getMoney() << "| " << std::setw(11) << std::left << p->getPosition() << "| " << std::setw(10) << std::left
                              << (p->isBankrupt() ? "Bankrupt" : (p->isInHospital() ? "In Hospital" : "Active")) << " |\n";
                }
                std::cout << "+----------------+------------+------------+------------+\n";
            }
            return true;
        } else if (command == "refresh") {
            board->drawBoard();
            return true;
        } else if (command == "list" || command == "help") {
            bool showAll = (tokens.size() > 1 && tokens[1] == "-a");

            for (const auto& item : commandData.items()) {
                const std::string& command = item.key(); // Get the JSON key
                const auto& cmdData = item.value();      // Get the corresponding value

                if (command != "invalid_command" && command != "list") { // Exclude invalid commands
                    std::cout << "/" << command << " - " << cmdData["description"].get<std::string>() << std::endl;

                    if (showAll && cmdData.contains("usage")) {
                        std::cout << "   Usage:   " << cmdData["usage"].get<std::string>() << std::endl;
                        if (cmdData.contains("examples")) {
                            std::cout << "   Example: " << std::endl;
                            for (const auto& example : cmdData["examples"]) {
                                std::cout << "            " << example.get<std::string>() << std::endl;
                            }
                        }
                    }
                }
            }
            return true;
        } else if (command == "test") {
            MiniGameManager::startMiniGame(player);
            return true;
        }
    }
    return false; // Invalid command
}

void Game::throwDice(std::shared_ptr<Player> player) {
    int steps;
    int diceControl = player->getDiceControl();
    if (diceControl > 0) {
        steps = diceControl;
        movePlayer(player, steps);
        player->setDiceControl(0);
        board->drawBoard();
        std::cout << "Dice control activated: Move " << steps << " steps" << std::endl;

    } else {
        std::uniform_int_distribution<int> dist(1, 6);
        int d1 = dist(engine);
        int d2 = dist(engine);

        steps = d1 + d2;
        movePlayer(player, steps);
        board->drawBoard();
        std::cout << "Dice roll result: (" << d1 << ", " << d2 << ") -> Move forward " << steps << " steps" << std::endl;
    }
}

void Game::checkGameOver() {
    int aliveCount = 0;
    for (auto& p : players) {
        if (!p->isBankrupt())
            aliveCount++;
    }
    if (aliveCount <= 1) {
        changeState(State::FINISH);
    }
    // Check if any player has reached the winning money
    for (auto& p : players) {
        if (p->getMoney() >= config.getWinMoney()) {
            changeState(State::FINISH);
        }
    }
}

void Game::endGame() {
    std::cout << "\n==== Game Result ====\n";
    std::sort(players.begin(), players.end(), [](auto& a, auto& b) {
        if (a->isBankrupt() && !b->isBankrupt())
            return false;
        if (!a->isBankrupt() && b->isBankrupt())
            return true;
        return a->getMoney() > b->getMoney();
    });
    for (size_t i = 0; i < players.size(); i++) {
        std::cout << (i + 1) << ". " << players[i]->getName() << " - Money: " << players[i]->getMoney() << (players[i]->isBankrupt() ? " (Bankrupt)" : "")
                  << std::endl;
    }
}

void Game::changeState(State newState) {
    currentState = newState;
}

State& operator++(State& state) {
    static const std::unordered_map<State, State> nextState = {
        {State::INIT, State::START},
        {State::START, State::MOVED},
        {State::MOVED, State::ROUND_END},
        {State::ROUND_END, State::START},
        {State::FINISH, State::FINISH}};

    auto it = nextState.find(state);
    if (it != nextState.end()) {
        state = it->second;
    }
    return state;
}

void Game::setState(const std::string& state) {
    std::string lowerState = state;
    std::transform(lowerState.begin(), lowerState.end(), lowerState.begin(), ::tolower);
    if (lowerState == "init") {
        currentState = State::INIT;
    } else if (lowerState == "start") {
        currentState = State::START;
    } else if (lowerState == "moved") {
        currentState = State::MOVED;
    } else if (lowerState == "round_end") {
        currentState = State::ROUND_END;
    } else if (lowerState == "finish") {
        currentState = State::FINISH;
    } else {
        std::cout << "Unknown state!" << std::endl;
    }
}

const nlohmann::json& Game::playerAction() {
    return dialogueData["player_action"][getStateString()];
}

const nlohmann::json& Game::playerAction(const std::string& key) {
    if (key == "" || key == "/")
        return dialogueData["player_action"][getStateString()]["default"];
    return dialogueData["player_action"][getStateString()][key];
}

void Game::movePlayer(std::shared_ptr<Player> player, int steps) {
    int currentPos = player->getPosition();
    int boardSize = board->getSize();
    int newPos = currentPos;
    std::shared_ptr<Tile> nextTile = nullptr;
    // Check each step in the path for barriers
    for (int i = 1; i <= steps; i++) {
        int nextPos = (currentPos + i) % boardSize;
        nextTile = board->getTile(nextPos);

        if (nextTile->isBlocked()) {
            std::cout << "A barrier is blocking the path at " << nextTile->getName() << std::endl;
            nextTile->setBlock(false);
            break;
        }
        newPos = nextPos;
    }

    // Update player position
    player->setPosition(newPos);
}

std::string Game::getStateString() {
    static const std::unordered_map<State, std::string> stateToString = {
        {State::INIT, "init"}, {State::START, "start"}, {State::MOVED, "moved"}, {State::ROUND_END, "round_end"}, {State::FINISH, "finish"}};

    auto it = stateToString.find(currentState);
    return (it != stateToString.end()) ? it->second : "unknown";
}

bool Game::isActivateState() const {
    return currentState == State::START || currentState == State::MOVED;
}
bool Game::isRoundState() const {
    return currentState == State::START || currentState == State::MOVED || currentState == State::ROUND_END;
}