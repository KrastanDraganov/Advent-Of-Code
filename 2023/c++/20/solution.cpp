#include <iostream>
#include <chrono>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>

#define endl '\n'

using namespace std;

const int MAX_MODULES = 60;
const int BUTTON_PRESSES = 1000;

const int FINAL_MODULE = -2;
const int BUTTON = -1;
const int BROADCASTER = 0;

const int LOW_PULSE = 0;
const int HIGH_PULSE = 1;

struct PulseInfo
{
    int pulse, receiver, sender;

    PulseInfo(int _pulse, int _receiver, int _sender) : pulse(_pulse), receiver(_receiver), sender(_sender) {}
};

unordered_map<string, int> moduleNameToId;
int availableIdNumber = 1;

char idToModuleType[MAX_MODULES];
int64_t currentConjuctionModuleMemory[MAX_MODULES];
int64_t defaultConjuctionModuleMemory[MAX_MODULES];
bool flipModuleState[MAX_MODULES];

vector<int> graph[MAX_MODULES];

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

void assign_ids(vector<string> &input)
{
    moduleNameToId["roadcaster"] = BROADCASTER;
    idToModuleType[BROADCASTER] = 'b';

    for (string &instruction : input)
    {
        vector<string> parts = split_string(instruction, ' ');

        char moduleType = parts[0][0];
        string currentName = parts[0].substr(1);

        if (moduleNameToId.count(currentName) > 0)
        {
            continue;
        }

        moduleNameToId[currentName] = availableIdNumber;
        idToModuleType[availableIdNumber] = moduleType;

        ++availableIdNumber;
    }
}

void make_graph(vector<string> &input)
{
    for (string &instruction : input)
    {
        vector<string> parts = split_string(instruction, ' ');

        char moduleType = parts[0][0];
        string senderName = parts[0].substr(1);

        int senderId = moduleNameToId[senderName];

        for (int i = 2; i < (int)parts.size(); ++i)
        {
            string receiverName = parts[i];
            if (receiverName.back() == ',')
            {
                receiverName.pop_back();
            }

            if (moduleNameToId.count(receiverName) == 0)
            {
                moduleNameToId[receiverName] = FINAL_MODULE;
            }

            int receiverId = moduleNameToId[receiverName];

            graph[senderId].push_back(receiverId);

            if (idToModuleType[receiverId] == '&')
            {
                currentConjuctionModuleMemory[receiverId] |= (1ll << senderId);
                defaultConjuctionModuleMemory[receiverId] |= (1ll << senderId);
            }
        }
    }
}

int64_t part1(vector<string> &input)
{
    assign_ids(input);
    make_graph(input);

    int lowPulsesCounter = 0;
    int highPulsesCounter = 0;

    for (int steps = 0; steps < BUTTON_PRESSES; ++steps)
    {
        queue<PulseInfo> bfs;
        bfs.push(PulseInfo(LOW_PULSE, BROADCASTER, BUTTON));

        while (!bfs.empty())
        {
            auto [currentPulse, currentModuleId, previousModuleId] = bfs.front();
            bfs.pop();

            if (currentPulse == LOW_PULSE)
            {
                ++lowPulsesCounter;
            }
            else if (currentPulse == HIGH_PULSE)
            {
                ++highPulsesCounter;
            }

            if (currentModuleId == FINAL_MODULE)
            {
                continue;
            }

            char currentModuleType = idToModuleType[currentModuleId];
            int nextPulse = -1;

            if (currentModuleType == '%')
            {
                if (currentPulse == HIGH_PULSE)
                {
                    continue;
                }

                flipModuleState[currentModuleId] = !flipModuleState[currentModuleId];
                nextPulse = flipModuleState[currentModuleId];
            }
            else if (currentModuleType == '&')
            {
                if (currentPulse == HIGH_PULSE and (currentConjuctionModuleMemory[currentModuleId] & (1ll << previousModuleId)))
                {
                    currentConjuctionModuleMemory[currentModuleId] ^= (1ll << previousModuleId);
                }
                else if (currentPulse == LOW_PULSE)
                {
                    currentConjuctionModuleMemory[currentModuleId] |= (1ll << previousModuleId);
                }

                nextPulse = currentConjuctionModuleMemory[currentModuleId] == 0ll ? LOW_PULSE : HIGH_PULSE;
            }
            else if (currentModuleType == 'b')
            {
                nextPulse = LOW_PULSE;
            }

            for (int nextModuleId : graph[currentModuleId])
            {
                bfs.push(PulseInfo(nextPulse, nextModuleId, currentModuleId));
            }
        }
    }

    return 1ll * lowPulsesCounter * highPulsesCounter;
}

int64_t part2()
{
    for (int i = 0; i < MAX_MODULES; ++i)
    {
        currentConjuctionModuleMemory[i] = defaultConjuctionModuleMemory[i];
        flipModuleState[i] = false;
    }

    unordered_set<int> targetIds;
    targetIds.insert(moduleNameToId["kv"]);
    targetIds.insert(moduleNameToId["jg"]);
    targetIds.insert(moduleNameToId["rz"]);
    targetIds.insert(moduleNameToId["mr"]);

    int buttonPresses = 0;
    int64_t result = 1ll;

    while (!targetIds.empty())
    {
        queue<PulseInfo> bfs;

        bfs.push(PulseInfo(LOW_PULSE, BROADCASTER, BUTTON));
        ++buttonPresses;

        while (!bfs.empty() and !targetIds.empty())
        {
            auto [currentPulse, currentModuleId, previousModuleId] = bfs.front();
            bfs.pop();

            if (currentModuleId == FINAL_MODULE)
            {
                continue;
            }

            char currentModuleType = idToModuleType[currentModuleId];
            int nextPulse = -1;

            if (currentModuleType == '%')
            {
                if (currentPulse == HIGH_PULSE)
                {
                    continue;
                }

                flipModuleState[currentModuleId] = !flipModuleState[currentModuleId];
                nextPulse = flipModuleState[currentModuleId];
            }
            else if (currentModuleType == '&')
            {
                if (previousModuleId >= 0)
                {
                    if (currentPulse == HIGH_PULSE and (currentConjuctionModuleMemory[currentModuleId] & (1ll << previousModuleId)))
                    {
                        currentConjuctionModuleMemory[currentModuleId] ^= (1ll << previousModuleId);
                    }
                    else if (currentPulse == LOW_PULSE)
                    {
                        currentConjuctionModuleMemory[currentModuleId] |= (1ll << previousModuleId);
                    }
                }

                nextPulse = currentConjuctionModuleMemory[currentModuleId] == 0ll ? LOW_PULSE : HIGH_PULSE;
            }
            else if (currentModuleType == 'b')
            {
                nextPulse = LOW_PULSE;
            }

            if (targetIds.count(currentModuleId) and nextPulse == HIGH_PULSE)
            {
                targetIds.erase(currentModuleId);
                result *= 1ll * buttonPresses;
            }

            for (int nextModuleId : graph[currentModuleId])
            {
                bfs.push(PulseInfo(nextPulse, nextModuleId, currentModuleId));
            }
        }
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