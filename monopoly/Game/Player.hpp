#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <memory>
#include <string>
#include <vector>

class Card;
class MiniGameManager;

class Player : public std::enable_shared_from_this<Player> {
private:
    std::string name;
    std::string icon;
    std::string color;
    int position;
    long long money;
    std::vector<std::shared_ptr<Card>> cards; // Cards held by the player
    bool bankrupt;                            // Whether the player is bankrupt
    bool inHospital;                          // Whether the player is in the hospital
    bool isMyTurn;                            // Whether it is the player's turn
    int hospitalRoundLeft;                    // Number of rounds left in the hospital
    int diceControl;

public:
    Player(const std::string& n, const std::string& i, const std::string& c, long long m); // name, icon, color, init money

    // Accessors
    std::string getName() const;
    std::string getIcon() const;
    std::string getColor() const;
    std::string getIconWithColor() const;
    std::string getDisplayName() const;

    long long getMoney() const;
    int getPosition() const;

    /**
    * Get the specified number for rolling dice
    */
    int getDiceControl() const;
    bool isBankrupt() const;
    bool isInHospital() const;
    bool checkIsMyTurn() const;

    // Update Player status
    void setPosition(int pos);
    void setMyTurn(bool b);
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

    /**
    * Get all cards currently held by the player
    */
    std::vector<std::shared_ptr<Card>> getCards();

    /**
    * Display cards owned by player and allowing them to choose to use
    * 
    * @param players Reference to all player (used if the card affects others).
    */
    void displayCards(std::vector<std::shared_ptr<Player>>& players);
    
    /**
    * Use the specified card
    * 
    * @param index For obtain specified card
    * @param players For efffecting may need choose player
    */
    void useCard(int index, std::vector<std::shared_ptr<Player>>& players);
    int rollDice();
};

#endif // PLAYER_HPP
