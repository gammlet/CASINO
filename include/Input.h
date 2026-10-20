#pragma once

#include <string>

// Safe console input helpers. They keep asking until the user types
// something valid, so bad input (like "abc" for a number) never crashes the game.
namespace input {

// Reads a whole number between min and max (inclusive).
int readInt(const std::string& prompt, int min, int max);

// Reads one character from `allowed` (case-insensitive) and returns it uppercased.
// Example: readChoice("(H)it or (S)tand? ", "HS")
char readChoice(const std::string& prompt, const std::string& allowed);

// Reads a full line of text (may be empty).
std::string readLine(const std::string& prompt);

}  // namespace input
