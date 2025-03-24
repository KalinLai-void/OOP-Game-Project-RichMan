#ifndef CARD_HPP
#define CARD_HPP

#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include "Game/Board.hpp"
class Player; // Forward declaration to prevent circular dependency

class Card {
private:
    std::string name;
    std::string icon;
    int price;
    std::string effect; // Card effect

public:
    Card(const std::string& n, const std::string& i, int p, const std::string& e)
        : name(n)
        , icon(i)
        , price(p)
        , effect(e) {}

    virtual ~Card() = default;

    std::string getName() const {
        return name;
    }
    std::string getIcon() const {
        return icon;
    }
    int getPrice() const {
        return price;
    }
    std::string getEffect() const {
        return effect;
    }

    virtual void useEffect(std::vector<std::shared_ptr<Player>>& players, std::shared_ptr<Player> curPlayer) = 0;
};

#endif // CARD_HPP
