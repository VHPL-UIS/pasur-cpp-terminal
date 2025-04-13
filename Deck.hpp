#ifndef __DECK_HPP__
#define __DECK_HPP__

#include "Card.hpp"
#include <vector>

class Deck
{
public:
    Deck();
    void shuffle();
    // Card drawCard();
    bool isEmpty() const;
    // void resetDeck();
    const std::vector<Card> &getCards() const { return cards; }

private:
    std::vector<Card> cards;
};

#endif // __DECK_HPP__