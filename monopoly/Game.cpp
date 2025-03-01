#include "Game.hpp"
#include "Bank.hpp"
#include "DiceControlCard.hpp"
#include "MiniGameManager.hpp"
#include "EventTile.hpp"
#include "HospitalTile.hpp"
#include "InputManager.hpp"
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

using namespace std;

std::default_random_engine Game::engine;

Game::Game(const GameConfig& cfg)
    : board(cfg), config(cfg), currentState(State::INIT) {
    engine.seed(static_cast<unsigned>(time(nullptr)));

    // load dialogue.json and commandData.json
    std::ifstream file("dialogue.json");
    if (!file) {
        cerr << "Error: Could not open dialogue.json" << endl;
        return;
    }
    file >> dialogueData;
    file.close();

    std::ifstream file2("command.json");
    if (!file2) {
        cerr << "Error: Could not open command.json" << endl;
        return;
    }
    file2 >> commandData;
    file2.close();
}

void Game::initGame() {
    if (config.getMode() == GameMode::RELEASE) {
        int playerCount;
        cout << dialogueData["input_player_num"]["prompt"].get<std::string>();
        cin >> playerCount;
        if (playerCount < 2)
            playerCount = 2;
        if (playerCount > 4)
            playerCount = 4;

        for (int i = 0; i < playerCount; i++) {
            cout << "Please enter the name of player " << (i + 1) << ": ";
            std::string name;
            cin >> name;
            auto p = std::make_shared<Player>(name, config.getPlayerIcons()[i], 10000);
            players.push_back(p);
        }
    } else {
        for (int i = 0; i < config.getPlayersNum(); i++) {
            players.push_back(std::make_shared<Player>(config.getPlayerNames()[i], config.getPlayerIcons()[i], config.getStartMoney()));
        }
    }
}

void Game::start() {
    // Display dialogue
    cout << "\n" << playerAction()["prompt"].get<std::string>() << endl;
    for (const auto& option : playerAction()["options"]) {
        cout << option["key"].get<std::string>() << ": " << option["description"].get<std::string>() << endl;
    }

    // Set start state
    delayTime(config.getAnimationSecond());
    ++currentState;

    // Main game loop
    while (currentState != State::FINISH) {
        for (auto& p : players) {
            board.drawBoard(players);
            cout << "It's " << p->getName() << "'s turn." << endl;
            if (p->isInHospital()) {
                cout << "You're in the hospital. You can't move." << endl;
                p->updateHospitalStatus();
                delayTime(config.getAnimationSecond());
                continue;
            }
            // start
            while (currentState == State::START) {
                processPlayerAction(p, board.getTile(p->getPosition()));
            }
            //++currentState;

            // moved
            while (currentState == State::MOVED) {
                processPlayerAction(p, board.getTile(p->getPosition()));
            }

            //++currentState;

            if (p->isBankrupt()) {
                cout << "player " << p->getName() << " Bankrupt, skip the action." << endl;
                continue;
            }

            if (checkGameOver())
                break;
        }
    }
    endGame();
}

void Game::processPlayerAction(std::shared_ptr<Player> player, std::shared_ptr<Tile> tile) {
    TileAction action = TileAction::NONE;
    nlohmann::json nowPlayerAction = playerAction();
    //----------------------------------
    // State::MOVED
    if (currentState == State::MOVED) {
        action = tile->landOn(player);
        switch (action) {
        case TileAction::PURCHASE_PROPERTY:
            nowPlayerAction = playerAction("property_unowned");
            cout << "\n" << nowPlayerAction["prompt"].get<std::string>() << endl;
            cout << "Property price: " << static_pointer_cast<PropertyTile>(tile)->getCurrentPrice() << endl;
            break;
        case TileAction::OWN:
            nowPlayerAction = playerAction("property_owned");
            cout << "\n" << nowPlayerAction["prompt"].get<std::string>() << endl;
            cout << "Upgrade cost: " << static_pointer_cast<PropertyTile>(tile)->getUpgradeCost() << endl;
            cout << "Sell value: " << static_pointer_cast<PropertyTile>(tile)->getCurrentPrice() << endl;
            break;
        case TileAction::PAY_TOLL:
            nowPlayerAction = playerAction("property_toll");
            cout << "\n" << nowPlayerAction["prompt"].get<std::string>() << endl;
            cout << "tile owner: " << static_pointer_cast<PropertyTile>(tile)->getPropertyOwner()->getName() << endl;
            break;
        case TileAction::HOSPITAL:
            nowPlayerAction = playerAction("default");
            break;
        case TileAction::SPECIAL_EVENT:
            nowPlayerAction = playerAction("event");
            cout << "\n" << nowPlayerAction["prompt"].get<std::string>() << endl;
            break;
        case TileAction::START:
            nowPlayerAction = playerAction("start_point");
            cout << "\n" << nowPlayerAction["prompt"].get<std::string>() << endl;
            cout << "Receive bonus: " << static_pointer_cast<StartTile>(tile)->getBonus() << endl;
            break;
        case TileAction::STORE:
            nowPlayerAction = playerAction("store");
            cout << "\n" << nowPlayerAction["prompt"].get<std::string>() << endl;
            break;
        default:
            cout << tile->getName() << endl;
            break;
        }
    }
    //----------------------------------
    cout << "\n" << playerAction()["option_prompt"].get<std::string>() << endl;

    // Display current tile which key can be pressed
    for (const auto& option : nowPlayerAction["options"]) {
        cout << option["key"].get<std::string>() << ": " << option["description"].get<std::string>() << endl;
    }
    // Check player input
    char key;

    bool validInput = false;
    std::string input;
    while (!validInput) {
        input.clear();
        key = InputManager::getKey();
        cout << key;
        if (key == '/') {
            std::getline(std::cin, input);
            validInput = processCommand(player, input);
            if (!validInput) {
                cout << commandData["invalid_command"]["prompt"].get<std::string>() << endl;
            }
            processPlayerAction(player, tile);
            return;
        } else {
            cout << endl;
            for (const auto& option : playerAction()["options"]) {
                if (option["key"].get<std::string>()[0] == key || option["key"].get<std::string>()[0] == '*') {
                    validInput = true;
                    break;
                }
            }
            if (!validInput) {
                cout << dialogueData["invalid_input"]["prompt"].get<std::string>() << endl;
            }
        }
    }
    //----------------------------------
    switch (key) {
    case 'R':
        ++currentState;
        switch (action) {
        case TileAction::PURCHASE_PROPERTY:
            static_pointer_cast<PropertyTile>(tile)->purchase(player);
            break;
        case TileAction::OWN:
            static_pointer_cast<PropertyTile>(tile)->upgrade(player);
            break;
        case TileAction::PAY_TOLL:
            static_pointer_cast<PropertyTile>(tile)->payToll(player);
            break;
        case TileAction::HOSPITAL:
            static_pointer_cast<HospitalTile>(tile)->handleHospitalChoice(player);
            break;
        case TileAction::STORE:
            static_pointer_cast<StoreTile>(tile)->enterStore(player);
            break;
        }
        break;
    case 'S':
        ++currentState;
        if (action == TileAction::OWN) {
            static_pointer_cast<PropertyTile>(tile)->sell(player);
        }
        break;
    case 'I':
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "Player: " << player->getName() << std::endl;
        std::cout << "Position: " << player->getPosition() << std::endl;
        std::cout << "Money: " << player->getMoney() << std::endl;
        std::cout << "Status: " << (player->isBankrupt() ? "Bankrupt" : "Active") << std::endl;
        std::cout << "----------------------------------------" << std::endl;
        // card
        // std::cout << "Opening the item card interface (to be implemented)." << endl;
        player->displayCards(players, board);
        break;
    case 'P':
        cout << "Opening the player trading interface (to be implemented)." << endl;
        break;
    case 'T':
        ++currentState;
        throwDice(player);
        break;
    // input any key to continue
    default:
        ++currentState;
        if (action == TileAction::SPECIAL_EVENT) {
            static_pointer_cast<EventTile>(tile)->triggerEvent(player);
            break;
        }
        cout << "Action Pass." << endl;
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
        std::cout << commandData["invalid_command"]["prompt"].get<std::string>() << std::endl;
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
            std::vector<std::shared_ptr<Tile>> tmpTileList = board.getTileList();
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
            player->setPosition(newPos);
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
                std::cout << cardName << std::endl;
            }
            
            CardStore cardStore;

            std::shared_ptr<Card> targetCard = nullptr;
            for (const auto& card : cardStore.getCards()) {
                if (card->getName() == cardName) {
                    targetCard = card;
                    break;
                }
            }

            if (!targetCard) {
                std::string prompt = currCommandData["prompt"].get<std::string>();
                prompt.replace(prompt.find("{card_name}"), 11, cardName);
                std::cout << prompt << std::endl << std::endl;

                std::cout << "Available cards name:" << std::endl;
                std::vector<std::shared_ptr<Card>> availableCards = cardStore.getCards();
                for (int i = 0; i < availableCards.size(); i++) {
                    std::cout << i + 1 << ". " << availableCards[i]->getName() << std::endl;
                }
                return false;
            }



            player->addCard(targetCard);
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
            setState(newState);
            std::cout << prompt << std::endl;
            std::cout << "Current state: " << getStateString() << std::endl;
            return true;
        } else if (command == "info") {
            std::cout << "------------------------" << std::endl;
            for (const auto& p : players) {
                std::cout << "Player: " << p->getName() << std::endl;
                std::cout << "Position: " << p->getPosition() << std::endl;
                std::cout << "Money: " << p->getMoney() << std::endl;
                std::cout << "Status: " << (p->isBankrupt() ? "Bankrupt" : "Active") << std::endl;
                std::cout << "------------------------" << std::endl;
            }
            return true;
        } else if (command == "refresh") {
            board.drawBoard(players);
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
                            std::cout << "   Example: " << endl;
                            for (const auto& example : cmdData["examples"]) {
                                std::cout << "            " << example.get<std::string>() << std::endl;
                            }
                        }
                    }
                }
            }
            return true;
        } else if (command == "test") {
            player->sendToStart();
            return true;
        }
    }
    return false; // Invalid command
}

void Game::throwDice(std::shared_ptr<Player> player) {
    std::uniform_int_distribution<int> dist(1, 6);
    int d1 = dist(engine);
    int d2 = dist(engine);
    int diceControl = player->getDiceControl();
    if (diceControl > 0) {
        d1 = diceControl;
        player->setDiceControl(0);
    }

    int steps = d1 + d2;

    int newPos = (player->getPosition() + steps) % board.getSize();
    player->setPosition(newPos);

    // Draw the board
    board.drawBoard(players);
    cout << "\nDice roll result: (" << d1 << ", " << d2 << ") → Move forward " << steps << " steps" << endl;
}

bool Game::checkGameOver() {
    int aliveCount = 0;
    for (auto& p : players) {
        if (!p->isBankrupt())
            aliveCount++;
    }
    if (aliveCount <= 1) {
        changeState(State::FINISH);
        return true;
    }
    // Check if any player has reached the winning money
    for (auto& p : players) {
        if (p->getMoney() >= config.getWinMoney()) {
            changeState(State::FINISH);
            return true;
        }
    }
    return false;
}

void Game::endGame() {
    cout << "\n==== Game Result ====\n";
    std::sort(players.begin(), players.end(), [](auto& a, auto& b) {
        if (a->isBankrupt() && !b->isBankrupt())
            return false;
        if (!a->isBankrupt() && b->isBankrupt())
            return true;
        return a->getMoney() > b->getMoney();
    });
    for (size_t i = 0; i < players.size(); i++) {
        cout << (i + 1) << ". " << players[i]->getName() << " - Money: " << players[i]->getMoney() << (players[i]->isBankrupt() ? " (Bankrupt)" : "") << endl;
    }
}

void Game::changeState(State newState) {
    currentState = newState;
}

std::string Game::getStateString() {
    static const std::unordered_map<State, std::string> stateToString = {
        {  State::INIT,   "init"},
        { State::START,  "start"},
        { State::MOVED,  "moved"},
        {State::FINISH, "finish"}
    };

    auto it = stateToString.find(currentState);
    return (it != stateToString.end()) ? it->second : "unknown";
}

State& operator++(State& state) {
    static const std::unordered_map<State, State> nextState = {
        {  State::INIT,  State::START},
        { State::START,  State::MOVED},
        { State::MOVED,  State::START},
        {State::FINISH, State::FINISH}
    };

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
    } else if (lowerState == "finish") {
        currentState = State::FINISH;
    } else {
        std::cout << "Unknown state!" << std::endl;
    }
}

const nlohmann::json& Game::playerAction() {
    return dialogueData["player_action"][getStateString()];
}

const nlohmann::json& Game::playerAction(const string& key) {
    if (key == "" || key == "/")
        return dialogueData["player_action"][getStateString()]["default"];
    return dialogueData["player_action"][getStateString()][key];
}
