#include "Card.hpp"

Card::Card(Rank r, Suit s) : rank(r), suit(s) {}

std::string Card::toString() const
{
    std::string rankStr;
    std::string suitStr;

    switch (rank)
    {
    case Rank::Ace:
        rankStr = "1";
        break;
    case Rank::Two:
        rankStr = "2";
        break;
    case Rank::Three:
        rankStr = "3";
        break;
    case Rank::Four:
        rankStr = "4";
        break;
    case Rank::Five:
        rankStr = "5";
        break;
    case Rank::Six:
        rankStr = "6";
        break;
    case Rank::Seven:
        rankStr = "7";
        break;
    case Rank::Eight:
        rankStr = "8";
        break;
    case Rank::Nine:
        rankStr = "9";
        break;
    case Rank::Ten:
        rankStr = "10";
        break;
    case Rank::Jack:
        rankStr = "J";
        break;
    case Rank::Queen:
        rankStr = "Q";
        break;
    case Rank::King:
        rankStr = "K";
        break;
    default:
        rankStr = "Unknown";
        break;
    }

    switch (suit)
    {
    case Suit::Hearts:
        suitStr = "♥";
        break;
    case Suit::Diamonds:
        suitStr = "♦";
        break;
    case Suit::Clubs:
        suitStr = "♣";
        break;
    case Suit::Spades:
        suitStr = "♠";
        break;
    default:
        suitStr = "Unknown";
        break;
    }

    std::string red = "\033[31m";
    std::string black = "\033[30m";
    std::string reset = "\033[0m";
    std::string bold = "\033[1m";

    std::string color = (suit == Suit::Hearts || suit == Suit::Diamonds) ? red : black;

    return bold + color + rankStr + suitStr + reset;
}