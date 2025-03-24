#include "Utils.hpp"
#include <chrono>
#include <regex>
#include <string>
#include <thread>
#include <sstream>

#ifdef _WIN32
void setConsoleSize(int width, int height) {
    std::string command = "mode con: cols=" + std::to_string(width) + " lines=" + std::to_string(height);
    system(command.c_str());
}

#else
void setConsoleSize(int width, int height) {
    std::string command = "stty cols " + std::to_string(width) + " rows " + std::to_string(height);
    system(command.c_str());
}
#endif

void delayTime(long long ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    // std::this_thread::sleep_for(std::chrono::seconds(s));
}

std::string stripAnsi(const std::string& text) {
    static const std::regex ansiRegex("\033\\[[0-9;]*m");
    return std::regex_replace(text, ansiRegex, "");
}

std::pair<int, int> getBoardPosition(int posIndex, int mapSize) {
    int tempIndex = 0;
    for (int col = 0; col < mapSize; ++col) {
        if (tempIndex == posIndex)
            return {0, col};
        tempIndex++;
    }
    for (int row = 1; row < mapSize; ++row) {
        if (tempIndex == posIndex)
            return {row, mapSize - 1};
        tempIndex++;
    }
    for (int col = mapSize - 2; col >= 0; --col) {
        if (tempIndex == posIndex)
            return {mapSize - 1, col};
        tempIndex++;
    }
    for (int row = mapSize - 2; row > 0; --row) {
        if (tempIndex == posIndex)
            return {row, 0};
        tempIndex++;
    }
    return {-1, -1}; // 出錯處理，應該不會發生
}

void delayMilliseconds(int ms) {
    auto start = std::chrono::steady_clock::now();
    while (std::chrono::steady_clock::now() - start < std::chrono::milliseconds(ms)) {
        // Busy-waiting loop (not CPU-efficient but avoids thread sleep issues)
    }
}

std::vector<std::string> split(const std::string& str) {
    std::istringstream iss(str);
    std::vector<std::string> tokens;
    std::string token;
    while (iss >> token) {
        tokens.push_back(token);
    }

    return tokens;
}
