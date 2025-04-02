#ifndef CARD_HPP
#define CARD_HPP

#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include "Game/Board.hpp"
class Player; // Forward declaration to prevent circular dependency


/*
* You can inherit from this class and override `useEffect` to implement custom card behaviro.
*/
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

    /*
    * Pure virtual function: must be implemented in derived card classes.
    * 
    * @param players List of all players in the game (can be used for target selection)
    * @param curPlayer The player who is using the card
    */
    virtual void useEffect(std::vector<std::shared_ptr<Player>>& players, std::shared_ptr<Player> curPlayer) = 0;
};

#endif // CARD_HPP
