﻿#ifndef PLAYER_HPP
#define PLAYER_HPP

//#include "Card.hpp"
#include "PlayerIcon.hpp"
#include <memory>
#include <string>
#include <vector>

class Card;
class MiniGameManager;

class Player : public std::enable_shared_from_this<Player> {
private:
    std::string name;
    PlayerIcon icon;
    int position;
    long long money;
    std::vector<std::shared_ptr<Card>> cards; // Cards held by the player
    bool bankrupt;                            // Whether the player is bankrupt
    bool inHospital;                          // Whether the player is in the hospital
    int hospitalRoundLeft;                    // Number of rounds left in the hospital
    int diceControl;

public:
    Player(const std::string& n, const PlayerIcon& i, long long m);

    // Accessors
    std::string getName() const;
    std::string getIcon() const;
    std::string getIconWithColor() const;

    long long getMoney() const;
    int getPosition() const;
    int getDiceControl() const;
    bool isBankrupt() const;
    bool isInHospital() const;

    // Update Player status
    void setPosition(int pos);
    bool addMoney(long long amount);
    bool deductMoney(long long amount);
    void setBankrupt(bool b);
    void addCard(std::shared_ptr<Card> card);
    void setDiceControl(int step);
    void sendToStart();

    // Hospital related
    void sendToHospital(int rounds);
    void recoverFromHospital();
    void updateHospitalStatus();

    // MiniGame related
    void startMiniGame();
    void endMiniGame();

    std::vector<std::shared_ptr<Card>> getCards();
    void displayCards(std::vector<std::shared_ptr<Player>>& players);
    void useCard(int index, std::vector<std::shared_ptr<Player>>& players);
    int rollDice();

};

#endif // PLAYER_HPP
