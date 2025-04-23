#ifndef __GAME_HPP__
#define __GAME_HPP__

#include "Deck.hpp"
#include "Card.hpp"
#include <unordered_map>
#include <vector>
#include <algorithm>

class GameTest;

class Game
{
    friend class GameTest;

public:
    Game();
    void play();
    void clearTable() { tableCards.clear(); }
    void setPlayerTurn(bool turn) { isPlayerTurn = turn; }
    void setTableCards(const std::vector<Card> &cards) { tableCards = cards; }
    const std::unordered_map<Suit, std::vector<Card>> &getPlayerCollection() const { return playerCollectionCards; }
    bool isCardOnTable(const Card &card) const
    {
        return std::find(tableCards.begin(), tableCards.end(), card) != tableCards.end();
    }

private:
    Deck deck;
    std::vector<Card> playerHand;
    std::vector<Card> cpuHand;
    std::vector<Card> tableCards;
    std::unordered_map<Suit, std::vector<Card>> playerCollectionCards;
    std::unordered_map<Suit, std::vector<Card>> cpuCollectionCards;
    std::vector<Card> simulatedCpuHand;
    // use hash tables for calculating the score
    bool isPlayerTurn;
    int round;
    bool playerIsFinalHandWinner;
    int playerExtraScore;
    int cpuExtraScore;

    void dealCards();
    void playerTurn();
    void cpuTurn();
    void printTableCards() const;
    void printPlayerHand() const;
    void printCpuHand() const;
    void findCombinationSum(const std::vector<Card> &cards, int target, int start, std::vector<Card> &combination, std::vector<std::vector<Card>> &result);
    void collectCards(const Card &card, bool isSimulation = false);
    void calculateScore();
    void printPlayerCollection() const;
    void printCpuCollection() const;
    int cardScore(const Card &card);
    int evaluateCapturedCards(const Card &dropCard, const std::vector<Card> &cards, const std::vector<Card> &tableCards);
    void printCards(const std::vector<Card> &cards) const;
    void handleJack(const Card &card, bool isSimulation);
    void handleQueenKing(const Card &card, bool isSimulation);
    void handleNumbers(const Card &card, bool isSimulation);
    void collectCardInternal(const Card &c, bool isSimulation);
    void awardSoor();
};

#endif // __GAME_HPP__