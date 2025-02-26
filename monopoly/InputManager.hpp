// #ifndef INPUTMANAGER_HPP
// #define INPUTMANAGER_HPP
// #include <cctype>
// #ifdef _WIN32
// #    include <conio.h>
// #else
// #    include <termios.h>
// #    include <unistd.h>
// #endif

// class InputManager {
// public:
//     // Get a key press without needing to press Enter, and automatically convert to uppercase
//     static char getKey() {
// #ifdef _WIN32
//         char ch = _getch();
// #else
//         struct termios oldt, newt;
//         char ch;
//         tcgetattr(STDIN_FILENO, &oldt);
//         newt = oldt;
//         newt.c_lflag &= ~(ICANON | ECHO);
//         tcsetattr(STDIN_FILENO, TCSANOW, &newt);
//         ch = getchar();
//         tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
// #endif
//         return std::toupper(ch); // Convert to uppercase
//     }
// };
// #endif // INPUTMANAGER_HPP

#ifndef INPUTMANAGER_HPP
#    define INPUTMANAGER_HPP
#    include <cctype>
#    include <iostream>

#    ifdef _WIN32
#        include <conio.h> // Windows _getch()
#    else
#        include <termios.h>
#        include <unistd.h>
#    endif

class InputManager {
public:
    // Get a key press without needing to press Enter, and automatically convert to uppercase
    static char getKey() {
        int ch; // 改用 int
        while (true) {
#    ifdef _WIN32
            ch = _getch();
            if (ch == 0 || ch == 0xE0) {
                (void)_getch();
                continue;
            }
#    else
            struct termios oldt, newt;
            tcgetattr(STDIN_FILENO, &oldt);
            newt = oldt;
            newt.c_lflag &= ~(ICANON | ECHO);
            tcsetattr(STDIN_FILENO, TCSANOW, &newt);
            ch = getchar();
            tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

            if (ch == 27) {
                getchar();
                getchar();
                continue;
            }
#    endif
            if (std::isalnum(ch) || std::ispunct(ch) || ch == ' ') {
                return std::isalpha(ch) ? std::toupper(ch) : ch;
            }
        }
    }
};

#endif // INPUTMANAGER_HPP

/*int main() {
    std::cout << "請按一個鍵: ";
    char key = InputHandler::getKey();
    std::cout << "\n你按下的鍵是: " << key << std::endl;
    return 0;
}*/