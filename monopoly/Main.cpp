#include "Game.hpp"
#include "GameConfig.hpp"
#include <iostream>
using namespace std;

int main() {
    GameMode mode = GameMode::DEBUG;
    GameConfig::getInstance().setMode(mode);
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    Game game;
    game.initGame(GameConfig::getInstance());
    game.start();

    return 0;
}
