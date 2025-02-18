#include "Game.hpp"
#include "GameConfig.hpp"
#include <nlohmann/json.hpp>
#include <iostream>
using namespace std;
int main() {
    GameMode mode = GameMode::DUEL;
    GameConfig::getInstance().setMode(mode);
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    Game game(GameConfig::getInstance());
    game.initGame();
    game.start();

    return 0;
}
