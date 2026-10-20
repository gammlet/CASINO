#pragma once

#include "Deck.h"
#include "Game.h"
#include "Hand.h"

// Classic Blackjack: one player against the computer dealer.
// Rules: dealer hits until 17, blackjack pays 3:2, double down on the first decision.
class Blackjack : public Game {
public:
    std::string getName() const override;
    void playRound(Player& player) override;

private:
    void dealInitialCards();
    void showTable(bool revealDealer) const;
    void playerTurn(Player& player);   // hit / stand / double down
    void dealerTurn();                 // dealer hits until 17 or more
    void settleBet(Player& player);    // compare hands and pay out

    Deck deck_;
    Hand playerHand_;
    Hand dealerHand_;

    static constexpr std::size_t kReshuffleAt = 15;   // reset the deck when this low
    static constexpr int kDealerStandsOn = 17;
};
