#include "Game.hpp"
#include <iostream>

Game::Game()
{
    round = 1;
}

void Game::play()
{
    std::cout << "Starting the game..." << std::endl;

    while (!deck.isEmpty())
    {
        dealCards();

        while (!playerHand.empty() && !cpuHand.empty())
        {
            playerTurn();
            cpuTurn();
        }
    }

    calculateScore();
}

void Game::dealCards()
{
    if (deck.isEmpty())
    {
        std::cout << "Deck is empty. Cannot deal cards." << std::endl;
        return;
    }

    playerHand.clear();
    cpuHand.clear();

    if (round > 1)
    {
        for (int i = 0; i < 4; ++i)
        {
            playerHand.push_back(deck.drawCard());
            cpuHand.push_back(deck.drawCard());
        }
    }
    else
    {
        tableCards.clear();
        for (int i = 0; i < 4; ++i)
        {
            playerHand.push_back(deck.drawCard());
            cpuHand.push_back(deck.drawCard());
            tableCards.push_back(deck.drawCard(true));
        }
    }

    ++round;
}

void Game::printTableCards() const
{
    std::cout << "Table Cards: ";
    for (const auto &card : tableCards)
    {
        std::cout << card.toString() << " ";
    }
    std::cout << std::endl;
}

void Game::printPlayerHand() const
{
    std::cout << "Player Hand: ";
    for (const auto &card : playerHand)
    {
        std::cout << card.toString() << " ";
    }
    std::cout << std::endl;
}

void Game::playerTurn()
{
    isPlayerTurn = true;
    std::cout << "Player's turn." << std::endl;
    printPlayerHand();
    printTableCards();

    std::cout << "Choose a card to play (1-" << playerHand.size() << "): ";
    size_t choice;
    std::cin >> choice;
    if (choice >= 1 && choice <= playerHand.size())
    {
        Card chosenCard = playerHand[choice - 1];
        playerHand.erase(playerHand.begin() + choice - 1);
        std::cout << "You played: " << chosenCard.toString() << std::endl;
        collectCards(chosenCard);
    }
    else
    {
        std::cout << "Invalid choice. Try again." << std::endl;
        playerTurn();
    }
}

void Game::cpuTurn()
{
    isPlayerTurn = false;
    std::cout << "CPU's turn." << std::endl;

    printTableCards();

    if (cpuHand.empty())
    {
        std::cout << "CPU has no cards left." << std::endl;
        return;
    }

    // Simple AI: CPU plays the last card in its hand
    Card chosenCard = cpuHand.back();
    cpuHand.pop_back();
    std::cout << "CPU played: " << chosenCard.toString() << std::endl;
    collectCards(chosenCard);
}

void Game::findCombinationSum(const std::vector<Card> &cards, int target, int start, std::vector<Card> &combination, std::vector<std::vector<Card>> &result)
{
    if (target == 0)
    {
        result.push_back(combination);
        return;
    }
    for (size_t i = start; i < cards.size(); ++i)
    {
        int cardRank = static_cast<int>(cards[i].getRank());
        if (cardRank <= target)
        {
            combination.push_back(cards[i]);
            findCombinationSum(cards, target - cardRank, i + 1, combination, result);
            combination.pop_back();
        }
    }
}

void Game::collectCards(const Card &card)
{
    if (card.getRank() == Rank::Jack)
    {
        if (tableCards.size() < 1)
        {
            tableCards.push_back(card);
            return;
        }

        for (auto it = tableCards.begin(); it != tableCards.end();)
        {
            if (it->getRank() != Rank::Queen && it->getRank() != Rank::King)
            {
                if (isPlayerTurn)
                {
                    playerCollectionCards[it->getSuit()].push_back(*it);
                }
                else
                {
                    cpuCollectionCards[it->getSuit()].push_back(*it);
                }
                it = tableCards.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }
    else if (card.getRank() == Rank::Queen || card.getRank() == Rank::King) // Queen or King
    {
        std::vector<Card> multipleCards;
        for (auto it = tableCards.begin(); it != tableCards.end();)
        {
            if (it->getRank() == card.getRank())
            {
                multipleCards.push_back(*it);
            }
            else
            {
                ++it;
            }
        }

        if (!multipleCards.empty())
        {
            bool hasClub = false;
            for (auto it = multipleCards.begin(); it != multipleCards.end();)
            {
                if (it->getSuit() == Suit::Clubs)
                {
                    hasClub = true;
                    if (isPlayerTurn)
                    {
                        playerCollectionCards[it->getSuit()].push_back(*it);
                    }
                    else
                    {
                        cpuCollectionCards[it->getSuit()].push_back(*it);
                    }
                    it = tableCards.erase(it);
                    break;
                }
                else
                {
                    ++it;
                }
            }

            if (!hasClub)
            {
                auto it = multipleCards.begin();
                if (isPlayerTurn)
                {
                    playerCollectionCards[it->getSuit()].push_back(*it);
                }
                else
                {
                    cpuCollectionCards[it->getSuit()].push_back(*it);
                }
                it = tableCards.erase(it);
            }
            multipleCards.clear();
        }

        tableCards.push_back(card);
    }
    else // Non-face cards
    {
        int target = 11 - static_cast<int>(card.getRank());
        std::vector<Card> combination;
        std::vector<std::vector<Card>> result;
        findCombinationSum(tableCards, target, 0, combination, result);

        if (result.empty())
        {
            std::cout << "No combination found." << std::endl;
            tableCards.push_back(card);
            return;
        }
        else
        {
            int counter = 0;
            for (const auto &comb : result)
            {
                std::cout << "Combination " << ++counter << ": ";
                for (const auto &c : comb)
                {
                    std::cout << c.toString() << " ";
                }
                std::cout << std::endl;
            }

            if (result.size() > 1)
            {
                std::cout << "Multiple combinations found. Choose one (1-" << result.size() << "): ";
                size_t choice;
                std::cin >> choice;
                if (choice >= 1 && choice <= result.size())
                {
                    for (const auto &c : result[choice - 1])
                    {
                        for (auto it = tableCards.begin(); it != tableCards.end();)
                        {
                            if (c.getRank() == it->getRank())
                            {
                                if (isPlayerTurn)
                                {
                                    playerCollectionCards[it->getSuit()].push_back(*it);
                                }
                                else
                                {
                                    cpuCollectionCards[it->getSuit()].push_back(*it);
                                }
                                it = tableCards.erase(it);
                            }
                            else
                            {
                                ++it;
                            }
                        }
                    }
                }
            }
            else
            {
                for (const auto &c : result[0])
                {
                    for (auto it = tableCards.begin(); it != tableCards.end();)
                    {
                        if (c.getRank() == it->getRank())
                        {
                            if (isPlayerTurn)
                            {
                                playerCollectionCards[it->getSuit()].push_back(*it);
                            }
                            else
                            {
                                cpuCollectionCards[it->getSuit()].push_back(*it);
                            }
                            it = tableCards.erase(it);
                        }
                        else
                        {
                            ++it;
                        }
                    }
                }
            }
        }
    }
}

void Game::calculateScore()
{
    int playerScore = 0;
    int cpuScore = 0;

    if (playerCollectionCards[Suit::Clubs].size() > 6)
    {
        playerScore += 7;
    }

    if (cpuCollectionCards[Suit::Clubs].size() > 6)
    {
        cpuScore += 7;
    }

    std::cout << "Player Score: " << playerScore << std::endl;
    std::cout << "CPU Score: " << cpuScore << std::endl;
}