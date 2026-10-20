#include "Player.h"

#include <utility>

Player::Player(std::string name, int chips) : name_(std::move(name)), chips_(chips) {}

const std::string& Player::getName() const { return name_; }

int Player::getChips() const { return chips_; }

void Player::setChips(int chips) { chips_ = chips; }

bool Player::canBet(int amount) const { return amount > 0 && amount <= chips_; }

bool Player::placeBet(int amount) {
    if (!canBet(amount)) return false;
    chips_ -= amount;
    currentBet_ = amount;
    return true;
}

int Player::getBet() const { return currentBet_; }

bool Player::doubleBet() {
    if (currentBet_ <= 0 || currentBet_ > chips_) return false;
    chips_ -= currentBet_;
    currentBet_ *= 2;
    return true;
}

void Player::winBet(double payoutMultiplier) {
    chips_ += static_cast<int>(currentBet_ * payoutMultiplier);
    currentBet_ = 0;
}

void Player::pushBet() {
    chips_ += currentBet_;
    currentBet_ = 0;
}

void Player::loseBet() { currentBet_ = 0; }

bool Player::isBroke() const { return chips_ <= 0; }
