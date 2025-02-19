#ifndef GAME_HPP
#define GAME_HPP

#include "Board.hpp"
#include "GameConfig.hpp"
#include "Player.hpp"
#include <memory>
#include <random>
#include <vector>

enum class State { INIT, START, MOVED, FINISH };
State& operator++(State& state);
class Game {
private:
    Board board;
    GameConfig config;
    State currentState;

    std::vector<std::shared_ptr<Player>> players;
    bool gameOver;
    static std::default_random_engine engine;

    void processPlayerAction(std::shared_ptr<Player> player, std::shared_ptr<Tile> tile);
    void throwDice(std::shared_ptr<Player> player);

public:
    Game(const GameConfig& config);
    void initGame();
    void start();
    void checkGameOver();
    void endGame();
    void changeState(State newState);
    std::string getStateString();
};

#endif // GAME_HPP