#include "Game.hpp"
#include "Bank.hpp"
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
            auto p = std::make_shared<Player>(name, config.getplayerIcons()[i], 10000);
            players.push_back(p);
        }
    } else {
        for (int i = 0; i < config.getPlayersNum(); i++) {
            players.push_back(std::make_shared<Player>(config.getPlayerNames()[i], config.getplayerIcons()[i], config.getStartMoney()));
        }
    }
}

void Game::start() {
    cout << "遊戲開始！" << endl;
    while (!gameOver) {
        for (auto& p : players) {
            system("cls");
            board.drawBoard(players); // 呼叫 Board::drawBoard() 顯示棋盤
            if (p->isInHospital()) {
                cout << "玩家 " << p->getName() << " 目前在醫院，無法行動。" << endl;
                p->updateHospitalStatus();
                continue;
            }
            std::uniform_int_distribution<int> dist(1, 6);
            int d1 = dist(engine);
            int d2 = dist(engine);
            int steps = d1 + d2;

            cout << "\n-----------\n";
            cout << "玩家 " << p->getName() << " 擲骰：(" << d1 << "," << d2 << ") → 前進 " << steps << " 步" << endl;

            int newPos = (p->getPosition() + steps) % board.getSize();
            p->setPosition(newPos);

            auto tile = board.getTile(newPos);
            if (tile) {
                tile->landOn(p);
                if (p->isBankrupt()) {
                    cout << "玩家 " << p->getName() << " 已破產，停止行動。" << endl;
                }
            }
            // 判斷雙骰狀況：數字相同時可再行動一次
            if (d1 == d2 && !p->isBankrupt() && !p->isInHospital()) {
                cout << "擲出雙數，再行動一次！" << endl;
                int d3 = dist(engine);
                int d4 = dist(engine);
                int steps2 = d3 + d4;
                cout << "再擲一次結果：(" << d3 << "," << d4 << ") → 前進 " << steps2 << " 步" << endl;
                int newPos2 = (p->getPosition() + steps2) % board.getSize();
                p->setPosition(newPos2);
                auto tile2 = board.getTile(newPos2);
                if (tile2) {
                    tile2->landOn(p);
                    if (p->isBankrupt()) {
                        cout << "玩家 " << p->getName() << " 已破產，停止行動。" << endl;
                    }
                }
            }
            checkGameOver();
            if (gameOver)
                break;
        }
    }
    endGame();
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
