#pragma once

#include <random>
#include <vector>

#include "Card.h"

// A standard 52-card deck that can shuffle and deal.
class Deck {
public:
    Deck();                 // builds a fresh, shuffled deck

    void reset();           // put all 52 cards back
    void shuffle();         // randomize the order
    Card deal();            // take the top card (throws if empty)
    std::size_t size() const;
    bool empty() const;

private:
    std::vector<Card> cards_;
    std::mt19937 rng_;      // modern C++ random engine (better than rand())
};
