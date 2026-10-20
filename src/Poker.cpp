#include "Poker.h"

#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <utility>

#include "Input.h"

// ---------------------------------------------------------------------------
// Hand evaluation
// ---------------------------------------------------------------------------

HandValue evaluatePokerHand(const Hand& hand) {
    const std::vector<Card>& cards = hand.getCards();

    // 1. Count how many times each rank appears, and check for a flush.
    std::map<int, int> rankCounts;
    bool flush = !cards.empty();
    for (const Card& c : cards) {
        ++rankCounts[static_cast<int>(c.getRank())];
        if (c.getSuit() != cards[0].getSuit()) flush = false;
    }

    // 2. Group ranks by (count, rank), biggest group first, then highest rank.
    //    K K 9 9 3 -> (2,K) (2,9) (1,3)
    std::vector<std::pair<int, int>> groups;   // (count, rank)
    for (const auto& [rank, count] : rankCounts) groups.emplace_back(count, rank);
    std::sort(groups.begin(), groups.end(), std::greater<>());

    std::vector<int> tiebreakers;
    for (const auto& g : groups) tiebreakers.push_back(g.second);

    // 3. Straight: 5 different ranks in a row. A-2-3-4-5 (the "wheel") counts, with 5 high.
    bool straight = false;
    if (cards.size() == 5 && rankCounts.size() == 5) {
        int high = tiebreakers.front();
        int low = tiebreakers.back();
        if (high - low == 4) {
            straight = true;
        } else if (tiebreakers == std::vector<int>{14, 5, 4, 3, 2}) {
            straight = true;
            tiebreakers = {5, 4, 3, 2, 1};   // Ace plays low
        }
    }

    // 4. Pick the category.
    HandCategory category;
    int biggest = groups.empty() ? 0 : groups[0].first;
    int second = groups.size() > 1 ? groups[1].first : 0;

    if (straight && flush) {
        category = (tiebreakers.front() == 14) ? HandCategory::RoyalFlush : HandCategory::StraightFlush;
    } else if (biggest == 4) {
        category = HandCategory::FourOfAKind;
    } else if (biggest == 3 && second == 2) {
        category = HandCategory::FullHouse;
    } else if (flush) {
        category = HandCategory::Flush;
    } else if (straight) {
        category = HandCategory::Straight;
    } else if (biggest == 3) {
        category = HandCategory::ThreeOfAKind;
    } else if (biggest == 2 && second == 2) {
        category = HandCategory::TwoPair;
    } else if (biggest == 2) {
        category = HandCategory::OnePair;
    } else {
        category = HandCategory::HighCard;
    }

    return HandValue{category, tiebreakers};
}

int comparePokerHands(const Hand& a, const Hand& b) {
    HandValue va = evaluatePokerHand(a);
    HandValue vb = evaluatePokerHand(b);
    if (va.category != vb.category) {
        return static_cast<int>(va.category) - static_cast<int>(vb.category);
    }
    for (std::size_t i = 0; i < va.tiebreakers.size() && i < vb.tiebreakers.size(); ++i) {
        if (va.tiebreakers[i] != vb.tiebreakers[i]) {
            return va.tiebreakers[i] - vb.tiebreakers[i];
        }
    }
    return 0;
}

const char* categoryName(HandCategory category) {
    switch (category) {
        case HandCategory::HighCard:      return "High Card";
        case HandCategory::OnePair:       return "One Pair";
        case HandCategory::TwoPair:       return "Two Pair";
        case HandCategory::ThreeOfAKind:  return "Three of a Kind";
        case HandCategory::Straight:      return "Straight";
        case HandCategory::Flush:         return "Flush";
        case HandCategory::FullHouse:     return "Full House";
        case HandCategory::FourOfAKind:   return "Four of a Kind";
        case HandCategory::StraightFlush: return "Straight Flush";
        case HandCategory::RoyalFlush:    return "Royal Flush";
    }
    return "Unknown";
}

// ---------------------------------------------------------------------------
// The game
// ---------------------------------------------------------------------------

std::string Poker::getName() const { return "Poker (Five-Card Draw)"; }

void Poker::playRound(Player& player) {
    std::cout << "\n--- Poker: Five-Card Draw ---\n";
    std::cout << "You have " << player.getChips() << " chips.\n";

    int bet = input::readInt("Your bet: ", 1, player.getChips());
    player.placeBet(bet);

    // Fresh deck every round: 10 cards dealt + up to 6 swapped is well under 52.
    deck_.reset();
    playerHand_.clear();
    computerHand_.clear();
    for (int i = 0; i < 5; ++i) {
        playerHand_.addCard(deck_.deal());
        computerHand_.addCard(deck_.deal());
    }

    std::cout << "\nYour hand:\n";
    showHand(playerHand_, true);

    drawPhase(playerHand_);
    computerDraw(computerHand_);

    std::cout << "\nYour hand after the draw:\n";
    showHand(playerHand_, false);

    char choice = input::readChoice("(S)howdown or (F)old? ", "SF");
    if (choice == 'F') {
        std::cout << "You fold and lose " << player.getBet() << " chips.\n";
        player.loseBet();
        return;
    }

    if (computerFolds(computerHand_)) {
        std::cout << "The computer folds! You win " << player.getBet() << " chips.\n";
        player.winBet(2.0);
        return;
    }

    std::cout << "\nShowdown!\n";
    std::cout << "You:      ";
    showHand(playerHand_, false);
    std::cout << "Computer: ";
    showHand(computerHand_, false);

    int result = comparePokerHands(playerHand_, computerHand_);
    if (result > 0) {
        std::cout << "You win " << player.getBet() << " chips!\n";
        player.winBet(2.0);
    } else if (result < 0) {
        std::cout << "Computer wins. You lose " << player.getBet() << " chips.\n";
        player.loseBet();
    } else {
        std::cout << "It's a tie. Your bet is returned.\n";
        player.pushBet();
    }
}

void Poker::showHand(const Hand& hand, bool numbered) const {
    if (numbered) {
        const std::vector<Card>& cards = hand.getCards();
        for (std::size_t i = 0; i < cards.size(); ++i) {
            std::cout << "  " << (i + 1) << ":" << cards[i].toString();
        }
        std::cout << "\n";
    } else {
        std::cout << hand.toString();
    }
    std::cout << "  (" << categoryName(evaluatePokerHand(hand).category) << ")\n";
}

void Poker::drawPhase(Hand& hand) {
    while (true) {
        std::string line = input::readLine("Cards to swap, up to 3 (e.g. 1 3 5, Enter for none): ");
        std::istringstream iss(line);
        std::set<int> picks;
        int n;
        bool valid = true;

        while (iss >> n) {
            if (n < 1 || n > 5) valid = false;
            picks.insert(n);
        }
        if (!iss.eof()) valid = false;               // something that wasn't a number
        if (picks.size() > kMaxSwap) valid = false;

        if (!valid) {
            std::cout << "  Enter up to 3 different numbers from 1 to 5, separated by spaces.\n";
            continue;
        }

        for (int pick : picks) {
            hand.replaceCard(static_cast<std::size_t>(pick - 1), deck_.deal());
        }
        if (!picks.empty()) {
            std::cout << "You swapped " << picks.size() << " card(s).\n";
        }
        return;
    }
}

void Poker::computerDraw(Hand& hand) {
    HandValue value = evaluatePokerHand(hand);

    // Made hands (straight or better) use all five cards: keep them.
    if (value.category >= HandCategory::Straight) {
        std::cout << "The computer stands pat.\n";
        return;
    }

    // Count ranks so we know which cards are part of a pair/trips.
    std::map<Rank, int> counts;
    for (const Card& c : hand.getCards()) ++counts[c.getRank()];

    // Candidates to swap: cards not part of a group, lowest rank first.
    std::vector<std::size_t> loners;
    for (std::size_t i = 0; i < hand.size(); ++i) {
        if (counts[hand.getCards()[i].getRank()] == 1) loners.push_back(i);
    }
    std::sort(loners.begin(), loners.end(), [&hand](std::size_t a, std::size_t b) {
        return hand.getCards()[a].getRank() < hand.getCards()[b].getRank();
    });

    // With nothing, keep the two highest cards and swap three.
    std::size_t swapCount = std::min(loners.size(), kMaxSwap);
    for (std::size_t i = 0; i < swapCount; ++i) {
        hand.replaceCard(loners[i], deck_.deal());
    }
    std::cout << "The computer swaps " << swapCount << " card(s).\n";
}

bool Poker::computerFolds(const Hand& hand) const {
    // Simple AI: fold a high-card hand unless it holds an Ace or King.
    HandValue value = evaluatePokerHand(hand);
    return value.category == HandCategory::HighCard && value.tiebreakers.front() < 13;
}
