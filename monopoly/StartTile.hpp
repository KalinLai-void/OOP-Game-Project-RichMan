#ifndef STARTTILE_HPP
#define STARTTILE_HPP

#include "Tile.hpp"
#include <iostream>

class StartTile : public Tile {
private:
    int bonus; // 玩家經過或停留時獲得的獎勵金額

public:
    // `StartTile` 構造函數，預設獎勵 $2000
    StartTile(const std::string& n, int b = 2000);

    // 當玩家停在起點時的行為
    void landOn(std::shared_ptr<Player> player) override;
};

#endif // STARTTILE_HPP
