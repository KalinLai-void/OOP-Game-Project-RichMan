#ifndef INPUTMANAGER_HPP
#define INPUTMANAGER_HPP
#include <cctype>
#ifdef _WIN32
#    include <conio.h>
#else
#    include <termios.h>
#    include <unistd.h>
#endif

class InputManager {
public:
    // Get a key press without needing to press Enter, and automatically convert to uppercase
    static char getKey() {
#ifdef _WIN32
        char ch = _getch();
#else
        struct termios oldt, newt;
        char ch;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        ch = getchar();
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
#endif
        return std::toupper(ch); // Convert to uppercase
    }
};
#endif // INPUTMANAGER_HPP
