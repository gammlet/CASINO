#include "Deck.h"

#include <algorithm>
#include <stdexcept>

Deck::Deck() : rng_(std::random_device{}()) {
    reset();
}

void Deck::reset() {
    cards_.clear();
    for (int s = 0; s < 4; ++s) {
        for (int r = static_cast<int>(Rank::Two); r <= static_cast<int>(Rank::Ace); ++r) {
            cards_.emplace_back(static_cast<Rank>(r), static_cast<Suit>(s));
        }
    }
    shuffle();
}

void Deck::shuffle() {
    std::shuffle(cards_.begin(), cards_.end(), rng_);
}

Card Deck::deal() {
    if (cards_.empty()) {
        throw std::runtime_error("Cannot deal from an empty deck");
    }
    Card top = cards_.back();
    cards_.pop_back();
    return top;
}

std::size_t Deck::size() const { return cards_.size(); }

bool Deck::empty() const { return cards_.empty(); }
