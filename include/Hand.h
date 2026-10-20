#pragma once

#include <string>
#include <vector>

#include "Card.h"

// The cards one player is currently holding.
class Hand {
public:
    void addCard(const Card& card);
    void replaceCard(std::size_t index, const Card& card);  // for Poker's draw phase
    void clear();

    const std::vector<Card>& getCards() const;
    std::size_t size() const;

    // --- Blackjack helpers ---
    int blackjackScore() const;   // best total, counting Aces as 1 or 11
    bool isBust() const;          // score over 21
    bool isBlackjack() const;     // exactly 2 cards totalling 21

    // e.g. "[K♠] [7♥]"
    std::string toString() const;

private:
    std::vector<Card> cards_;
};
