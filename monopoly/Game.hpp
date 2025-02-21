#ifndef GAME_HPP
#define GAME_HPP

#include "Board.hpp"
#include "GameConfig.hpp"
#include "Player.hpp"
#include "Tile.hpp"
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
    nlohmann::json dialogueData;

    std::vector<std::shared_ptr<Player>> players;
    static std::default_random_engine engine;

    void processPlayerAction(std::shared_ptr<Player> player, std::shared_ptr<Tile> tile);
    void throwDice(std::shared_ptr<Player> player);
    const nlohmann::json& playerAction();
    const nlohmann::json& playerAction(const std::string& key);

public:
    Game(const GameConfig& config);
    void initGame();
    void start();
    bool checkGameOver();
    void endGame();
    void changeState(State newState);
    std::string getStateString();
};

#endif // GAME_HPP