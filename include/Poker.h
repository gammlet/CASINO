#pragma once

#include <vector>

#include "Deck.h"
#include "Game.h"
#include "Hand.h"

// Poker hand categories, weakest to strongest.
enum class HandCategory {
    HighCard, OnePair, TwoPair, ThreeOfAKind, Straight,
    Flush, FullHouse, FourOfAKind, StraightFlush, RoyalFlush
};

// The result of evaluating a 5-card hand.
// `tiebreakers` holds rank values in order of importance, so two hands of the
// same category can be compared (e.g. pair of Kings beats pair of Queens).
struct HandValue {
    HandCategory category;
    std::vector<int> tiebreakers;
};

// Free functions so they can be unit-tested on their own (see tests/).
HandValue evaluatePokerHand(const Hand& hand);
int comparePokerHands(const Hand& a, const Hand& b);   // >0 a wins, <0 b wins, 0 tie
const char* categoryName(HandCategory category);

// Five-Card Draw against one computer opponent.
// Round: ante -> see 5 cards -> swap up to 3 -> stay in or fold -> showdown.
class Poker : public Game {
public:
    std::string getName() const override;
    void playRound(Player& player) override;

private:
    void showHand(const Hand& hand, bool numbered) const;
    void drawPhase(Hand& hand);        // player picks up to 3 cards to swap
    void computerDraw(Hand& hand);     // simple AI swaps weak cards
    bool computerFolds(const Hand& hand) const;

    Deck deck_;
    Hand playerHand_;
    Hand computerHand_;

    static constexpr std::size_t kMaxSwap = 3;
};
