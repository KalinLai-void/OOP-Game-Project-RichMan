#include "StoreTile.hpp"
#include "Bank.hpp"
#include "CardStore.hpp"
#include <iostream>

using namespace std;

StoreTile::StoreTile(const size_t id, const std::string& n, const GameConfig& config)
    : Tile(id, n), store(CardStore::getInstance(config)) {}

CardStore* StoreTile::getStore() {
    return store;
}

void StoreTile::enterStore(std::shared_ptr<Player> player) {
    if (store) {
        store->displayStore(player);
    } else {
        std::cerr << "Error: store is null!" << std::endl;
    }
}

TileAction StoreTile::landOn(std::shared_ptr<Player> player) {
    return TileAction::STORE;
}
