#include "Card.h"

Card::Card(Rank rank, Suit suit) : rank_(rank), suit_(suit) {}

Rank Card::getRank() const { return rank_; }

Suit Card::getSuit() const { return suit_; }

int Card::blackjackValue() const {
    int r = static_cast<int>(rank_);
    if (rank_ == Rank::Ace) return 11;
    if (r >= 10) return 10;          // Ten, Jack, Queen, King
    return r;                        // Two .. Nine
}

std::string Card::toString() const {
    std::string rankText;
    switch (rank_) {
        case Rank::Jack:  rankText = "J"; break;
        case Rank::Queen: rankText = "Q"; break;
        case Rank::King:  rankText = "K"; break;
        case Rank::Ace:   rankText = "A"; break;
        default:          rankText = std::to_string(static_cast<int>(rank_));
    }

    std::string suitText;
    switch (suit_) {
        case Suit::Clubs:    suitText = "♣"; break;  // ♣
        case Suit::Diamonds: suitText = "♦"; break;  // ♦
        case Suit::Hearts:   suitText = "♥"; break;  // ♥
        case Suit::Spades:   suitText = "♠"; break;  // ♠
    }

    return "[" + rankText + suitText + "]";
}
