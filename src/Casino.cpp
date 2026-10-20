#include "Casino.h"

#include <filesystem>
#include <fstream>
#include <iostream>

#include "Blackjack.h"
#include "Input.h"
#include "Poker.h"

Casino::Casino() : player_("Player", kStartingChips) {
    games_.push_back(std::make_unique<Blackjack>());
    games_.push_back(std::make_unique<Poker>());
}

void Casino::run() {
    std::cout << "=============================\n"
              << "       CONSOLE  CASINO\n"
              << "=============================\n";

    loadPlayer();

    while (true) {
        if (player_.isBroke()) {
            std::cout << "\nYou're out of chips! The house gives you a fresh "
                      << kStartingChips << " to start over.\n";
            player_.setChips(kStartingChips);
        }

        showMenu();
        int cashOut = static_cast<int>(games_.size()) + 1;
        int choice = input::readInt("> ", 1, cashOut);

        if (choice == cashOut) break;

        games_[choice - 1]->playRound(player_);   // virtual call
        savePlayer();                             // save after every round
    }

    savePlayer();
    std::cout << "\nYou leave with " << player_.getChips()
              << " chips. Thanks for playing, " << player_.getName() << "!\n";
}

void Casino::showMenu() const {
    std::cout << "\n" << player_.getName() << " | Chips: " << player_.getChips() << "\n";
    for (std::size_t i = 0; i < games_.size(); ++i) {
        std::cout << "  " << (i + 1) << ") " << games_[i]->getName() << "\n";
    }
    std::cout << "  " << (games_.size() + 1) << ") Cash out\n";
}

void Casino::loadPlayer() {
    std::ifstream in(kSaveFile);
    std::string name;
    int chips = 0;

    // Save file format: line 1 = name, line 2 = chips
    if (in && std::getline(in, name) && in >> chips && !name.empty() && chips > 0) {
        player_ = Player(name, chips);
        std::cout << "Welcome back, " << name << "!\n";
        return;
    }

    name = input::readLine("What's your name? ");
    if (name.empty()) name = "Player";
    player_ = Player(name, kStartingChips);
    std::cout << "Welcome, " << name << "! You start with " << kStartingChips << " chips.\n";
}

void Casino::savePlayer() const {
    std::error_code ec;
    std::filesystem::create_directories(std::filesystem::path(kSaveFile).parent_path(), ec);

    std::ofstream out(kSaveFile);
    if (!out) {
        std::cerr << "(Warning: could not save to " << kSaveFile << ")\n";
        return;
    }
    out << player_.getName() << "\n" << player_.getChips() << "\n";
}
