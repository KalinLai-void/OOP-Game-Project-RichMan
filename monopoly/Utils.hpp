#ifndef UTILS_HPP
#define UTILS_HPP
#include <nlohmann/json.hpp>
#include <iostream>
#ifdef _WIN32 // Only compile on Windows platform
#    include <windows.h>
#endif

// Set the console window size (Windows platform)
void setConsoleSize(int width, int height);
void delayTime(long long ms);
std::string stripAnsi(const std::string& text);
std::pair<int, int> getBoardPosition(int posIndex, int mapSize);

#endif // UTILS_HPP
