#ifndef GAME_HPP
#define GAME_HPP

#include "Board.hpp"
#include "GameConfig.hpp"
#include "Player.hpp"
#include <memory>
#include <random>
#include <vector>

class Game {
private:
    Board board;
    std::vector<std::shared_ptr<Player>> players;
    bool gameOver;
    static std::default_random_engine engine;

public:
    Game();
    void initGame(const GameConfig& config);
    void start();
    void checkGameOver();
    void endGame();
};

#endif // GAME_HPP
