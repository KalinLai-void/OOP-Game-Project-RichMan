#include "utils.hpp"
#include <iostream>
#include <string>

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

// #ifdef _WIN32
// void setConsoleSize(int width, int height) {
//     HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

//     // 先取得當前控制台緩衝區大小
//     CONSOLE_SCREEN_BUFFER_INFO csbi;
//     GetConsoleScreenBufferInfo(hConsole, &csbi);

//     // 先縮小視窗大小，避免緩衝區縮小失敗
//     SMALL_RECT minimalRect = {0, 0, 1, 1}; // 設定一個最小的視窗大小
//     SetConsoleWindowInfo(hConsole, TRUE, &minimalRect);

//     // 設定新的緩衝區大小（確保它大於等於視窗大小）
//     COORD bufferSize = {static_cast<SHORT>(width), static_cast<SHORT>(height)};
//     SetConsoleScreenBufferSize(hConsole, bufferSize);

//     // 設定新的視窗大小
//     SMALL_RECT windowSize = {0, 0, static_cast<SHORT>(width - 1), static_cast<SHORT>(height - 1)};
//     SetConsoleWindowInfo(hConsole, TRUE, &windowSize);
// }
// #endif
