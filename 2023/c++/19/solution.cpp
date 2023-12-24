#include <iostream>
#include <chrono>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <array>

#define endl '\n'

using namespace std;

const int INF = 1e9 + 3;

const int MAX_WORKFLOWS = 600;

const int REJECTED = 0;
const int ACCEPTED = 1;

struct Requirement
{
    int category;
    int minValue, maxValue;
    int nextWorkflow;

    Requirement(char _category, int _minValue, int _maxValue, int _nextWorkflow) : category(_category), minValue(_minValue), maxValue(_maxValue), nextWorkflow(_nextWorkflow) {}
};

unordered_map<char, int> categoryToIndex;
unordered_map<string, int> workflowNameToId;
int availableIdNumber = 2;

vector<Requirement> requirements[MAX_WORKFLOWS];
vector<array<int, 4>> partRatings;

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

void assign_id(string &name)
{
    if (workflowNameToId.count(name) == 0)
    {
        workflowNameToId[name] = availableIdNumber++;
    }
}

void get_info_from_input(vector<string> &input)
{
    categoryToIndex['x'] = 0;
    categoryToIndex['m'] = 1;
    categoryToIndex['a'] = 2;
    categoryToIndex['s'] = 3;

    workflowNameToId["R"] = REJECTED;
    workflowNameToId["A"] = ACCEPTED;

    int line = 0;

    while (input[line] != "")
    {
        vector<string> workflowInfo = split_string(input[line], '{');

        assign_id(workflowInfo[0]);
        int workflowId = workflowNameToId[workflowInfo[0]];

        vector<string> currentReqs = split_string(workflowInfo[1], ',');

        for (string &req : currentReqs)
        {
            vector<string> parts = split_string(req, ':');

            bool isLastRequirement = (int)parts.size() == 1;
            if (isLastRequirement)
            {
                parts[0].pop_back();
                assign_id(parts[0]);

                requirements[workflowId].push_back(Requirement(0, -INF, INF, workflowNameToId[parts[0]]));
                break;
            }

            assign_id(parts[1]);

            int minValue = -1, maxValue = -1;
            if (parts[0][1] == '>')
            {
                minValue = stoi(parts[0].substr(2)) + 1;
                maxValue = INF;
            }
            else if (parts[0][1] == '<')
            {
                minValue = -INF;
                maxValue = stoi(parts[0].substr(2)) - 1;
            }

            requirements[workflowId].push_back(Requirement(categoryToIndex[parts[0][0]], minValue, maxValue, workflowNameToId[parts[1]]));
        }

        ++line;
    }

    ++line;

    while (line < (int)input.size())
    {
        vector<string> parts = split_string(input[line], ',');

        partRatings.push_back({});
        for (int i = 0; i < 4; ++i)
        {
            vector<string> equation = split_string(parts[i], '=');
            partRatings.back()[i] = stoi(equation[1]);
        }

        ++line;
    }
}

bool isAccepted(array<int, 4> &partRating)
{
    int currentWorkflow = workflowNameToId["in"];
    int index = 0;

    while (currentWorkflow != ACCEPTED and currentWorkflow != REJECTED)
    {
        auto &[category, minValue, maxValue, nextWorkflow] = requirements[currentWorkflow][index];

        if (partRating[category] < minValue or partRating[category] > maxValue)
        {
            ++index;
            continue;
        }

        currentWorkflow = nextWorkflow;
        index = 0;
    }

    return currentWorkflow == ACCEPTED;
}

uint64_t combinations(int currentWorkflow, int currentIndex, array<int, 4> &minValues, array<int, 4> &maxValues)
{
    if (currentWorkflow == REJECTED)
    {
        return 0ull;
    }

    if (currentWorkflow == ACCEPTED)
    {
        uint64_t result = 1ull;

        for (int i = 0; i < 4; ++i)
        {
            if (minValues[i] >= maxValues[i])
            {
                return 0ull;
            }

            result *= 1ull * (maxValues[i] - minValues[i] + 1);
        }

        return result;
    }

    if (currentIndex == (int)requirements[currentWorkflow].size())
    {
        return 0ull;
    }

    uint64_t result = 0ull;
    auto &[category, newMinValue, newMaxValue, nextWorkflow] = requirements[currentWorkflow][currentIndex];

    int oldMinValue = minValues[category];
    int oldMaxValue = maxValues[category];

    minValues[category] = max(minValues[category], newMinValue);
    maxValues[category] = min(maxValues[category], newMaxValue);

    if (minValues[category] <= maxValues[category])
    {
        result += combinations(nextWorkflow, 0, minValues, maxValues);
    }

    minValues[category] = oldMinValue;
    maxValues[category] = newMinValue - 1;

    if (minValues[category] <= maxValues[category])
    {
        result += combinations(currentWorkflow, currentIndex + 1, minValues, maxValues);
    }

    minValues[category] = newMaxValue + 1;
    maxValues[category] = oldMaxValue;

    if (minValues[category] <= maxValues[category])
    {
        result += combinations(currentWorkflow, currentIndex + 1, minValues, maxValues);
    }

    minValues[category] = oldMinValue;
    maxValues[category] = oldMaxValue;

    return result;
}

int part1(vector<string> &input)
{
    get_info_from_input(input);

    int result = 0;
    for (auto &part : partRatings)
    {
        if (!isAccepted(part))
        {
            continue;
        }

        for (int i = 0; i < 4; ++i)
        {
            result += part[i];
        }
    }

    return result;
}

uint64_t part2()
{
    array<int, 4> minValues{1, 1, 1, 1};
    array<int, 4> maxValues{4000, 4000, 4000, 4000};

    return combinations(workflowNameToId["in"], 0, minValues, maxValues);
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