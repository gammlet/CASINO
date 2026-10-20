#pragma once

#include <string>

// The four suits of a standard deck.
enum class Suit { Clubs, Diamonds, Hearts, Spades };

// Ranks are numbered so that comparing them works naturally (Two < Ace).
enum class Rank {
    Two = 2, Three, Four, Five, Six, Seven, Eight, Nine, Ten,
    Jack, Queen, King, Ace
};

// A single playing card. Immutable once created.
class Card {
public:
    Card(Rank rank, Suit suit);

    Rank getRank() const;
    Suit getSuit() const;

    // Blackjack value: 2-10 face value, J/Q/K = 10, Ace = 11.
    // (Hand::blackjackScore() turns Aces into 1 when needed.)
    int blackjackValue() const;

    // Short text form, e.g. "[K♠]" or "[10♥]".
    std::string toString() const;

private:
    Rank rank_;
    Suit suit_;
};
