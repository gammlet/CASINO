#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Game.h"
#include "Player.h"

// Owns the player and the list of games, runs the main menu,
// and saves/loads the chip balance between sessions.
class Casino {
public:
    Casino();
    void run();

private:
    void showMenu() const;
    void loadPlayer();
    void savePlayer() const;

    Player player_;
    std::vector<std::unique_ptr<Game>> games_;   // polymorphism: any Game subclass

    static constexpr int kStartingChips = 500;
    static constexpr const char* kSaveFile = "data/save.txt";
};
