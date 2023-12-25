#include <iostream>
#include <chrono>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <numeric>

#define endl '\n'

using namespace std;

const int MAX_NODES = 803;

const int START = 0;
const int END = 1;

unordered_map<string, int> nodeNameToId;
int availableId;

string instructions;
pair<int, int> nodes[MAX_NODES];

vector<int> startingNodes;
bool isEndingNode[MAX_NODES];

int get_id(string &nodeName)
{
    if (nodeNameToId.count(nodeName) == 0)
    {
        nodeNameToId[nodeName] = availableId++;
    }

    return nodeNameToId[nodeName];
}

int find_steps_to_end(int startNode)
{
    int currentNode = startNode;
    int instructionIndex = 0;

    int steps = 0;

    while (!isEndingNode[currentNode])
    {
        ++steps;
        if (instructionIndex == (int)instructions.size())
        {
            instructionIndex = 0;
        }

        char currentInstruction = instructions[instructionIndex];
        if (currentInstruction == 'L')
        {
            currentNode = nodes[currentNode].first;
        }
        else if (currentInstruction == 'R')
        {
            currentNode = nodes[currentNode].second;
        }

        ++instructionIndex;
    }

    return steps;
}

int part1()
{
    return find_steps_to_end(START);
}

int64_t part2()
{
    int64_t result = 1ll;

    for (int startNode : startingNodes)
    {
        int64_t steps = find_steps_to_end(startNode);

        result = 1ll * result * steps / gcd(result, steps);
    }

    return result;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ifstream fin("input.txt");

    getline(fin, instructions);

    string row;
    getline(fin, row); // empty line in input

    nodeNameToId["AAA"] = START;
    nodeNameToId["ZZZ"] = END;
    availableId = 2;

    while (getline(fin, row))
    {
        stringstream sin(row);

        string currentNode, leftNode, rightNode, dummy;
        sin >> currentNode >> dummy >> leftNode >> rightNode;

        leftNode = leftNode.substr(1);
        leftNode.pop_back();

        rightNode.pop_back();

        nodes[get_id(currentNode)] = {get_id(leftNode), get_id(rightNode)};

        if (currentNode.back() == 'A')
        {
            startingNodes.push_back(get_id(currentNode));
        }

        if (currentNode.back() == 'Z')
        {
            isEndingNode[get_id(currentNode)] = true;
        }
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