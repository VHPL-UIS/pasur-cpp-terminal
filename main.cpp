#include "Deck.hpp"
#include "Card.hpp"
#include <iostream>
#include <vector>

int main()
{
    Deck deck;
    std::cout << "Deck created with " << deck.getCards().size() << " cards." << std::endl;

    std::cout << "Deck contains:" << std::endl;
    auto cards = deck.getCards();
    for (const Card &card : cards)
    {
        std::cout << card.toString() << std::endl;
    }

    return 0;
}