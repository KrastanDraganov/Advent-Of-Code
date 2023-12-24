#include <iostream>
#include <chrono>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <sstream>

#define endl '\n'

using namespace std;

const int MAX_CARDS = 225;

int cardsCopies[MAX_CARDS];

int allCards;
unordered_set<int> winningNumbers[MAX_CARDS];
vector<int> myNumbers[MAX_CARDS];

vector<string> split_string(string &s, char delimiter)
{
    istringstream stringStream(s);
    string currentString;

    vector<string> result;

    while (getline(stringStream, currentString, delimiter))
    {
        result.push_back(currentString);
    }

    return result;
}

void get_info_from_input(vector<string> &input)
{
    allCards = (int)input.size();

    for (int i = 0; i < allCards; ++i)
    {
        vector<string> info = split_string(input[i], ':');
        vector<string> numbers = split_string(info[1], '|');

        vector<string> leftNumbers = split_string(numbers[0], ' ');
        vector<string> rightNumbers = split_string(numbers[1], ' ');

        for (string &num : leftNumbers)
        {
            if (num != "")
            {
                winningNumbers[i].insert(stoi(num));
            }
        }

        for (string &num : rightNumbers)
        {
            if (num != "")
            {
                myNumbers[i].push_back(stoi(num));
            }
        }
    }
}

int part1(vector<string> &input)
{
    get_info_from_input(input);

    for (int i = 0; i < allCards; ++i)
    {
        cardsCopies[i] = 1;
    }

    int result = 0;

    for (int i = 0; i < allCards; ++i)
    {
        int currentPoints = 0;
        int winningCounter = 0;

        for (int num : myNumbers[i])
        {
            if (winningNumbers[i].count(num) == 0)
            {
                continue;
            }

            ++winningCounter;

            if (currentPoints == 0)
            {
                currentPoints = 1;
            }
            else
            {
                currentPoints *= 2;
            }
        }

        result += currentPoints;

        for (int j = i + 1; j < min(allCards, i + winningCounter + 1); ++j)
        {
            cardsCopies[j] += cardsCopies[i];
        }
    }

    return result;
}

int part2()
{
    int result = 0;

    for (int i = 0; i < allCards; ++i)
    {
        result += cardsCopies[i];
    }

    return result;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ifstream fin("input.txt");

    vector<string> input;

    string row;
    while (getline(fin, row))
    {
        input.push_back(row);
    }

    fin.close();

    auto t_begin = chrono::high_resolution_clock::now();
    cout << "Result, part 1: " << part1(input) << endl;
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