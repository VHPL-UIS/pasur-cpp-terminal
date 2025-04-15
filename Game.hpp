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
    void play();

private:
    Deck deck;
    std::vector<Card> playerHand;
    std::vector<Card> cpuHand;
    std::vector<Card> tableCards;
    std::unordered_map<Suit, std::vector<Card>> playerCollectionCards;
    std::unordered_map<Suit, std::vector<Card>> cpuCollectionCards;
    // use hash tables for calculating the score
    bool isPlayerTurn;
    int round;

    void dealCards();
    void playerTurn();
    void cpuTurn();
    void printTableCards() const;
    void printPlayerHand() const;
    void findCombinationSum(const std::vector<Card> &cards, int target, int start, std::vector<Card> &combination, std::vector<std::vector<Card>> &result);
    void collectCards(const Card &card);
    void calculateScore();
};

#endif // __GAME_HPP__