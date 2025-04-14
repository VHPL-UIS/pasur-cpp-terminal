#include "Game.hpp"
#include <iostream>

Game::Game()
{
    round = 1;
    dealCards();
}

void Game::dealCards()
{
    if (deck.isEmpty())
    {
        std::cout << "Deck is empty. Cannot deal cards." << std::endl;
        return;
    }

    playerHnad.clear();
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
            tableCards.push_back(deck.drawCard());
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

    std::cout << "Player's turn." << std::endl;
    printPlayerHand();
    printTableCards();

    std::cout << "Choose a card to play (1-" << playerHand.size() << "): ";
    int choice;
    std::cin >> choice;
    if (choice >= 1 && choice <= playerHand.size())
    {
        Card chosenCard = playerHand[choice - 1];
        playerHand.erase(playerHand.begin() + choice - 1);
        std::cout << "You played: " << chosenCard.toString() << std::endl;
    }
    else
    {
        std::cout << "Invalid choice. Try again." << std::endl;
        playerTurn();
    }
}