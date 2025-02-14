#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>

class Player {
private:
    std::string name;
    long long money;
    int position;          // 棋盤上的位置
    bool bankrupt;         // 是否破產
    bool inHospital;       // 是否住院
    int hospitalRoundLeft; // 住院回合數
public:
    Player(const std::string& n, long long m);

    // 存取器
    std::string getName() const;
    long long getMoney() const;
    int getPosition() const;
    bool isBankrupt() const;
    bool isInHospital() const;

    // 更新
    void setPosition(int pos);
    void adjustMoney(long long delta);
    void setBankrupt(bool b);

    // 住院相關
    void sendToHospital(int rounds);
    void recoverFromHospital();
    void updateHospitalStatus();
};

#endif // PLAYER_HPP
