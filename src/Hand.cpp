#include "Hand.h"

#include <stdexcept>

void Hand::addCard(const Card& card) { cards_.push_back(card); }

void Hand::replaceCard(std::size_t index, const Card& card) {
    if (index >= cards_.size()) {
        throw std::out_of_range("Hand::replaceCard index out of range");
    }
    cards_[index] = card;
}

void Hand::clear() { cards_.clear(); }

const std::vector<Card>& Hand::getCards() const { return cards_; }

std::size_t Hand::size() const { return cards_.size(); }

int Hand::blackjackScore() const {
    int total = 0;
    int aces = 0;
    for (const Card& c : cards_) {
        total += c.blackjackValue();
        if (c.getRank() == Rank::Ace) ++aces;
    }
    // Count Aces as 1 instead of 11, one at a time, while we'd bust.
    while (total > 21 && aces > 0) {
        total -= 10;
        --aces;
    }
    return total;
}

bool Hand::isBust() const { return blackjackScore() > 21; }

bool Hand::isBlackjack() const { return cards_.size() == 2 && blackjackScore() == 21; }

std::string Hand::toString() const {
    std::string out;
    for (std::size_t i = 0; i < cards_.size(); ++i) {
        if (i > 0) out += " ";
        out += cards_[i].toString();
    }
    return out;
}
