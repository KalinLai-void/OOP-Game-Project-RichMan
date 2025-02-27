#include "MiniGameManager.hpp"
#include "Player.hpp"
#include <gtest/gtest.h>
#include <memory>

class MiniGameManagerTest : public ::testing::Test {
protected:
    std::shared_ptr<MiniGameManager> miniGameManager;
    std::shared_ptr<Player> player;

    void SetUp() override {
        miniGameManager = std::make_shared<MiniGameManager>();
        player = std::make_shared<Player>("TestPlayer", "T", 1000);
    }
};

TEST_F(MiniGameManagerTest, StartMiniGame) {
    testing::internal::CaptureStdout();
    miniGameManager->startMiniGame(player);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "Starting mini-game for player: TestPlayer\n");
}

TEST_F(MiniGameManagerTest, EndMiniGame) {
    testing::internal::CaptureStdout();
    miniGameManager->endMiniGame(player);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "Ending mini-game for player: TestPlayer\n");
}

TEST_F(MiniGameManagerTest, ListMiniGames) {
    testing::internal::CaptureStdout();
    miniGameManager->listMiniGames();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "Available mini-games: ...\n");
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
