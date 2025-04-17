#include <gtest/gtest.h>
#include "Game.hpp"

class GameTest : public ::testing::Test
{
protected:
    Game game;

    void SetUp() override
    {
        game.clearTable();
        game.setPlayerTurn(true);
    }

    void playCard(const Card &card)
    {
        game.collectCards(card);
    }

    bool isCollected(const Card &card)
    {
        auto &playerCards = game.getPlayerCollection();
        auto it = playerCards.find(card.getSuit());
        if (it != playerCards.end())
        {
            return std::find(it->second.begin(), it->second.end(), card) != it->second.end();
        }

        return false;
    }
};

TEST_F(GameTest, JackCollectsAllNonFaceCards)
{
    game.setTableCards({Card(Rank::Two, Suit::Hearts), Card(Rank::Queen, Suit::Spades), Card(Rank::Nine, Suit::Diamonds)});

    Card jack(Rank::Jack, Suit::Clubs);
    playCard(jack);

    EXPECT_TRUE(isCollected(Card(Rank::Two, Suit::Hearts)));
    EXPECT_FALSE(isCollected(Card(Rank::Queen, Suit::Spades)));
    EXPECT_TRUE(isCollected(Card(Rank::Nine, Suit::Diamonds)));
    EXPECT_TRUE(isCollected(jack));
}

TEST_F(GameTest, KingCollectsMatchingKingWithPreferenceForClubs)
{
    game.setTableCards({Card(Rank::King, Suit::Hearts), Card(Rank::King, Suit::Clubs)});

    Card king(Rank::King, Suit::Spades);
    playCard(king);

    EXPECT_TRUE(isCollected(Card(Rank::King, Suit::Clubs)));
    EXPECT_FALSE(isCollected(Card(Rank::King, Suit::Hearts)));
    EXPECT_TRUE(isCollected(king));
}

TEST_F(GameTest, NumberCardPicksCardsSummingTo11)
{
    game.setTableCards({Card(Rank::Three, Suit::Hearts), Card(Rank::Seven, Suit::Diamonds)});

    Card eight(Rank::Eight, Suit::Clubs);
    playCard(eight);

    EXPECT_TRUE(isCollected(Card(Rank::Three, Suit::Hearts)));
    EXPECT_TRUE(isCollected(eight));
}

TEST_F(GameTest, NoCombinationPutsCardOnTable)
{
    game.setTableCards({Card(Rank::Two, Suit::Hearts), Card(Rank::Three, Suit::Spades)});

    Card four(Rank::Four, Suit::Clubs);
    playCard(four);

    EXPECT_FALSE(isCollected(four));
    EXPECT_TRUE(game.isCardOnTable(four));
}
