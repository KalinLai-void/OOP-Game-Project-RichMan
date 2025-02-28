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

class MiniGameManager;

class Game {
private:
    Board board;
    GameConfig config;
    State currentState;
    nlohmann::json dialogueData;
    nlohmann::json commandData;

    std::vector<std::shared_ptr<Player>> players;
    static std::default_random_engine engine;
    std::shared_ptr<MiniGameManager> miniGameManager;

    void processPlayerAction(std::shared_ptr<Player> player, std::shared_ptr<Tile> tile);
    bool processCommand(std::shared_ptr<Player> player, const std::string& input);
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
    void setState(const std::string& state);
    std::string getStateString();
};

#endif // GAME_HPP