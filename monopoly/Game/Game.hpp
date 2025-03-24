#ifndef GAME_HPP
#define GAME_HPP

#include "Board.hpp"
#include "Tiles/Tile.hpp"
#include "GameConfig.hpp"
#include "Player.hpp"
#include "Utils/Utils.hpp"
#include <memory>
#include <random>
#include <vector>

enum class State { INIT, START, MOVED, ROUND_END, FINISH };
State& operator++(State& state);

class Game {
private:
    Board* board;
    GameConfig config;
    State currentState;
    std::string diceResult;
    nlohmann::json dialogueData;
    nlohmann::json commandData;
    bool gameForceControl;

    std::vector<std::shared_ptr<Player>> players;
    static std::default_random_engine engine;

    // isCommandResult = false (default). let it continue to retry input when isCommandResult = true
    void processPlayerAction(std::shared_ptr<Player> player, std::shared_ptr<Tile> tile, bool isCommandResult = false);
    bool processCommand(std::shared_ptr<Player> player, const std::string& input);
    void throwDice(std::shared_ptr<Player> player);
    void movePlayer(std::shared_ptr<Player> player, int steps);
    const nlohmann::json& playerAction();
    const nlohmann::json& playerAction(const std::string& key);
    Game(const GameConfig& config);
    static std::shared_ptr<Game> instance;

public:
    static std::shared_ptr<Game> getInstance(const GameConfig& config);

    void initGame();
    void start();
    void checkGameOver();
    void endGame();
    void changeState(State newState);
    void setState(const std::string& state);
    std::string getStateString();
    bool isActivateState() const;
    bool isRoundState() const;
};

#endif // GAME_HPP