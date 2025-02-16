#ifndef INPUTMANAGER_HPP
#define INPUTMANAGER_HPP

#ifdef _WIN32
#    include <conio.h>
#else
#    include <termios.h>
#    include <unistd.h>
#endif

class InputManager {
public:
    // 取得一個按鍵，不需要按 Enter 鍵
    static char getKey() {
#ifdef _WIN32
        return _getch();
#else
        struct termios oldt, newt;
        char ch;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        ch = getchar();
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        return ch;
#endif
    }
};

#endif // INPUTMANAGER_HPP
