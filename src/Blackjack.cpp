#include "Blackjack.h"

#include <iostream>

#include "Input.h"

std::string Blackjack::getName() const { return "Blackjack"; }

void Blackjack::playRound(Player& player) {
    std::cout << "\n--- Blackjack ---\n";
    std::cout << "You have " << player.getChips() << " chips.\n";

    int bet = input::readInt("Your bet: ", 1, player.getChips());
    player.placeBet(bet);

    if (deck_.size() < kReshuffleAt) {
        std::cout << "(Shuffling a fresh deck...)\n";
        deck_.reset();
    }

    dealInitialCards();

    // A natural blackjack on either side ends the round immediately.
    if (playerHand_.isBlackjack() || dealerHand_.isBlackjack()) {
        showTable(true);
        settleBet(player);
        return;
    }

    showTable(false);
    playerTurn(player);

    if (playerHand_.isBust()) {
        std::cout << "Bust! You lose " << player.getBet() << " chips.\n";
        player.loseBet();
        return;
    }

    dealerTurn();
    settleBet(player);
}

void Blackjack::dealInitialCards() {
    playerHand_.clear();
    dealerHand_.clear();
    // Deal in casino order: player, dealer, player, dealer.
    playerHand_.addCard(deck_.deal());
    dealerHand_.addCard(deck_.deal());
    playerHand_.addCard(deck_.deal());
    dealerHand_.addCard(deck_.deal());
}

void Blackjack::showTable(bool revealDealer) const {
    std::cout << "\nDealer: ";
    if (revealDealer) {
        std::cout << dealerHand_.toString() << "  = " << dealerHand_.blackjackScore() << "\n";
    } else {
        std::cout << dealerHand_.getCards()[0].toString() << " [??]\n";
    }
    std::cout << "You:    " << playerHand_.toString() << "  = " << playerHand_.blackjackScore() << "\n";
}

void Blackjack::playerTurn(Player& player) {
    bool firstDecision = true;

    while (!playerHand_.isBust() && playerHand_.blackjackScore() < 21) {
        bool canDouble = firstDecision && player.canBet(player.getBet());
        char choice = canDouble
            ? input::readChoice("(H)it, (S)tand or (D)ouble down? ", "HSD")
            : input::readChoice("(H)it or (S)tand? ", "HS");

        if (choice == 'S') {
            return;
        }

        if (choice == 'D') {
            player.doubleBet();
            std::cout << "You double down. Bet is now " << player.getBet() << ".\n";
            playerHand_.addCard(deck_.deal());
            showTable(false);
            return;   // exactly one card after doubling
        }

        // Hit
        playerHand_.addCard(deck_.deal());
        showTable(false);
        firstDecision = false;
    }
}

void Blackjack::dealerTurn() {
    std::cout << "\nDealer reveals: " << dealerHand_.toString()
              << "  = " << dealerHand_.blackjackScore() << "\n";

    while (dealerHand_.blackjackScore() < kDealerStandsOn) {
        Card c = deck_.deal();
        dealerHand_.addCard(c);
        std::cout << "Dealer draws " << c.toString() << "  = " << dealerHand_.blackjackScore() << "\n";
    }
}

void Blackjack::settleBet(Player& player) {
    int bet = player.getBet();
    int playerScore = playerHand_.blackjackScore();
    int dealerScore = dealerHand_.blackjackScore();

    if (playerHand_.isBlackjack() && dealerHand_.isBlackjack()) {
        std::cout << "Both have blackjack. Push, your bet is returned.\n";
        player.pushBet();
    } else if (playerHand_.isBlackjack()) {
        std::cout << "BLACKJACK! You win " << (bet * 3 / 2) << " chips.\n";
        player.winBet(2.5);
    } else if (dealerHand_.isBlackjack()) {
        std::cout << "Dealer has blackjack. You lose " << bet << " chips.\n";
        player.loseBet();
    } else if (dealerHand_.isBust()) {
        std::cout << "Dealer busts! You win " << bet << " chips.\n";
        player.winBet(2.0);
    } else if (playerScore > dealerScore) {
        std::cout << playerScore << " beats " << dealerScore << ". You win " << bet << " chips.\n";
        player.winBet(2.0);
    } else if (playerScore == dealerScore) {
        std::cout << "Both have " << playerScore << ". Push, your bet is returned.\n";
        player.pushBet();
    } else {
        std::cout << "Dealer's " << dealerScore << " beats your " << playerScore
                  << ". You lose " << bet << " chips.\n";
        player.loseBet();
    }
}
