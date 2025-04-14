#include "Deck.hpp"
#include <algorithm>
#include <random>

Deck::Deck()
{
    for (int suit = 0; suit < 4; ++suit)
    {
        for (int rank = 1; rank <= 13; ++rank)
        {
            cards.emplace_back(static_cast<Rank>(rank), static_cast<Suit>(suit));
        }
    }
    shuffle();
}

Card Deck::drawCard()
{
    if (isEmpty())
    {
        throw std::out_of_range("No cards left in the deck");
    }
    Card card = cards.back();
    cards.pop_back();
    return card;
}

void Deck::shuffle()
{
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(cards.begin(), cards.end(), g);
}

bool Deck::isEmpty() const
{
    return cards.empty();
}