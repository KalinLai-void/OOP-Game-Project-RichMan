#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <random>
#include <ctime>
#include <map>
#include <algorithm>
#include <iomanip>
using namespace std;

/* =======================================
   1. 地圖 (世界地標) - 類似結構體/紀錄用
   ======================================= */
struct WorldTile {
    string city;    // 城市名稱
    string country; // 國家名稱
    bool hasHouse;  // 是否有房子 (此處僅示範，可擴充為等級等)
};

// 建立一個地標索引 -> WorldTile 的對照表
map<int, WorldTile> createWorldMap() {
    map<int, WorldTile> tiles;
    // 你可以自行定義更多或更少的地標
    tiles[0] = { "起點", "START", false };
    tiles[1] = { "東京", "日本", false };
    tiles[2] = { "悉尼", "澳洲", false };
    tiles[3] = { "巴黎", "法國", true };
    tiles[4] = { "紐約", "美國", false };
    tiles[5] = { "里約", "巴西", false };
    tiles[6] = { "首爾", "南韓", false };
    tiles[7] = { "倫敦", "英國", true };
    tiles[8] = { "柏林", "德國", false };
    tiles[9] = { "羅馬", "義大利", false };
    tiles[10] = { "阿姆斯特丹", "荷蘭", false };
    tiles[11] = { "洛杉磯", "美國", true };
    tiles[12] = { "香港", "中國", true };
    tiles[13] = { "曼谷", "泰國", true };
    tiles[14] = { "伊斯坦堡", "土耳其", false };
    tiles[15] = { "莫斯科", "俄羅斯", false };
    tiles[16] = { "北京", "中國", true };
    tiles[17] = { "新加坡", "新加坡", true };
    tiles[18] = { "墨西哥城", "墨西哥", false };
    tiles[19] = { "馬德里", "西班牙", true };
    tiles[20] = { "開羅", "埃及", false };
    tiles[21] = { "芝加哥", "美國", false };
    // 可以再繼續擴充，若超過 32 格或更多，可自行增加
    return tiles;
}

// 供後面測試使用
static const int WORLD_MAP_SIZE = 8;  // 繪製 8x8 外框

/* =======================================
   2. 先示範繪製 8x8 棋盤 (drawWorldBoard)
   ======================================= */
   /* =======================================
      4. 玩家 Player 類別
      ======================================= */
class Player {
private:
    string name;
    long long money;
    int position;
    bool bankrupt;
    bool inHospital;
    int hospitalRoundLeft;

public:
    Player(const string& n, long long m)
        : name(n), money(m), position(0),
        bankrupt(false), inHospital(false), hospitalRoundLeft(0) {}

    string getName() const { return name; }
    long long getMoney() const { return money; }
    int getPosition() const { return position; }
    bool isBankrupt() const { return bankrupt; }
    bool isInHospital() const { return inHospital; }

    void setPosition(int pos) { position = pos; }
    void adjustMoney(long long delta) { money += delta; }
    void setBankrupt(bool b) { bankrupt = b; }

    void sendToHospital(int rounds) {
        inHospital = true;
        hospitalRoundLeft = rounds;
    }
    void recoverFromHospital() {
        inHospital = false;
        hospitalRoundLeft = 0;
    }
    void updateHospitalStatus() {
        if (inHospital) {
            hospitalRoundLeft--;
            if (hospitalRoundLeft <= 0) {
                recoverFromHospital();
            }
        }
    }
};


void drawWorldBoard(const vector<shared_ptr<Player>>& players) {
    // 建立 8x8 的字串陣列，先全部填空白
    vector<vector<string>> board(WORLD_MAP_SIZE, vector<string>(WORLD_MAP_SIZE, "   "));

    // 取得我們定義好的地標資料
    map<int, WorldTile> tileMap = createWorldMap();

    // 簡單做一個「外框環狀」填充：
    // 上排 (0 -> 7)
    // 右排 (8 -> 15)
    // 下排 (16 -> 23)
    // 左排 (24 -> 31)
    // 以此類推，可依照實際地圖格數調整
    // 這裡僅示範 8x8 外框放 32 格 (0~31)
    // 若實際你遊戲是 22 格或更多，就酌情修改判斷與對應。

    int posIndex = 0; // 0 ~ 31 (假設我們只示範 32 格)
    for (int col = 0; col < WORLD_MAP_SIZE; ++col) {
        // 上排
        if (tileMap.count(posIndex)) {
            board[0][col] = tileMap[posIndex].city;
        }
        else {
            board[0][col] = "P" + to_string(posIndex);
        }
        posIndex++;
    }
    for (int row = 1; row < WORLD_MAP_SIZE; ++row) {
        // 右排
        if (tileMap.count(posIndex)) {
            board[row][WORLD_MAP_SIZE - 1] = tileMap[posIndex].city;
        }
        else {
            board[row][WORLD_MAP_SIZE - 1] = "P" + to_string(posIndex);
        }
        posIndex++;
    }
    for (int col = WORLD_MAP_SIZE - 2; col >= 0; --col) {
        // 下排
        if (tileMap.count(posIndex)) {
            board[WORLD_MAP_SIZE - 1][col] = tileMap[posIndex].city;
        }
        else {
            board[WORLD_MAP_SIZE - 1][col] = "P" + to_string(posIndex);
        }
        posIndex++;
    }
    for (int row = WORLD_MAP_SIZE - 2; row > 0; --row) {
        // 左排
        if (tileMap.count(posIndex)) {
            board[row][0] = tileMap[posIndex].city;
        }
        else {
            board[row][0] = "P" + to_string(posIndex);
        }
        posIndex++;
    }

    // 接著將「玩家位置」顯示在棋盤上
    // 若玩家所在的索引超過 31，則取 mod 32 (或依實際格數)
    for (const shared_ptr<Player>& p : players) {
        int pPos = p->getPosition() % 32; // 假設我們只有 32 格外框
        // 計算此索引在外框上的 (row, col) 座標
        // 與上方填充邏輯一致
        // 可以直接複製上面四段迴圈，找出 pPos 所在的 row, col
        // 以下為示範：簡單把同樣的流程包裝起來
        int tempIndex = 0;
        int rowOut = 0, colOut = 0; // 用於紀錄玩家對應的位置
        bool found = false;

        // 上排
        for (int col = 0; col < WORLD_MAP_SIZE && !found; ++col) {
            if (tempIndex == pPos) {
                rowOut = 0; colOut = col;
                found = true;
                break;
            }
            tempIndex++;
        }
        // 右排
        for (int row = 1; row < WORLD_MAP_SIZE && !found; ++row) {
            if (tempIndex == pPos) {
                rowOut = row; colOut = WORLD_MAP_SIZE - 1;
                found = true;
                break;
            }
            tempIndex++;
        }
        // 下排
        for (int col = WORLD_MAP_SIZE - 2; col >= 0 && !found; --col) {
            if (tempIndex == pPos) {
                rowOut = WORLD_MAP_SIZE - 1; colOut = col;
                found = true;
                break;
            }
            tempIndex++;
        }
        // 左排
        for (int row = WORLD_MAP_SIZE - 2; row > 0 && !found; --row) {
            if (tempIndex == pPos) {
                rowOut = row; colOut = 0;
                found = true;
                break;
            }
            tempIndex++;
        }

        // 將玩家符號疊加顯示
        // 如果該格原本是 "東京" 或 "Pxx"，我們將它改成 "東京(A)" 之類
        board[rowOut][colOut] += "(" + p->getName() + ")";
    }

    // 最後輸出 8x8 棋盤
    cout << "\n======== 世界地圖顯示(示範) ========\n\n";
    cout << "+--------+--------+--------+--------+--------+--------+--------+--------+\n";
    for (int i = 0; i < WORLD_MAP_SIZE; i++) {
        cout << "| ";
        for (int j = 0; j < WORLD_MAP_SIZE; j++) {
            cout << left << setw(6) << board[i][j] << " | ";
        }
        cout << "\n+--------+--------+--------+--------+--------+--------+--------+--------+\n";
    }
    cout << endl;
}

/* =======================================
   3. Singleton - 銀行 Bank (供範例)
   ======================================= */
class Bank {
private:
    long long totalMoneyInBank;
    static Bank* instance;

    Bank() : totalMoneyInBank(1000000000LL) {}
public:
    static Bank* getInstance() {
        if (!instance) instance = new Bank();
        return instance;
    }
    void payToPlayer(long long amount) {
        totalMoneyInBank -= amount;
    }
    void receiveFromPlayer(long long amount) {
        totalMoneyInBank += amount;
    }
    long long getTotalMoneyInBank() const {
        return totalMoneyInBank;
    }
};
Bank* Bank::instance = nullptr;


/* =======================================
   5. 棋盤上的 Tile 基底類別
   ======================================= */
class Tile {
protected:
    string name;
public:
    Tile(const string& n) : name(n) {}
    virtual ~Tile() {}
    string getName() const { return name; }
    virtual void landOn(shared_ptr<Player> player) = 0;
};

/* =======================================
   5.1 PropertyTile (地產格)
   ======================================= */
enum class PropertyLevel {
    LEVEL1 = 1,
    LEVEL2,
    LEVEL3
};

class PropertyTile : public Tile {
private:
    long long basePrice;
    long long baseToll;
    shared_ptr<Player> owner;
    PropertyLevel level;

public:
    PropertyTile(const string& n, long long price, long long toll)
        : Tile(n), basePrice(price), baseToll(toll), owner(nullptr), level(PropertyLevel::LEVEL1) {}

    long long getToll() const {
        switch (level) {
        case PropertyLevel::LEVEL1: return baseToll;
        case PropertyLevel::LEVEL2: return baseToll * 2;
        case PropertyLevel::LEVEL3: return baseToll * 3;
        }
        return baseToll;
    }

    long long getUpgradeCost() const {
        return static_cast<long long>(basePrice * 0.5);
    }

    void landOn(shared_ptr<Player> player) override {
        if (!owner) {
            // 無人擁有，詢問是否購買
            cout << "玩家 " << player->getName()
                << " 抵達 [地產] " << name
                << "，價格：" << basePrice << "，是否購買？(y/n) ";
            char c;
            cin >> c;
            if (c == 'y' || c == 'Y') {
                if (player->getMoney() >= basePrice) {
                    player->adjustMoney(-basePrice);
                    owner = player;
                    cout << "已購買 " << name << "。\n";
                }
                else {
                    cout << "金錢不足，無法購買。\n";
                }
            }
        }
        else {
            if (owner == player) {
                // 自己的地，詢問是否升級
                cout << "玩家 " << player->getName()
                    << " 抵達自己的地產 [" << name << "]。\n";
                if (level != PropertyLevel::LEVEL3) {
                    cout << "是否升級？升級費用：" << getUpgradeCost() << " (y/n) ";
                    char c;
                    cin >> c;
                    if (c == 'y' || c == 'Y') {
                        if (player->getMoney() >= getUpgradeCost()) {
                            player->adjustMoney(-getUpgradeCost());
                            if (level == PropertyLevel::LEVEL1) {
                                level = PropertyLevel::LEVEL2;
                            }
                            else if (level == PropertyLevel::LEVEL2) {
                                level = PropertyLevel::LEVEL3;
                            }
                            cout << "升級成功，當前等級：" << (int)level << "\n";
                        }
                        else {
                            cout << "金錢不足，無法升級。\n";
                        }
                    }
                }
                else {
                    cout << "已達最高等級，不可再升級。\n";
                }
            }
            else {
                // 別人的地，付過路費
                long long fee = getToll();
                cout << "玩家 " << player->getName()
                    << " 抵達 [" << name << "]，擁有者：" << owner->getName()
                    << "，需支付過路費：" << fee << "\n";
                if (player->getMoney() >= fee) {
                    player->adjustMoney(-fee);
                    owner->adjustMoney(fee);
                }
                else {
                    cout << "金錢不足，" << player->getName() << " 破產！\n";
                    player->setBankrupt(true);
                }
            }
        }
    }
};

/* =======================================
   5.2 EventTile (事件格)
   ======================================= */
class EventTile : public Tile {
public:
    EventTile(const string& n) : Tile(n) {}
    void landOn(shared_ptr<Player> player) override {
        cout << "玩家 " << player->getName()
            << " 停在事件格 [" << name << "]，觸發隨機事件！\n";
        static default_random_engine engine(static_cast<unsigned>(time(nullptr)));
        uniform_int_distribution<int> dist(1, 3);
        int eventType = dist(engine);
        switch (eventType) {
        case 1:
            cout << "幸運事件：獲得銀行獎勵 5000！\n";
            player->adjustMoney(5000);
            Bank::getInstance()->payToPlayer(5000);
            break;
        case 2:
            cout << "倒楣事件：被罰款 3000！\n";
            if (player->getMoney() >= 3000) {
                player->adjustMoney(-3000);
                Bank::getInstance()->receiveFromPlayer(3000);
            }
            else {
                player->setBankrupt(true);
                cout << "金錢不足，破產！\n";
            }
            break;
        case 3:
            cout << "意外受傷，送往醫院，需住院 2 回合！\n";
            player->sendToHospital(2);
            break;
        }
    }
};

/* =======================================
   5.3 HospitalTile (醫院格)
   ======================================= */
class HospitalTile : public Tile {
public:
    HospitalTile(const string& n) : Tile(n) {}
    void landOn(shared_ptr<Player> player) override {
        cout << "玩家 " << player->getName()
            << " 抵達 [" << name << "] (醫院)，若受傷狀態則需停留。\n";
        // 可擴充：支付醫藥費提早出院等
    }
};

/* =======================================
   5.4 StoreTile (商店格)
   ======================================= */
class StoreTile : public Tile {
public:
    StoreTile(const string& n) : Tile(n) {}
    void landOn(shared_ptr<Player> player) override {
        cout << "玩家 " << player->getName()
            << " 來到 [" << name << "] (道具商店)。\n"
            << "可購買 [控骰卡] (2000)。是否購買？(y/n) ";
        char c;
        cin >> c;
        if (c == 'y' || c == 'Y') {
            if (player->getMoney() >= 2000) {
                player->adjustMoney(-2000);
                Bank::getInstance()->receiveFromPlayer(2000);
                cout << "購買成功。(尚未實作道具管理)\n";
            }
            else {
                cout << "金錢不足，無法購買。\n";
            }
        }
    }
};

/* =======================================
   6. Board 類別 - 管理所有格子 (1D 線性)
   ======================================= */
   /* =======================================
      6. Board 類別 - 管理所有格子 (1D 線性)
      使之產生 32 格，以配合 drawWorldBoard()
      ======================================= */
class Board {
private:
    vector<shared_ptr<Tile>> tiles;

public:
    Board() {
        // 從 createWorldMap() 拿到 (index -> WorldTile) 的地圖資料
        auto worldMap = createWorldMap();

        // 建立 32 個格子 (index: 0 ~ 31)
        // 與 drawWorldBoard() 所使用的 32 個外框座標相對應
        for (int i = 0; i < 32; i++) {
            // 根據 worldMap 是否包含此 i，決定 cityName
            string cityName;
            if (worldMap.count(i) > 0) {
                cityName = worldMap[i].city; // 例如 "東京"
            }
            else {
                cityName = "P" + to_string(i); // 預設名稱
            }

            // 根據名稱/索引選擇生成哪種 Tile
            // (如果你想更精細，可再擴充 if-else 或增加參數判斷)
            if (cityName == "醫院") {
                tiles.push_back(make_shared<HospitalTile>(cityName));
            }
            else if (cityName == "命運" || cityName == "機會") {
                tiles.push_back(make_shared<EventTile>(cityName));
            }
            else if (cityName == "商店") {
                tiles.push_back(make_shared<StoreTile>(cityName));
            }
            else {
                // 其他一律當作「地產格」
                // 給定一些價格/過路費 (此處僅示範，可依需求做計算/隨機生成)
                long long price = 2000 + (i * 100);
                long long toll = 200 + (i * 10);
                tiles.push_back(make_shared<PropertyTile>(cityName, price, toll));
            }
        }
    }

    int getSize() const {
        return static_cast<int>(tiles.size());
    }

    shared_ptr<Tile> getTile(int index) {
        if (index < 0 || index >= (int)tiles.size()) {
            return nullptr;
        }
        return tiles[index];
    }
};


/* =======================================
   7. Game 類別 - 管理整體流程
   ======================================= */
class Game {
private:
    Board board;
    vector<shared_ptr<Player>> players;
    static default_random_engine engine;
    bool gameOver;

public:
    Game() : gameOver(false) {
        engine.seed(static_cast<unsigned>(time(nullptr)));
    }

    void initGame() {
        int playerCount;
        cout << "請輸入玩家人數 (2~4)：";
        cin >> playerCount;
        if (playerCount < 2) playerCount = 2;
        if (playerCount > 4) playerCount = 4;

        for (int i = 0; i < playerCount; i++) {
            cout << "請輸入第 " << (i + 1) << " 位玩家名稱：";
            string name;
            cin >> name;
            auto p = make_shared<Player>(name, 10000);
            players.push_back(p);
        }
    }

    void start() {
        cout << "遊戲開始！\n";
        while (!gameOver) {
            // (可視需求) 在每一回合開頭或結尾印出地圖
            // 在這裡示範在回合開始前印出
            drawWorldBoard(players); // <--- 這裡呼叫我們的「世界地圖顯示」

            for (auto& p : players) {
                if (p->isBankrupt()) continue;
                if (p->isInHospital()) {
                    cout << "玩家 " << p->getName() << " 在醫院休養中。\n";
                    p->updateHospitalStatus();
                    continue;
                }

                // 擲骰
                uniform_int_distribution<int> dist(1, 6);
                int d1 = dist(engine);
                int d2 = dist(engine);
                int steps = d1 + d2;
                cout << "\n-----------\n";
                cout << "玩家 " << p->getName() << " 擲骰: ("
                    << d1 << "," << d2 << "), 前進 " << steps << " 步\n";

                // 線性棋盤移動 (以 board.getSize() 做環狀)
                int newPos = (p->getPosition() + steps) % board.getSize();
                p->setPosition(newPos);

                // 停留
                auto tile = board.getTile(newPos);
                if (tile) {
                    tile->landOn(p);
                    if (p->isBankrupt()) {
                        cout << "玩家 " << p->getName() << " 已破產。\n";
                    }
                }

                // 雙骰相同 → 再行動一次 (簡化邏輯)
                if (d1 == d2 && !p->isBankrupt() && !p->isInHospital()) {
                    cout << "雙數！再次行動！\n";
                    int d3 = dist(engine);
                    int d4 = dist(engine);
                    int steps2 = d3 + d4;
                    cout << "再擲一次: (" << d3 << "," << d4 << "), 前進 " << steps2 << " 步\n";
                    int newPos2 = (p->getPosition() + steps2) % board.getSize();
                    p->setPosition(newPos2);

                    auto tile2 = board.getTile(newPos2);
                    if (tile2) {
                        tile2->landOn(p);
                        if (p->isBankrupt()) {
                            cout << "玩家 " << p->getName() << " 已破產。\n";
                        }
                    }
                }

                checkGameOver();
                if (gameOver) break;
            }
        }

        endGame();
    }

    void checkGameOver() {
        // 簡單結束條件：只剩一人未破產 或 有人資金達到目標
        int aliveCount = 0;
        for (auto& p : players) {
            if (!p->isBankrupt()) aliveCount++;
        }
        if (aliveCount <= 1) {
            gameOver = true;
            return;
        }
        for (auto& p : players) {
            if (p->getMoney() >= 100000) {
                gameOver = true;
                break;
            }
        }
    }

    void endGame() {
        cout << "\n===== 遊戲結束 =====\n";
        sort(players.begin(), players.end(), [](auto& a, auto& b) {
            if (a->isBankrupt() && !b->isBankrupt()) return false;
            if (!a->isBankrupt() && b->isBankrupt()) return true;
            return a->getMoney() > b->getMoney();
            });
        for (size_t i = 0; i < players.size(); i++) {
            cout << (i + 1) << ". " << players[i]->getName()
                << " - 資金: " << players[i]->getMoney()
                << (players[i]->isBankrupt() ? " (破產)" : "")
                << endl;
        }
    }
};
default_random_engine Game::engine;

/* =======================================
   main - 程式進入點
   ======================================= */
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    Game game;
    game.initGame();
    game.start();

    return 0;
}
