#include "Game.hpp"
#include <iostream>
#include <algorithm>

Game::Game()
{
    round = 1;
    playerIsFinalHandWinner = true;
    playerExtraScore = 0;
    cpuExtraScore = 0;
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
            // printPlayerCollection();
            cpuTurn();
            // printCpuCollection();
        }
    }

    if (!tableCards.empty())
    {
        for (const auto &card : tableCards)
        {
            if (playerIsFinalHandWinner)
            {
                playerCollectionCards[card.getSuit()].push_back(card);
            }
            else
            {
                cpuCollectionCards[card.getSuit()].push_back(card);
            }
        }
    }

    calculateScore();
}

void Game::dealCards()
{
    std::cout << "======>>>>>>> Round " << round << " <<<<<<=======" << std::endl;

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

    round++;
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
    std::cout << "Player Hand: \n";
    // for (const auto &card : playerHand)
    // {
    //     // std::cout << card.toString() << " ";
    // }
    printCards(playerHand);
}

void Game::printCpuHand() const
{
    std::cout << "CPU Hand: ";
    for (const auto &card : cpuHand)
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

    // printCpuHand();
    printTableCards();

    if (cpuHand.empty())
    {
        std::cout << "CPU has no cards left." << std::endl;
        return;
    }

    int chosenIndex = -1;
    int maxScore = -1;
    for (size_t i = 0; i < cpuHand.size(); ++i)
    {
        simulatedCpuHand.clear();
        collectCards(cpuHand[i], true);
        int score = evaluateCapturedCards(cpuHand[i], simulatedCpuHand, tableCards);
        if (score > maxScore)
        {
            maxScore = score;
            chosenIndex = i;
        }
    }

    if (chosenIndex == -1)
    {
        std::cout << "CPU has no valid cards to play." << std::endl;
        return;
    }
    Card chosenCard = cpuHand[chosenIndex];
    cpuHand.erase(cpuHand.begin() + chosenIndex);
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

void Game::collectCards(const Card &card, bool isSimulation)
{
    auto collectCard = [&](const Card &c)
    {
        if (isSimulation)
        {
            simulatedCpuHand.push_back(c);
            return;
        }

        auto &collector = isPlayerTurn ? playerCollectionCards : cpuCollectionCards;
        collector[c.getSuit()].push_back(c);
        playerIsFinalHandWinner = isPlayerTurn;
    };

    auto awardSoor = [&]()
    {
        std::cout << "SOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOORRRRR" << std::endl;
        if (isPlayerTurn)
        {
            playerExtraScore += 5;
        }
        else
        {
            cpuExtraScore += 5;
        }
    };

    if (tableCards.empty() && !isSimulation)
    {
        tableCards.push_back(card);
        return;
    }

    if (card.getRank() == Rank::Jack)
    {
        bool collectedAny = false;
        for (auto it = tableCards.begin(); it != tableCards.end();)
        {
            if (it->getRank() != Rank::Queen && it->getRank() != Rank::King)
            {
                collectCard(*it);
                collectedAny = true;
                if (!isSimulation)
                {
                    it = tableCards.erase(it);
                }
                else
                {
                    ++it;
                }
            }
            else
            {
                ++it;
            }
        }

        if (!collectedAny)
        {
            if (!isSimulation)
            {
                std::cout << "No cards collected." << std::endl;
                tableCards.push_back(card);
            }
        }
        else
        {
            if (!isSimulation)
            {
                collectCard(card);
            }
        }
    }
    else if (card.getRank() == Rank::Queen || card.getRank() == Rank::King)
    {
        std::vector<Card> matches;
        for (const auto &c : tableCards)
        {
            if (c.getRank() == card.getRank())
            {
                matches.push_back(c);
            }
        }

        if (!matches.empty())
        {
            Card chosen = matches.front();
            auto clubIt = std::find_if(matches.begin(), matches.end(),
                                       [](const Card &c)
                                       { return c.getSuit() == Suit::Clubs; });

            if (clubIt != matches.end())
            {
                chosen = *clubIt;
            }

            collectCard(chosen);
            if (!isSimulation)
            {
                tableCards.erase(std::remove(tableCards.begin(), tableCards.end(), chosen), tableCards.end());
                collectCard(card);
            }
        }
        else
        {
            if (!isSimulation)
            {
                tableCards.push_back(card);
            }
        }

        if (tableCards.empty() && !isSimulation)
        {
            awardSoor();
        }
    }
    else
    {
        int target = 11 - static_cast<int>(card.getRank());
        std::vector<Card> combination;
        std::vector<std::vector<Card>> result;

        findCombinationSum(tableCards, target, 0, combination, result);

        if (result.empty())
        {
            // std::cout << "No combination found." << std::endl;
            if (!isSimulation)
            {
                tableCards.push_back(card);
            }
            return;
        }

        size_t choice = 0;
        if (result.size() > 1)
        {
            if (isPlayerTurn)
            {
                std::cout << "Multiple combinations found. Choose one (1-" << result.size() << "): ";
                std::cin >> choice;
            }
            else
            {
                int maxScore = -1;
                for (size_t i = 0; i < result.size(); ++i)
                {
                    int score = evaluateCapturedCards(card, result[i], tableCards);
                    if (score > maxScore)
                    {
                        maxScore = score;
                        choice = i;
                    }
                }
            }

            if (choice >= 1 && choice <= result.size())
            {
                --choice;
            }
        }

        for (const auto &c : result[choice])
        {
            auto it = std::find_if(tableCards.begin(), tableCards.end(),
                                   [&](const Card &tc)
                                   { return tc.getRank() == c.getRank(); });

            if (it != tableCards.end())
            {
                collectCard(*it);
                if (!isSimulation)
                {
                    tableCards.erase(it);
                }
            }
        }

        if (!isSimulation)
        {
            collectCard(card);
        }

        if (tableCards.empty() && !isSimulation)
        {
            awardSoor();
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

    for (const auto &pair : playerCollectionCards)
    {
        for (const auto &card : pair.second)
        {
            playerScore += cardScore(card);
        }
    }

    cpuScore = 20 - playerScore + cpuExtraScore;
    playerScore += playerExtraScore;

    std::cout << "Player Score: " << playerScore << std::endl;
    std::cout << "CPU Score: " << cpuScore << std::endl;
}

int Game::cardScore(const Card &card)
{
    int score = 0;
    Rank rank = card.getRank();
    if (rank == Rank::Ace)
    {
        score += 1;
    }
    else if (rank == Rank::Two && card.getSuit() == Suit::Clubs)
    {
        score += 2;
    }
    else if (rank == Rank::Ten && card.getSuit() == Suit::Diamonds)
    {
        score += 3;
    }
    else if (rank == Rank::Jack)
    {
        score += 1;
    }
    return score;
}

void Game::printPlayerCollection() const
{
    std::cout << "Player Collection: " << std::endl;
    for (const auto &pair : playerCollectionCards)
    {
        std::cout << "Suit: " << static_cast<int>(pair.first) << " Cards: ";
        for (const auto &card : pair.second)
        {
            std::cout << card.toString() << " ";
        }
        std::cout << std::endl;
    }
}

void Game::printCpuCollection() const
{
    std::cout << "CPU Collection: " << std::endl;
    for (const auto &pair : cpuCollectionCards)
    {
        std::cout << "Suit: " << static_cast<int>(pair.first) << " Cards: ";
        for (const auto &card : pair.second)
        {
            std::cout << card.toString() << " ";
        }
        std::cout << std::endl;
    }
}

int Game::evaluateCapturedCards(const Card &dropCard, const std::vector<Card> &cards, const std::vector<Card> &tableCards)
{
    int score = 0;
    int risk = 0;
    for (const auto &card : cards)
    {
        if (card.getRank() == Rank::Ace)
        {
            score += 2;
        }
        else if (card.getRank() == Rank::Two && card.getSuit() == Suit::Clubs)
        {
            score += 2;
        }
        else if (card.getRank() == Rank::Ten && card.getSuit() == Suit::Diamonds)
        {
            score += 3;
        }

        if (card.getSuit() == Suit::Clubs)
        {
            score += 1;
        }
    }

    if (dropCard.getRank() == Rank::Jack && tableCards.size() < 1)
    {
        risk += 3;
    }

    if (dropCard.getRank() == Rank::Ace ||
        (dropCard.getRank() == Rank::Two && dropCard.getSuit() == Suit::Clubs) ||
        (dropCard.getRank() == Rank::Ten && dropCard.getSuit() == Suit::Diamonds) ||
        dropCard.getSuit() == Suit::Clubs)
    {
        int sum = static_cast<int>(dropCard.getRank());
        for (const auto &card : tableCards)
        {
            if (card.getRank() != Rank::Queen && card.getRank() != Rank::King)
            {
                sum += static_cast<int>(card.getRank());
            }
        }

        if (sum < 11)
        {
            risk += 2;
        }
    }

    return score - risk;
}

void Game::printCards(const std::vector<Card> &cards) const
{
    std::vector<std::vector<std::string>> cardLines;

    for (const Card &card : cards)
    {
        std::string rankStr = card.rankToString();
        std::string suitStr = card.suitToString();

        cardLines.push_back({"┌─────────┐",
                             "│" + rankStr + "        │",
                             "│         │",
                             "│    " + suitStr + "    │",
                             "│         │",
                             "│        " + rankStr + "│",
                             "└─────────┘"});
    }

    for (size_t line = 0; line < cardLines[0].size(); ++line)
    {
        for (const auto &card : cardLines)
        {
            std::cout << card[line] << " ";
        }
        std::cout << std::endl;
    }
}