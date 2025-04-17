#ifndef __CARD_HPP__
#define __CARD_HPP__

#include <string>

enum class Suit
{
    Hearts,
    Diamonds,
    Clubs,
    Spades
};

enum class Rank
{
    Ace = 1,
    Two,
    Three,
    Four,
    Five,
    Six,
    Seven,
    Eight,
    Nine,
    Ten,
    Jack,
    Queen,
    King
};

class Card
{
    Rank rank;
    Suit suit;

public:
    Card(Rank r, Suit s);
    Rank getRank() const { return rank; }
    Suit getSuit() const { return suit; }
    std::string toString() const;
    bool operator==(const Card &other) const
    {
        return rank == other.rank && suit == other.suit;
    }
};

#endif // __CARD_HPP__