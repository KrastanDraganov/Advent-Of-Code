#include <iostream>
#include <chrono>
#include <fstream>
#include <sstream>
#include <vector>

#define endl '\n'

using namespace std;

vector<vector<int>> sequences;

int find_extrapolated_value_back(vector<int> &nums)
{
    vector<int> currentSeq = nums;
    vector<int> lastValues;

    bool allZero;

    do
    {
        allZero = true;

        int seqSize = (int)currentSeq.size();
        vector<int> nextSeq(seqSize - 1);

        lastValues.push_back(currentSeq.back());

        for (int i = 0; i < seqSize - 1; ++i)
        {
            nextSeq[i] = currentSeq[i + 1] - currentSeq[i];
            allZero &= (nextSeq[i] == 0);
        }

        currentSeq = nextSeq;
    } while (!allZero);

    int result = 0;
    while (!lastValues.empty())
    {
        result += lastValues.back();
        lastValues.pop_back();
    }

    return result;
}

int find_extrapolated_value_front(vector<int> &nums)
{
    vector<int> currentSeq = nums;
    vector<int> firstValues;

    bool allZero;

    do
    {
        allZero = true;

        int seqSize = (int)currentSeq.size();
        vector<int> nextSeq(seqSize - 1);

        firstValues.push_back(currentSeq[0]);

        for (int i = 0; i < seqSize - 1; ++i)
        {
            nextSeq[i] = currentSeq[i + 1] - currentSeq[i];
            allZero &= (nextSeq[i] == 0);
        }

        currentSeq = nextSeq;
    } while (!allZero);

    int result = 0;
    while (!firstValues.empty())
    {
        result = firstValues.back() - result;
        firstValues.pop_back();
    }

    return result;
}

int part1()
{
    int result = 0;

    for (auto &seq : sequences)
    {
        result += find_extrapolated_value_back(seq);
    }

    return result;
}

int part2()
{
    int result = 0;

    for (auto &seq : sequences)
    {
        result += find_extrapolated_value_front(seq);
    }

    return result;
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

        int num;
        vector<int> currentSeq;

        while (sin >> num)
        {
            currentSeq.push_back(num);
        }

        sequences.push_back(currentSeq);
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