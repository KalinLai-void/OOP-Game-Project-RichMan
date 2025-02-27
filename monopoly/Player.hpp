﻿#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Card.hpp"
#include <string>
#include <vector>
#include <memory>

class Player : public std::enable_shared_from_this<Player> {
private:
    std::string name;
    std::string icon;
    int position;
    long long money;
    std::vector<std::shared_ptr<Card>> cards; // Cards held by the player
    bool bankrupt;           // Whether the player is bankrupt
    bool inHospital;         // Whether the player is in the hospital
    int hospitalRoundLeft;   // Number of rounds left in the hospital
public:
    Player(const std::string& n, const std::string& i, long long m);

    // Accessors
    std::string getName() const;
    std::string getIcon() const;

    long long getMoney() const;
    int getPosition() const;
    bool isBankrupt() const;
    bool isInHospital() const;

    // Update Player status
    void setPosition(int pos);
    bool adjustMoney(long long delta);
    void setBankrupt(bool b);
    void addCard(std::shared_ptr<Card> card);

    // Hospital related
    void sendToHospital(int rounds);
    void recoverFromHospital();
    void updateHospitalStatus();

    void displayCards(std::vector<std::shared_ptr<Player>>& players, Board& board);
    void useCard(int index, std::vector<std::shared_ptr<Player>>& players, Board& board);
};

#endif // PLAYER_HPP
