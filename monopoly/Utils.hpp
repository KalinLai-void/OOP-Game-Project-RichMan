﻿#ifndef UTILS_HPP
#define UTILS_HPP
#include <nlohmann/json.hpp>
#include <iostream>
#ifdef _WIN32 // Only compile on Windows platform
#    include <windows.h>
#endif

// Set the console window size (Windows platform)
void setConsoleSize(int width, int height);
void delayTime(long long s);
std::string stripAnsi(const std::string& text);

#endif // UTILS_HPP
