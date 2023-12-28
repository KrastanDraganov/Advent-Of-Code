#include <iostream>
#include <chrono>
#include <fstream>
#include <sstream>
#include <vector>

#define endl '\n'

using namespace std;

const int BASE = 17;
const int MOD = 256;

struct ListNode
{
    string label;
    int focal;
    ListNode *next;

    ListNode() : label(""), focal(-1), next(nullptr) {}
    ListNode(string _label, int _focal) : label(_label), focal(_focal), next(nullptr) {}
};

vector<string> steps;
ListNode *boxes[MOD];

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

pair<ListNode *, ListNode *> find_label(int boxIndex, string &tagetLabel)
{
    ListNode *prevNode = nullptr;
    ListNode *currentNode = boxes[boxIndex];

    while (currentNode != nullptr and currentNode->label != tagetLabel)
    {
        prevNode = currentNode;
        currentNode = currentNode->next;
    }

    return {prevNode, currentNode};
}

void remove_lens(int boxIndex, string &targetLabel)
{
    auto [prevNode, currentNode] = find_label(boxIndex, targetLabel);

    if (currentNode == nullptr)
    {
        return;
    }

    prevNode->next = currentNode->next;
}

void add_lens(int boxIndex, string &newLabel, int newFocal)
{
    auto [prevNode, currentNode] = find_label(boxIndex, newLabel);

    if (currentNode == nullptr)
    {
        prevNode->next = new ListNode(newLabel, newFocal);
    }
    else
    {
        currentNode->focal = newFocal;
    }
}

int calculate_hash(string &s)
{
    int result = 0;

    for (char c : s)
    {
        result += c;
        result *= BASE;
        result %= MOD;
    }

    return result;
}

int part1()
{
    int result = 0;

    for (string &step : steps)
    {
        result += calculate_hash(step);
    }

    return result;
}

int part2()
{
    for (int i = 0; i < MOD; ++i)
    {
        boxes[i] = new ListNode();
    }

    for (string &step : steps)
    {
        string label = "";
        int i = 0;

        while (step[i] != '=' and step[i] != '-')
        {
            label += step[i];
            ++i;
        }

        int currentBox = calculate_hash(label);

        if (step[i] == '-')
        {
            remove_lens(currentBox, label);
        }
        else
        {
            int focal = 0;

            ++i;
            while (i < (int)step.size())
            {
                focal = 10 * focal + step[i] - '0';
                ++i;
            }

            add_lens(currentBox, label, focal);
        }
    }

    int result = 0;
    for (int i = 0; i < MOD; ++i)
    {
        ListNode *currentNode = boxes[i]->next;

        for (int slot = 1; currentNode != nullptr; ++slot)
        {
            result += (i + 1) * slot * currentNode->focal;
            currentNode = currentNode->next;
        }
    }

    return result;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ifstream fin("input.txt");

    string row;
    getline(fin, row);

    steps = split_string(row, ',');

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