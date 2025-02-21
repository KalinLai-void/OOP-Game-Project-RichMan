#include "Utils.hpp"
#include <chrono>
#include <string>
#include <thread>

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

void delayTime(long long s) {
    std::this_thread::sleep_for(std::chrono::seconds(s));
}