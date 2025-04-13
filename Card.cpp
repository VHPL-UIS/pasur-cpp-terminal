#include "Card.hpp"

Card::Card(Rank r, Suit s) : rank(r), suit(s) {}

std::string Card::toString() const
{
    std::string rankStr;
    std::string suitStr;

    switch (rank)
    {
    case Rank::Ace:
        rankStr = "Ace";
        break;
    case Rank::Two:
        rankStr = "Two";
        break;
    case Rank::Three:
        rankStr = "Three";
        break;
    case Rank::Four:
        rankStr = "Four";
        break;
    case Rank::Five:
        rankStr = "Five";
        break;
    case Rank::Six:
        rankStr = "Six";
        break;
    case Rank::Seven:
        rankStr = "Seven";
        break;
    case Rank::Eight:
        rankStr = "Eight";
        break;
    case Rank::Nine:
        rankStr = "Nine";
        break;
    case Rank::Ten:
        rankStr = "Ten";
        break;
    case Rank::Jack:
        rankStr = "Jack";
        break;
    case Rank::Queen:
        rankStr = "Queen";
        break;
    case Rank::King:
        rankStr = "King";
        break;
    default:
        rankStr = "Unknown";
        break;
    }

    switch (suit)
    {
    case Suit::Hearts:
        suitStr = "Hearts";
        break;
    case Suit::Diamonds:
        suitStr = "Diamonds";
        break;
    case Suit::Clubs:
        suitStr = "Clubs";
        break;
    case Suit::Spades:
        suitStr = "Spades";
        break;
    default:
        suitStr = "Unknown";
        break;
    }

    return rankStr + " of " + suitStr;
}