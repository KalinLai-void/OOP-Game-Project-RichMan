#ifndef MINIGAMEMANAGER_HPP
#define MINIGAMEMANAGER_HPP

#include "Player.hpp"
#include <memory>
#include <vector>

class MiniGameManager {
public:
    void startMiniGame(std::shared_ptr<Player> player);
    void endMiniGame(std::shared_ptr<Player> player);
    void listMiniGames() const;
};

#endif // MINIGAMEMANAGER_HPP
