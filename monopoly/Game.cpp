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

using namespace std;

std::default_random_engine Game::engine;

Game::Game(const GameConfig& cfg)
    : board(cfg), config(cfg), gameOver(false) {
    engine.seed(static_cast<unsigned>(time(nullptr)));
    State currentState = State::INIT;

    // load dialogue.json
    std::ifstream file("dialogue.json");
    if (!file) {
        cerr << "Error: Could not open dialogue.json" << endl;
        return;
    }
    file >> dialogueData;
    file.close();
}

void Game::initGame() {
    cout << dialogueData["input_player_num"]["prompt"].get<std::string>();
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
    // 顯示對話
    cout << "\n" << playerAction()["prompt"].get<std::string>() << endl;
    for (const auto& option : playerAction()["options"]) {
        cout << option["key"].get<std::string>() << ": " << option["description"].get<std::string>() << endl;
    }

    // 設定開始狀態
    delayTime(config.getAnimationSecond());
    ++currentState;

    // 遊戲主迴圈
    while (!gameOver) {
        for (auto& p : players) {
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
            //  使用 processPlayerAction 處理玩家行動
            processPlayerAction(p, board.getTile(p->getPosition()));
            ++currentState;

            if (p->isBankrupt()) {
                cout << "player " << p->getName() << " Bankrupt, skip the action." << endl;
                continue;
            }

            checkGameOver();
            if (gameOver)
                break;
        }
    }
    endGame();
}

void Game::processPlayerAction(std::shared_ptr<Player> player, std::shared_ptr<Tile> tile) {
    // 執行對應的行動
    TileAction action = TileAction::NONE;
    if (currentState == State::MOVED) {
        action = tile->landOn(player);
        switch (action) {
        case TileAction::PURCHASE_PROPERTY:
            cout << "You can purchase " << tile->getName() << ". Press 'R' to purchase." << endl;
            break;
        case TileAction::OWN:
            cout << "You can upgrade or sell " << tile->getName() << ". Press 'R' to upgrade. Press 'S' to sell." << endl;
            cout << "Upgrade cost: " << static_pointer_cast<PropertyTile>(tile)->getUpgradeCost() << endl;
            cout << "Sell value: " << static_pointer_cast<PropertyTile>(tile)->getCurrentPrice() << endl;
            break;
        case TileAction::PAY_TOLL:
            cout << tile->getName() << " is owned by " << static_pointer_cast<PropertyTile>(tile)->getPropertyOwner()->getName()
                 << ". You need to pay a toll. Press 'R' to pay." << std::endl;
            break;
        case TileAction::HOSPITAL:
            cout << "You are in the hospital. Press 'R' to try your luck, or any other key to stay." << endl;
            break;
        case TileAction::SPECIAL_EVENT:
            cout << "Something exciting is about to happen! Press any key to uncover the secret." << endl;
            break;
        case TileAction::START:
            cout << "You are at the start point. You receive a bonus of " << static_pointer_cast<StartTile>(tile)->getBonus() << ". Press any key to continue."
                 << endl;
            break;
        case TileAction::STORE:
            cout << "You are at the store. Press 'R' to entry." << endl;
            break;
        default:
            cout << "No specific action available. Press 'T' to throw dice, 'I' for item card, 'P' for player trading." << endl;
            break;
        }
    }
    //----------------------------------
    // 顯示對話框
    cout << "\n" << playerAction()["prompt"].get<std::string>() << endl;

    // 顯示所有可選的行動
    for (const auto& option : dialogueData["player_action"]["default"]["options"]) {
        cout << option["key"].get<std::string>() << ": " << option["description"].get<std::string>() << endl;
    }

    char key;

    // 檢查玩家輸入是否有效
    bool validInput = false;
    while (!validInput) {
        key = InputManager::getKey();
        cout << key << endl; // 顯示玩家所按的鍵
        for (const auto& option : playerAction()["options"]) {
            if (option["key"].get<std::string>()[0] == key) {
                validInput = true;
                break;
            }
        }
        if (!validInput) {
            cout << dialogueData["invalid_input"]["prompt"].get<std::string>() << endl;
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
    case 'O':
        cout << "PASS" << endl;
        break;
    case 'T':
        throwDice(player);
        break;
    // input any key to continue
    default:
        if (action == TileAction::SPECIAL_EVENT)
            static_pointer_cast<EventTile>(tile)->triggerEvent(player);
        break;
    }
}

void Game::throwDice(std::shared_ptr<Player> player) {
    std::uniform_int_distribution<int> dist(1, 6);
    int d1 = dist(engine);
    int d2 = dist(engine);
    int steps = d1 + d2;
    cout << "\n擲骰子結果：" << "(" << d1 << ", " << d2 << ") → 前進 " << steps << " 步" << endl;

    int newPos = (player->getPosition() + steps) % board.getSize();
    player->setPosition(newPos);

    // 顯示更新後的棋盤狀態
    board.drawBoard(players);

    //// 取得目前所在的 tile 並觸發 landOn()
    // auto tile = board.getTile(newPos);
    // if (tile) {
    //     tile->landOn(player);
    // }
}

void Game::checkGameOver() {
    int aliveCount = 0;
    for (auto& p : players) {
        if (!p->isBankrupt())
            aliveCount++;
    }
    if (aliveCount <= 1) {
        gameOver = true;
    }
    for (auto& p : players) {
        if (p->getMoney() >= 1000000) {
            gameOver = true;
            break;
        }
    }
}

void Game::endGame() {
    cout << "\n==== 遊戲結束 ====\n";
    std::sort(players.begin(), players.end(), [](auto& a, auto& b) {
        if (a->isBankrupt() && !b->isBankrupt())
            return false;
        if (!a->isBankrupt() && b->isBankrupt())
            return true;
        return a->getMoney() > b->getMoney();
    });
    for (size_t i = 0; i < players.size(); i++) {
        cout << (i + 1) << ". " << players[i]->getName() << " - 資金：" << players[i]->getMoney() << (players[i]->isBankrupt() ? " (破產)" : "") << endl;
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

nlohmann::json& Game::playerAction() {
    return dialogueData["player_action"][getStateString()];
}