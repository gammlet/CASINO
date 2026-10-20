#pragma once

#include <string>

#include "Player.h"

// Abstract base class for every casino game.
// Casino only talks to games through this interface (polymorphism),
// so adding a new game later (e.g. Roulette) needs no changes to Casino.
class Game {
public:
    virtual ~Game() = default;

    // Name shown in the menu, e.g. "Blackjack".
    virtual std::string getName() const = 0;

    // Play one full round with the player: take a bet, play, pay out.
    virtual void playRound(Player& player) = 0;
};
