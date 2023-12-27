#include <iostream>
#include <chrono>
#include <fstream>
#include <sstream>
#include <vector>

#define endl '\n'

using namespace std;

const int MAX_LEN = 1003;
const int MAX_GROUPS = 10;
const int MAX_CONTINUOUS = 20;

vector<string> springs;
vector<vector<int>> allGroups;

int64_t dp[5 * MAX_LEN][5 * MAX_GROUPS][5 * MAX_CONTINUOUS];

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

void reset_dp(int springSize, int factor)
{
    for (int i = 0; i <= factor * springSize; ++i)
    {
        for (int j = 0; j < factor * MAX_GROUPS; ++j)
        {
            for (int k = 0; k < MAX_CONTINUOUS; ++k)
            {
                dp[i][j][k] = -1;
            }
        }
    }
}

bool flag = false;

int64_t calculate_dp(int index, string &spring, int currentGroup, vector<int> &groups, int continuosCounter)
{
    if (dp[index][currentGroup][continuosCounter] != -1)
    {
        return dp[index][currentGroup][continuosCounter];
    }

    if (index == (int)spring.size())
    {
        return (currentGroup == (int)groups.size());
    }

    if (spring[index] == '.')
    {
        if (continuosCounter > 0 and currentGroup < (int)groups.size())
        {
            if (continuosCounter != groups[currentGroup])
            {
                return 0;
            }

            ++currentGroup;
            continuosCounter = 0;
        }

        return dp[index][currentGroup][continuosCounter] = calculate_dp(index + 1, spring, currentGroup, groups, continuosCounter);
    }

    if (spring[index] == '#')
    {
        if (currentGroup == (int)groups.size() or continuosCounter + 1 > groups[currentGroup])
        {
            return 0;
        }

        return dp[index][currentGroup][continuosCounter] = calculate_dp(index + 1, spring, currentGroup, groups, continuosCounter + 1);
    }

    // Handle case with unknown
    int64_t &result = dp[index][currentGroup][continuosCounter];
    result = 0;

    // Cases with operational
    if (continuosCounter > 0 and currentGroup < (int)groups.size() and continuosCounter == groups[currentGroup])
    {
        spring[index] = '.';
        result += calculate_dp(index + 1, spring, currentGroup + 1, groups, 0);
        spring[index] = '?';
    }
    else if (continuosCounter == 0)
    {
        spring[index] = '.';
        result += calculate_dp(index + 1, spring, currentGroup, groups, 0);
        spring[index] = '?';
    }

    // Cases with damaged
    if (currentGroup < (int)groups.size() and continuosCounter + 1 <= groups[currentGroup])
    {
        spring[index] = '#';
        result += calculate_dp(index + 1, spring, currentGroup, groups, continuosCounter + 1);
        spring[index] = '?';
    }

    return result;
}

int64_t find_arrangements(int factor)
{
    int64_t result = 0ll;

    for (int i = 0; i < (int)springs.size(); ++i)
    {
        reset_dp((int)springs[i].size(), factor);
        result += calculate_dp(0, springs[i], 0, allGroups[i], 0);
    }

    return result;
}

int64_t part1()
{
    return find_arrangements(1);
}

int64_t part2()
{
    for (int i = 0; i < (int)springs.size(); ++i)
    {
        springs[i].pop_back();
        string originalSpring = springs[i];

        for (int j = 0; j < 4; ++j)
        {
            springs[i] += "?" + originalSpring;
        }
        springs[i] += ".";

        vector<int> originalGroups = allGroups[i];
        for (int j = 0; j < 4; ++j)
        {
            for (int value : originalGroups)
            {
                allGroups[i].push_back(value);
            }
        }
    }

    return find_arrangements(5);
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ifstream fin("input.txt");

    string row;
    while (getline(fin, row))
    {
        stringstream sin(row);

        string currentSpring, currentGroups;
        sin >> currentSpring >> currentGroups;

        currentSpring += ".";
        springs.push_back(currentSpring);

        vector<string> damagedValuesInStrings = split_string(currentGroups, ',');

        vector<int> damagedValuesInNums;
        for (string &value : damagedValuesInStrings)
        {
            damagedValuesInNums.push_back(stoi(value));
        }

        allGroups.push_back(damagedValuesInNums);
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