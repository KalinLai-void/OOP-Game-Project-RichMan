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

    /**
    * Handles player actions during their turn.
    * Includes tile effects, UI options, and commands.
    */ 
    void processPlayerAction(std::shared_ptr<Player> player, std::shared_ptr<Tile> tile, bool isCommandResult = false);

    /**
    * Parses and executes commands (e.g. /give, /move, etc.)
    * Returns true if command was valid
    */
    bool processCommand(std::shared_ptr<Player> player, const std::string& input);
    
    void throwDice(std::shared_ptr<Player> player);
    void movePlayer(std::shared_ptr<Player> player, int steps);
    
    /**
    * Retrieves the dialogue and options based on current game state.
    * 
    * @return A JSON object containing prompt text and available input options.
    */
    const nlohmann::json& playerAction();

    /**
    * Returns specific dialogue text .
    * 
    * @param key The key can assece dialogue (e.g., hospital, event, etc.).
    * @return A JSON object containing prompt text and available input options.
    */
    const nlohmann::json& playerAction(const std::string& key);
    Game(const GameConfig& config);
    static std::shared_ptr<Game> instance;

public:
    /**
    * Get the singleton Game tinstance
    * Creates one if not initialized
    */
    static std::shared_ptr<Game> getInstance(const GameConfig& config);

    /**
    * Initializes player data and board from config
    */
    void initGame();

    /**
    * Start game loop and handles player turns
    */
    void start();

    /**
    * Checks for win/loss conditions
    */
    void checkGameOver();

    /**
    * Called when game is over
    */
    void endGame();

    /**
    * Change current game state 
    */
    void changeState(State newState);
    void setState(const std::string& state);

    /**
    * Get current state as string
    */
    std::string getStateString();

    bool isActivateState() const;
    bool isRoundState() const;
};

#endif // GAME_HPP