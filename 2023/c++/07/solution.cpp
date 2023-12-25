#include <iostream>
#include <chrono>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <array>

#define endl '\n'

using namespace std;

const int CARDS_IN_HAND = 5;

unordered_map<char, int> cardStrength;

struct Hand
{
    string cards;
    int type;
    int bid;

    Hand(string _cards, int _bid) : cards(_cards), bid(_bid), type(-1)
    {
    }

    void calculate_type_without_joker()
    {
        string cardsCopy = cards;
        sort(cardsCopy.begin(), cardsCopy.end());

        array<int, 6> counter;
        counter.fill(0);

        int consecutive = 0;

        for (int i = 0; i < (int)cardsCopy.size(); ++i)
        {
            if (i > 0 and cardsCopy[i] == cardsCopy[i - 1])
            {
                ++consecutive;
            }
            else
            {
                ++counter[consecutive];
                consecutive = 1;
            }
        }
        ++counter[consecutive];

        if (counter[5] == 1)
        {
            type = 7;
        }
        else if (counter[4] == 1)
        {
            type = 6;
        }
        else if (counter[3] == 1)
        {
            if (counter[2] == 1)
            {
                type = 5;
            }
            else
            {
                type = 4;
            }
        }
        else if (counter[2] == 2)
        {
            type = 3;
        }
        else if (counter[2] == 1)
        {
            type = 2;
        }
        else
        {
            type = 1;
        }
    }

    void calculate_type_with_joker()
    {
        string cardsCopy = cards;
        sort(cardsCopy.begin(), cardsCopy.end());

        array<int, 6> counter;
        counter.fill(0);

        int consecutive = 0;
        int jokers = 0;

        for (int i = 0; i < (int)cardsCopy.size(); ++i)
        {
            if (cardsCopy[i] == 'J')
            {
                ++jokers;
                ++counter[consecutive];
                consecutive = 0;
            }
            else if (i > 0 and cardsCopy[i] == cardsCopy[i - 1])
            {
                ++consecutive;
            }
            else
            {
                ++counter[consecutive];
                consecutive = 1;
            }
        }
        ++counter[consecutive];

        if (counter[5] == 1)
        {
            type = 7;
        }
        else if (counter[4] == 1)
        {
            type = jokers == 1 ? 7 : 6;
        }
        else if (counter[3] == 1)
        {
            if (counter[2] == 1)
            {
                type = 5;
            }
            else if (jokers == 2)
            {
                type = 7;
            }
            else if (jokers == 1)
            {
                type = 6;
            }
            else
            {
                type = 4;
            }
        }
        else if (counter[2] == 2)
        {
            if (jokers == 1)
            {
                type = 5;
            }
            else
            {
                type = 3;
            }
        }
        else if (counter[2] == 1)
        {
            if (jokers == 3)
            {
                type = 7;
            }
            else if (jokers == 2)
            {
                type = 6;
            }
            else if (jokers == 1)
            {
                type = 4;
            }
            else
            {
                type = 2;
            }
        }
        else if (counter[1] > 0)
        {
            if (jokers == 4)
            {
                type = 7;
            }
            else if (jokers == 3)
            {
                type = 6;
            }
            else if (jokers == 2)
            {
                type = 4;
            }
            else if (jokers == 1)
            {
                type = 2;
            }
            else
            {
                type = 1;
            }
        }
        else
        {
            type = 7;
        }
    }

    bool
    operator<(const Hand &comp) const
    {
        if (type != comp.type)
        {
            return type < comp.type;
        }

        for (int i = 0; i < CARDS_IN_HAND; ++i)
        {
            if (cards[i] != comp.cards[i])
            {
                return cardStrength[cards[i]] < cardStrength[comp.cards[i]];
            }
        }

        return false;
    }
};

vector<Hand> hands;

void assign_card_strengths(vector<char> &cardLabels)
{
    cardStrength.clear();
    for (int i = 0; i < (int)cardLabels.size(); ++i)
    {
        cardStrength[cardLabels[i]] = i;
    }
}

int calculate_winnings()
{
    sort(hands.begin(), hands.end());

    int result = 0;
    for (int i = 0; i < (int)hands.size(); ++i)
    {
        result += (i + 1) * hands[i].bid;
    }

    return result;
}

int part1()
{
    vector<char> cardLabels = {'2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A'};
    assign_card_strengths(cardLabels);

    for (int i = 0; i < (int)hands.size(); ++i)
    {
        hands[i].calculate_type_without_joker();
    }

    return calculate_winnings();
}

int part2()
{
    vector<char> cardLabels = {'J', '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'Q', 'K', 'A'};
    assign_card_strengths(cardLabels);

    for (int i = 0; i < (int)hands.size(); ++i)
    {
        hands[i].calculate_type_with_joker();
    }

    return calculate_winnings();
}

int main()
{
    // ios::sync_with_stdio(false);
    // cin.tie(nullptr);

    ifstream fin("input.txt");

    string row;
    while (getline(fin, row))
    {
        stringstream sin(row);

        string cards;
        int bid;
        sin >> cards >> bid;

        hands.push_back(Hand(cards, bid));
    }

    fin.close();

    auto t_begin = chrono::high_resolution_clock::now();
    cout << "Result, part 1: " << part1() << endl;
    auto t_end = chrono::high_resolution_clock::now();

    auto part1Time = chrono::duration<double, std::milli>(t_end - t_begin).count();

    t_begin = chrono::high_resolution_clock::now();
    cout << "Result, part 2: " << part2() << endl;
    t_end = chrono::high_resolution_clock::now();

    auto part2Time = chrono::duration<double, std::milli>(t_end - t_begin).count();

    cout << "Completed part 1 in: " << part1Time << " ms" << endl;
    cout << "Completed part 2 in: " << part2Time << " ms" << endl;

    return 0;
}