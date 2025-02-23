#include "Game.hpp"
#include "Bank.hpp"
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
using namespace std;

std::default_random_engine Game::engine;

Game::Game(const GameConfig& cfg)
    : board(cfg), config(cfg) {
    engine.seed(static_cast<unsigned>(time(nullptr)));
    State currentState = State::INIT;

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
    //cout << commandData.dump();
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
            Board::clearScreen();
            // board.drawBoard(players);
            cout << "It's " << p->getName() << "'s turn." << endl;
            if (p->isInHospital()) {
                cout << "You're in the hospital. You can't move." << endl;
                p->updateHospitalStatus();
                delayTime(config.getAnimationSecond());
                continue;
            }
            // start
            processPlayerAction(p, board.getTile(p->getPosition()));
            ++currentState;

            // moved
            processPlayerAction(p, board.getTile(p->getPosition()));
            ++currentState;
            Board::clearScreen();

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
        } else {
            cout << endl;
            for (const auto& option : playerAction()["options"]) {

                if (option["key"].get<std::string>()[0] == key || option["key"].get<std::string>()[0] == '*') {
                    validInput = true;
                    break;
                }

                if (!validInput) {
                    cout << dialogueData["invalid_input"]["prompt"].get<std::string>() << endl;
                }
            }
        }
        
    }
    //----------------------------------
    switch (key) {
    case 'R':
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
    case 'S':
        if (action == TileAction::OWN) {
            static_pointer_cast<PropertyTile>(tile)->sell(player);
        }
    case 'I':
        cout << "Opening the item card interface (to be implemented)." << endl;
        break;
    case 'P':
        cout << "Opening the player trading interface (to be implemented)." << endl;
        break;
    case 'T':
        throwDice(player);
        break;
    // input any key to continue
    default:
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

    std::string command = tokens[0];

    // Check if the command exists in JSON
    if (commandData.contains(command)) {
        nlohmann::json tmpCommandData = commandData[command];
        if (command == "move") {
            if (tokens.size() < 2) {
                std::cout << "Usage: " << tmpCommandData["usage"].get<std::string>() << std::endl;
                return false;
            }
            int newPos = std::stoi(tokens[1]);
            std::cout << tmpCommandData["prompt"].get<std::string>().replace(tmpCommandData["prompt"].get<std::string>().find("{location}"), 9, tokens[1])
                      << std::endl;
            player->setPosition(newPos);
            return true;
        }

        if (command == "give") {
            if (tokens.size() < 3) {
                std::cout << "Usage: " << tmpCommandData["usage"].get<std::string>() << std::endl;
                return false;
            }
            std::string playerName = tokens[1];
            int amount = std::stoi(tokens[2]);
            std::string prompt = tmpCommandData["prompt"].get<std::string>();
            prompt.replace(prompt.find("{playerName}"), 11, playerName);
            prompt.replace(prompt.find("{money}"), 7, tokens[2]);
            std::cout << prompt << std::endl;
            return true;
        }

        if (command == "card") {
            if (tokens.size() < 2) {
                std::cout << "Usage: " << tmpCommandData["usage"].get<std::string>() << std::endl;
                return false;
            }
            std::string cardName = tokens[1];
            std::string prompt = tmpCommandData["prompt"].get<std::string>();
            prompt.replace(prompt.find("{card_name}"), 11, cardName);
            std::cout << prompt << std::endl;
            return true;
        }

        if (command == "gamestate") {
            if (tokens.size() < 2) {
                std::cout << "Usage: " << tmpCommandData["usage"].get<std::string>() << std::endl;
                return false;
            }
            std::string newState = tokens[1];
            std::string prompt = tmpCommandData["prompt"].get<std::string>();
            prompt.replace(prompt.find("{state}"), 7, newState);
            std::cout << prompt << std::endl;
            return true;
        }

        if (command == "list") {
            std::cout << tmpCommandData["prompt"].get<std::string>() << std::endl;
            return false;
        }
    } else {
        std::cout << commandData["invalid_command"]["prompt"].get<std::string>() << std::endl;
        return false;
    }

    return false;
    // // 解析輸入
    // while (iss >> token) {
    //     tokens.push_back(token);
    // }

    // if (tokens.empty())
    //     return false;

    // if (tokens[0] == "move") {
    //     if (tokens.size() < 2)
    //         return false;
    //     int newPos = std::stoi(tokens[1]);
    //     cout << "移動玩家到 " << newPos << " 格。" << endl;
    //     player->setPosition(newPos);
    //     return true;
    // }

    // if (tokens[0] == "give") { // todo
    //     if (tokens.size() < 3)
    //         return false;
    //     int targetPlayer = std::stoi(tokens[1]);
    //     int amount = std::stoi(tokens[2]);
    //     cout << "給玩家 " << targetPlayer << " " << amount << " 元。" << endl;
    //     // 在這裡實作玩家之間的交易邏輯
    //     return true;
    // }

    // if (tokens[0] == "list") {
    //     cout << "可用指令：" << endl;
    //     cout << "/move [位置] - 移動到指定位置" << endl;
    //     cout << "/give [玩家ID] [金額] - 給指定玩家金額" << endl;
    //     cout << "/list - 顯示可用指令" << endl;
    //     return true;
    // }

    // return false; // 指令不匹配
}

void Game::throwDice(std::shared_ptr<Player> player) {
    Board::clearScreen();
    std::uniform_int_distribution<int> dist(1, 6);
    int d1 = dist(engine);
    int d2 = dist(engine);
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
    switch (currentState) {
    case State::INIT:
        return "init";
    case State::START:
        return "start";
    case State::MOVED:
        return "moved";
    case State::FINISH:
        return "end";
    default:
        std::cout << "Unknown state!" << std::endl;
        return "unknown";
    }
}

State& operator++(State& state) {
    switch (state) {
    case State::INIT:
        state = State::START;
        break;
    case State::START:
        state = State::MOVED;
        break;
    case State::MOVED:
        state = State::START;
        break;
    default:
        state = State::FINISH;
        break;
    }
    return state;
}

const nlohmann::json& Game::playerAction() {
    return dialogueData["player_action"][getStateString()];
}

const nlohmann::json& Game::playerAction(const string& key) {
    if (key == "" || key == "/")
        return dialogueData["player_action"][getStateString()]["default"];
    return dialogueData["player_action"][getStateString()][key];
}