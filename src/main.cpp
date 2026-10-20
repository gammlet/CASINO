#include "Casino.h"

#ifdef _WIN32
#include <windows.h>
#endif

int main() {
#ifdef _WIN32
    // Lets the Windows console print card suits (♠ ♥ ♦ ♣) correctly.
    SetConsoleOutputCP(CP_UTF8);
#endif

    Casino casino;
    casino.run();
    return 0;
}
