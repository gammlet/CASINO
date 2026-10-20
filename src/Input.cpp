#include "Input.h"

#include <cctype>
#include <cstdlib>
#include <iostream>
#include <sstream>

namespace input {

std::string readLine(const std::string& prompt) {
    std::cout << prompt;
    std::string line;
    if (!std::getline(std::cin, line)) {
        // End of input (e.g. Ctrl+D / Ctrl+Z): exit cleanly instead of looping forever.
        std::cout << "\nInput closed. Goodbye!\n";
        std::exit(0);
    }
    return line;
}

int readInt(const std::string& prompt, int min, int max) {
    while (true) {
        std::string line = readLine(prompt);
        std::istringstream iss(line);
        int value;
        char extra;
        if (iss >> value && !(iss >> extra) && value >= min && value <= max) {
            return value;
        }
        std::cout << "  Please enter a number from " << min << " to " << max << ".\n";
    }
}

char readChoice(const std::string& prompt, const std::string& allowed) {
    while (true) {
        std::string line = readLine(prompt);
        if (line.size() == 1) {
            char c = static_cast<char>(std::toupper(static_cast<unsigned char>(line[0])));
            if (allowed.find(c) != std::string::npos) return c;
        }
        std::cout << "  Please type one of: " << allowed << "\n";
    }
}

}  // namespace input
