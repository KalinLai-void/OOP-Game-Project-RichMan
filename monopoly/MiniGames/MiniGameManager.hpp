#ifndef MINIGAMEMANAGER_HPP
#define MINIGAMEMANAGER_HPP

#include "Game/Player.hpp"
#include "MiniGame.hpp"
#include <memory>
#include <vector>
#include <iostream>

class MiniGameManager {
private:
    static std::vector<std::shared_ptr<Player>> players;
    MiniGameManager() = delete;
    MiniGameManager(const MiniGameManager&) = delete;
    MiniGameManager& operator=(const MiniGameManager&) = delete;

public:
    static void startMiniGame(std::shared_ptr<Player> player);
    static void endMiniGame(std::shared_ptr<Player> player);
    static void listMiniGames();
    static long long setBetAmount(std::shared_ptr<Player> player);
};

#endif // MINIGAMEMANAGER_HPP