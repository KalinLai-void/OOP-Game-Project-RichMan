#include "Game.hpp"
#include "Bank.hpp"
#include "InputManager.hpp"
#include <algorithm>
#include <ctime>
#include <iostream>
#include <random>

using namespace std;

std::default_random_engine Game::engine;

Game::Game(const GameConfig& cfg)
    : board(cfg), config(cfg), gameOver(false) {
    engine.seed(static_cast<unsigned>(time(nullptr)));
}

void Game::initGame() {
    if (config.getMode() == GameMode::RELEASE) {
        int playerCount;
        cout << "請輸入玩家人數 (2~4 之間)：";
        cin >> playerCount;
        if (playerCount < 2)
            playerCount = 2;
        if (playerCount > 4)
            playerCount = 4;

        for (int i = 0; i < playerCount; i++) {
            cout << "請輸入第 " << (i + 1) << " 位玩家名稱：";
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
    cout << "遊戲開始！" << endl;
    // 遊戲主迴圈：直到 gameOver 為 true
    while (!gameOver) {
        // 每個回合依序輪到各玩家行動
        for (auto& p : players) {
            // 畫出目前棋盤狀態
            board.drawBoard(players);

            // 若玩家在醫院則更新狀態並跳過此回合
            if (p->isInHospital()) {
                cout << "玩家 " << p->getName() << " 目前在醫院，無法行動。" << endl;
                p->updateHospitalStatus();
                // 暫停一下以便玩家觀察狀態 (可自行加入等待機制)
                continue;
            }

            // 進入玩家行動階段：先呼叫 processPlayerAction() 選單
            // 玩家可以選擇直接擲骰子 (按 T) 或進行其他操作
            processPlayerAction(p, board.getTile(p->getPosition()));

            // 檢查玩家是否破產
            if (p->isBankrupt()) {
                cout << "玩家 " << p->getName() << " 已破產，跳過行動。" << endl;
                continue;
            }

            // 檢查是否觸發遊戲結束條件
            checkGameOver();
            if (gameOver)
                break;
        }
    }
    endGame();
}

void Game::processPlayerAction(std::shared_ptr<Player> player, std::shared_ptr<Tile> tile) {
    // 根據 tile 類型進行額外的動作處理，這裡以房產操作為例
    // 你可以擴充其他類型的 tile 對應的操作
    cout << "\n請選擇操作：" << endl;
    cout << "R: 購買/升級房產" << endl;
    cout << "S: 出售房產" << endl;
    cout << "I: 查看道具" << endl;
    cout << "P: 發起交易" << endl;
    cout << "T: 擲骰子" << endl;
    cout << "請按下對應按鍵：";

    char key = InputManager::getKey();
    cout << key << endl; // 顯示玩家所按的鍵

    switch (key) {
    case 'R':
    case 'r':
        // 此處假設 tile 為 PropertyTile，呼叫其購買或升級邏輯
        tile->landOn(player);
        break;
    case 'S':
    case 's':
        // 呼叫玩家出售房產的介面或邏輯（需根據實際設計實作）
        cout << "呼叫出售房產功能（待實作）" << endl;
        break;
    case 'I':
    case 'i':
        // 呼叫查看道具卡介面（待實作）
        cout << "呼叫道具卡介面（待實作）" << endl;
        break;
    case 'P':
    case 'p':
        // 呼叫發起玩家交易介面（待實作）
        cout << "呼叫玩家交易介面（待實作）" << endl;
        break;
    case 'T':
    case 't':
        throwDice(player);
        break;
    default:
        cout << "無效按鍵，結束操作，繼續遊戲。" << endl;
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

    // 取得目前所在的 tile 並觸發 landOn()
    auto tile = board.getTile(newPos);
    if (tile) {
        tile->landOn(player);
    }
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

// #include "Game.hpp"
// #include "Bank.hpp"
// #include <algorithm>
// #include <ctime>
// #include <iostream>
// #include <random>
// using namespace std;

// std::default_random_engine Game::engine;

// Game::Game(const GameConfig& cfg)
//     : board(cfg), config(cfg), gameOver(false) {
//     engine.seed(static_cast<unsigned>(time(nullptr)));
// }

// void Game::initGame() {
//     if (config.getMode() == GameMode::RELEASE) {
//         int playerCount;
//         cout << "請輸入玩家人數 (2~4 之間)：";
//         cin >> playerCount;
//         if (playerCount < 2)
//             playerCount = 2;
//         if (playerCount > 4)
//             playerCount = 4;

//         for (int i = 0; i < playerCount; i++) {
//             cout << "請輸入第 " << (i + 1) << " 位玩家名稱：";
//             std::string name;
//             cin >> name;
//             auto p = std::make_shared<Player>(name, config.getPlayerIcons()[i], 10000);
//             players.push_back(p);
//         }
//     } else {
//         for (int i = 0; i < config.getPlayersNum(); i++) {
//             players.push_back(std::make_shared<Player>(config.getPlayerNames()[i], config.getPlayerIcons()[i], config.getStartMoney()));
//         }
//     }
// }

// void Game::start() {
//     cout << "遊戲開始！" << endl;
//     while (!gameOver) {
//         for (auto& p : players) {
//             system("cls");
//             board.drawBoard(players); // 呼叫 Board::drawBoard() 顯示棋盤
//             if (p->isInHospital()) {
//                 cout << "玩家 " << p->getName() << " 目前在醫院，無法行動。" << endl;
//                 p->updateHospitalStatus();
//                 continue;
//             }
//             std::uniform_int_distribution<int> dist(1, 6);
//             int d1 = dist(engine);
//             int d2 = dist(engine);
//             int steps = d1 + d2;

//             cout << "\n-----------\n";
//             cout << "玩家 " << p->getName() << " 擲骰：(" << d1 << "," << d2 << ") → 前進 " << steps << " 步" << endl;

//             int newPos = (p->getPosition() + steps) % board.getSize();
//             p->setPosition(newPos);

//             auto tile = board.getTile(newPos);
//             if (tile) {
//                 tile->landOn(p);
//                 if (p->isBankrupt()) {
//                     cout << "玩家 " << p->getName() << " 已破產，停止行動。" << endl;
//                 }
//             }
//             // 判斷雙骰狀況：數字相同時可再行動一次
//             if (d1 == d2 && !p->isBankrupt() && !p->isInHospital()) {
//                 cout << "擲出雙數，再行動一次！" << endl;
//                 int d3 = dist(engine);
//                 int d4 = dist(engine);
//                 int steps2 = d3 + d4;
//                 cout << "再擲一次結果：(" << d3 << "," << d4 << ") → 前進 " << steps2 << " 步" << endl;
//                 int newPos2 = (p->getPosition() + steps2) % board.getSize();
//                 p->setPosition(newPos2);
//                 auto tile2 = board.getTile(newPos2);
//                 if (tile2) {
//                     tile2->landOn(p);
//                     if (p->isBankrupt()) {
//                         cout << "玩家 " << p->getName() << " 已破產，停止行動。" << endl;
//                     }
//                 }
//             }
//             checkGameOver();
//             if (gameOver)
//                 break;
//         }
//     }
//     endGame();
// }

// void Game::checkGameOver() {
//     int aliveCount = 0;
//     for (auto& p : players) {
//         if (!p->isBankrupt())
//             aliveCount++;
//     }
//     if (aliveCount <= 1) {
//         gameOver = true;
//     }
//     for (auto& p : players) {
//         if (p->getMoney() >= 1000000) {
//             gameOver = true;
//             break;
//         }
//     }
// }

// void Game::endGame() {
//     cout << "\n==== 遊戲結束 ====\n";
//     std::sort(players.begin(), players.end(), [](auto& a, auto& b) {
//         if (a->isBankrupt() && !b->isBankrupt())
//             return false;
//         if (!a->isBankrupt() && b->isBankrupt())
//             return true;
//         return a->getMoney() > b->getMoney();
//     });
//     for (size_t i = 0; i < players.size(); i++) {
//         cout << (i + 1) << ". " << players[i]->getName() << " - 資金：" << players[i]->getMoney() << (players[i]->isBankrupt() ? " (破產)" : "") << endl;
//     }
// }
