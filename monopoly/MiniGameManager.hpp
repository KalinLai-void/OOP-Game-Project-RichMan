#ifndef MINIGAMEMANAGER_HPP
#define MINIGAMEMANAGER_HPP

#include "Player.hpp"
#include <memory>
#include <vector>

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
};

#endif // MINIGAMEMANAGER_HPP