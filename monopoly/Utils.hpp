#ifndef UTILS_HPP
#define UTILS_HPP

#ifdef _WIN32 // 只在 Windows 平台編譯
#    include <windows.h>
#endif

// 設定 Console 視窗大小（Windows 平台）
void setConsoleSize(int width, int height);
void delayTime(long long s);
#endif // UTILS_HPP
