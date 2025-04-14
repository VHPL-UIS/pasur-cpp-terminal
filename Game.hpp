#ifndef __GAME_HPP__
#define __GAME_HPP__

#include "Deck.hpp"
#include "Card.hpp"
#include <unordered_map>
#include <vector>

class Game
{
public:
    Game();
    void start();

private:
    Deck deck;
    std::vector<Card> playerHand;
    std::vector<Card> cpuHand;
    std::vector<Card> tableCards;
    std::unordered_map<Card::Suit, std::vector<Card>> playerCollectionCards;
    std::unordered_map<Card::Suit, std::vector<Card>> cpuCollectionCards;
    // use hash tables for calculating the score

    void dealCards();
    void playerTurn();
    void cpuTurn();
    void printTableCards() const;
    void printPlayerHand() const;
    void captureCards();
    bool isPlayerTurn;
    int round;
};

#endif // __GAME_HPP__