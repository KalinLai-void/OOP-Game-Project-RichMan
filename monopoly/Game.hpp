#ifndef GAME_HPP
#    define GAME_HPP

#    include "Board.hpp"
#    include "GameConfig.hpp"
#    include "Player.hpp"
#    include <memory>
#    include <random>
#    include <vector>

class Game {
private:
    Board board;
    GameConfig config;
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
};

#endif // GAME_HPP

// // Game.hpp (部分內容)
// #ifndef GAME_HPP
// #define GAME_HPP

// #include "Board.hpp"
// #include "GameConfig.hpp"
// #include "Player.hpp"
// #include <memory>
// #include <vector>

// class Game {
// public:
//     Game(const GameConfig& cfg);
//     void initGame();
//     void start();
//     void checkGameOver();
//     void endGame();

// private:
//     void processPlayerAction(std::shared_ptr<Player> player, std::shared_ptr<Tile> tile);

//     Board board;
//     GameConfig config;
//     bool gameOver;
//     std::vector<std::shared_ptr<Player>> players;

//     // 宣告隨機引擎
//     static std::default_random_engine engine;
// };

// #endif // GAME_HPP
