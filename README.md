# Console Casino

A terminal-based casino written in C++17 for [Course code]. One player starts with
500 chips and can play **Blackjack** or **Five-Card Draw Poker** against the computer.
The chip balance is saved to `data/save.txt` between sessions.

## Team

| Name | Responsible for |
|------|-----------------|
| [Name] | Core classes (Card, Deck, Hand, Player) and Casino menu |
| [Name] | Blackjack |
| [Name] | Poker |

## Build and run

Requires a C++17 compiler and CMake 3.14+.

```bash
cmake -S . -B build
cmake --build build
./build/casino          # Windows: build\Debug\casino.exe
./build/test_hands      # run the tests
```

Run the game from the project folder so it finds `data/`.

Without CMake:

```bash
g++ -std=c++17 -Wall -Iinclude src/*.cpp -o casino
```

## Project structure

```
include/   class declarations (.h)
src/       class implementations (.cpp) and main.cpp
tests/     test_hands.cpp: checks deck, blackjack scoring and poker hands
data/      save file (created at runtime)
```

## Class design

- `Card`: rank and suit
- `Deck`: 52 cards, shuffle (std::mt19937), deal
- `Hand`: the cards a player holds; Blackjack scoring
- `Player`: name, chips, current bet
- `Game`: abstract base class with `getName()` and `playRound()`
- `Blackjack`, `Poker`: inherit from `Game`
- `Casino`: main menu, owns the player and games, saves/loads chips
- `input` helpers: safe number and choice input

## How to play

Enter your name, then pick a game from the menu by number. Your chips are saved
after every round. If you run out, the house gives you a fresh 500.

**Blackjack**
1. Place a bet.
2. You and the dealer get two cards; one dealer card stays hidden.
3. Type `H` to hit (take a card), `S` to stand, or `D` to double down
   (double your bet, take exactly one more card; first decision only).
4. Over 21 is a bust and you lose. The dealer then draws until reaching 17.
5. Closer to 21 wins 1:1. A natural blackjack (Ace + 10-value card) pays 3:2. Ties return your bet.

**Poker (Five-Card Draw)**
1. Place a bet. You and the computer each get five cards.
2. Type the positions of up to 3 cards to swap (e.g. `1 4 5`), or press Enter to keep all.
3. Choose `S` for showdown or `F` to fold (you lose your bet).
4. The computer may fold a weak hand, which means you win.
5. At showdown the better hand wins 1:1. Hand ranking, lowest to highest: High Card,
   One Pair, Two Pair, Three of a Kind, Straight, Flush, Full House, Four of a Kind,
   Straight Flush, Royal Flush.

## Status

- [x] Core classes, menu, save/load, input validation
- [x] Blackjack: hit, stand, double down, dealer AI, 3:2 blackjack payout
- [x] Poker: full hand evaluation with tie-breakers, draw phase, computer AI
- [x] Tests: 29 checks covering deck, blackjack scoring, every poker hand, bets
