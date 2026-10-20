#pragma once

#include <string>

// The person at the table: a name and a chip balance.
// Chips are removed when a bet is placed and added back on a win.
class Player {
public:
    explicit Player(std::string name, int chips = 500);

    const std::string& getName() const;
    int getChips() const;
    void setChips(int chips);   // used when loading a save file

    bool canBet(int amount) const;
    bool placeBet(int amount);  // false if amount is invalid or too large
    int getBet() const;
    bool doubleBet();           // Blackjack double down: bet the same amount again

    void winBet(double payoutMultiplier); // e.g. 2.0 = bet back + equal win, 2.5 = blackjack 3:2
    void pushBet();             // tie: return the bet
    void loseBet();             // bet is gone

    bool isBroke() const;

private:
    std::string name_;
    int chips_;
    int currentBet_ = 0;
};
