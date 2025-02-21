#include "StoreTile.hpp"
#include "Bank.hpp"
#include "CardStore.hpp"
#include <iostream>

using namespace std;

StoreTile::StoreTile(const std::string& n)
    : Tile(n) {}

CardStore& StoreTile::getStore() {
    return store;
}

void StoreTile::enterStore(std::shared_ptr<Player> player) {
    // CardStore store;
    store.displayStore(player);
}

TileAction StoreTile::landOn(std::shared_ptr<Player> player) {
    return TileAction::STORE;
}
