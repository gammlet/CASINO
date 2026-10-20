// Small self-checking test program. Build target: test_hands
// Run it after changing Hand or Poker code to catch mistakes early.

#include <iostream>
#include <string>

#include "Deck.h"
#include "Hand.h"
#include "Player.h"
#include "Poker.h"

static int failures = 0;

static void check(bool condition, const std::string& description) {
    std::cout << (condition ? "  PASS  " : "  FAIL  ") << description << "\n";
    if (!condition) ++failures;
}

static Hand makeHand(std::initializer_list<Card> cards) {
    Hand h;
    for (const Card& c : cards) h.addCard(c);
    return h;
}

int main() {
    std::cout << "Deck\n";
    {
        Deck d;
        check(d.size() == 52, "new deck has 52 cards");
        d.deal();
        check(d.size() == 51, "dealing removes one card");
    }

    std::cout << "Blackjack scoring\n";
    check(makeHand({{Rank::King, Suit::Spades}, {Rank::Seven, Suit::Hearts}}).blackjackScore() == 17,
          "K + 7 = 17");
    check(makeHand({{Rank::Ace, Suit::Spades}, {Rank::King, Suit::Hearts}}).isBlackjack(),
          "A + K is blackjack");
    check(makeHand({{Rank::Ace, Suit::Spades}, {Rank::Ace, Suit::Hearts}}).blackjackScore() == 12,
          "A + A = 12");
    check(makeHand({{Rank::Ace, Suit::Spades}, {Rank::Nine, Suit::Hearts},
                    {Rank::Five, Suit::Clubs}}).blackjackScore() == 15,
          "A + 9 + 5 = 15 (Ace becomes 1)");
    check(makeHand({{Rank::King, Suit::Spades}, {Rank::Queen, Suit::Hearts},
                    {Rank::Five, Suit::Clubs}}).isBust(),
          "K + Q + 5 is bust");

    std::cout << "Poker hand evaluation\n";
    {
        using R = Rank;
        using S = Suit;
        Hand highCard = makeHand({{R::Ace, S::Spades}, {R::Jack, S::Hearts}, {R::Nine, S::Clubs},
                                  {R::Six, S::Diamonds}, {R::Three, S::Spades}});
        Hand pairKings = makeHand({{R::King, S::Spades}, {R::King, S::Hearts}, {R::Nine, S::Clubs},
                                   {R::Four, S::Diamonds}, {R::Two, S::Spades}});
        Hand pairQueens = makeHand({{R::Queen, S::Spades}, {R::Queen, S::Hearts}, {R::Ace, S::Clubs},
                                    {R::Four, S::Diamonds}, {R::Two, S::Spades}});
        Hand twoPair = makeHand({{R::King, S::Spades}, {R::King, S::Hearts}, {R::Nine, S::Clubs},
                                 {R::Nine, S::Diamonds}, {R::Three, S::Spades}});
        Hand trips = makeHand({{R::Seven, S::Spades}, {R::Seven, S::Hearts}, {R::Seven, S::Clubs},
                               {R::King, S::Diamonds}, {R::Two, S::Spades}});
        Hand straight = makeHand({{R::Five, S::Spades}, {R::Six, S::Hearts}, {R::Seven, S::Clubs},
                                  {R::Eight, S::Diamonds}, {R::Nine, S::Spades}});
        Hand wheel = makeHand({{R::Ace, S::Spades}, {R::Two, S::Hearts}, {R::Three, S::Clubs},
                               {R::Four, S::Diamonds}, {R::Five, S::Spades}});
        Hand flush = makeHand({{R::Two, S::Hearts}, {R::Six, S::Hearts}, {R::Nine, S::Hearts},
                               {R::Jack, S::Hearts}, {R::King, S::Hearts}});
        Hand fullHouse = makeHand({{R::Three, S::Spades}, {R::Three, S::Hearts}, {R::Three, S::Clubs},
                                   {R::Ten, S::Diamonds}, {R::Ten, S::Spades}});
        Hand quads = makeHand({{R::Ace, S::Spades}, {R::Ace, S::Hearts}, {R::Ace, S::Clubs},
                               {R::Ace, S::Diamonds}, {R::Two, S::Spades}});
        Hand straightFlush = makeHand({{R::Five, S::Clubs}, {R::Six, S::Clubs}, {R::Seven, S::Clubs},
                                       {R::Eight, S::Clubs}, {R::Nine, S::Clubs}});
        Hand royal = makeHand({{R::Ten, S::Spades}, {R::Jack, S::Spades}, {R::Queen, S::Spades},
                               {R::King, S::Spades}, {R::Ace, S::Spades}});

        check(evaluatePokerHand(highCard).category == HandCategory::HighCard, "A J 9 6 3 is High Card");
        check(evaluatePokerHand(pairKings).category == HandCategory::OnePair, "K K 9 4 2 is One Pair");
        check(evaluatePokerHand(twoPair).category == HandCategory::TwoPair, "K K 9 9 3 is Two Pair");
        check(evaluatePokerHand(trips).category == HandCategory::ThreeOfAKind, "7 7 7 K 2 is Three of a Kind");
        check(evaluatePokerHand(straight).category == HandCategory::Straight, "5 6 7 8 9 is a Straight");
        check(evaluatePokerHand(wheel).category == HandCategory::Straight, "A 2 3 4 5 is a Straight");
        check(evaluatePokerHand(flush).category == HandCategory::Flush, "all hearts is a Flush");
        check(evaluatePokerHand(fullHouse).category == HandCategory::FullHouse, "3 3 3 10 10 is a Full House");
        check(evaluatePokerHand(quads).category == HandCategory::FourOfAKind, "A A A A 2 is Four of a Kind");
        check(evaluatePokerHand(straightFlush).category == HandCategory::StraightFlush, "5-9 clubs is a Straight Flush");
        check(evaluatePokerHand(royal).category == HandCategory::RoyalFlush, "10-A spades is a Royal Flush");

        std::cout << "Poker comparisons\n";
        check(comparePokerHands(flush, pairKings) > 0, "Flush beats One Pair");
        check(comparePokerHands(pairKings, pairQueens) > 0, "Pair of Kings beats pair of Queens (despite Ace kicker)");
        check(comparePokerHands(straight, wheel) > 0, "9-high straight beats A-2-3-4-5");
        check(comparePokerHands(fullHouse, flush) > 0, "Full House beats Flush");
        check(comparePokerHands(highCard, pairQueens) < 0, "High Card loses to One Pair");
        check(comparePokerHands(pairKings, pairKings) == 0, "identical hands tie");
    }

    std::cout << "Player bets\n";
    {
        Player p("Test", 100);
        check(!p.placeBet(150), "cannot bet more than you have");
        check(p.placeBet(40) && p.getChips() == 60, "bet 40 leaves 60");
        check(p.doubleBet() && p.getBet() == 80 && p.getChips() == 20, "double down to 80");
        p.winBet(2.0);
        check(p.getChips() == 180, "winning 80 at 1:1 gives 180");
        p.placeBet(10);
        p.winBet(2.5);
        check(p.getChips() == 195, "blackjack 3:2 on 10 pays 15");
    }

    std::cout << "\n" << (failures == 0 ? "All tests passed." : std::to_string(failures) + " test(s) failed.")
              << "\n";
    return failures == 0 ? 0 : 1;
}
