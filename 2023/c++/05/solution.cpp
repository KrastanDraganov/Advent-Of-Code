#include <iostream>
#include <chrono>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>

#define endl '\n'

using namespace std;

const int64_t INF = 1e18 + 3;

const int MAX_MAPS = 7;

struct MapInfo
{
    int64_t destination, source, range;

    MapInfo(int64_t _destination, int64_t _source, int64_t _range) : destination(_destination), source(_source), range(_range) {}
};

vector<int64_t> seeds;
vector<pair<int64_t, int64_t>> seedRanges;

int allMaps;
vector<MapInfo> maps[MAX_MAPS];

void get_info_from_input(vector<string> &input)
{
    stringstream sinSeeds(input[0]);

    string dummy;
    sinSeeds >> dummy;

    int64_t seedNumber;
    while (sinSeeds >> seedNumber)
    {
        seeds.push_back(seedNumber);
    }

    int line = 2;
    for (int i = 0, line = 2; line < (int)input.size(); ++i, ++line)
    {
        ++line;
        allMaps = i + 1;

        while (line < (int)input.size() and input[line] != "")
        {
            stringstream sinInfo(input[line]);

            int64_t destination, source, range;
            sinInfo >> destination >> source >> range;

            maps[i].push_back(MapInfo(destination, source, range));

            ++line;
        }
    }
}

int64_t get_new_location(int64_t seed, int index)
{
    for (auto &[destination, source, range] : maps[index])
    {
        if (seed >= source and seed <= source + range - 1)
        {
            return destination + (seed - source);
        }
    }

    return seed;
}

vector<pair<int64_t, int64_t>> get_new_ranges(vector<pair<int64_t, int64_t>> &initialRanges, int index)
{
    vector<pair<int64_t, int64_t>> result;

    for (auto &[minSeed, len] : initialRanges)
    {
        int64_t maxSeed = minSeed + len - 1;

        vector<pair<int64_t, int64_t>> newFormedRanges;

        for (auto &[destination, source, range] : maps[index])
        {
            if (maxSeed < source or minSeed > source + range - 1)
            {
                continue;
            }

            int64_t minOverlap = max(minSeed, source);
            int64_t maxOverlap = min(maxSeed, source + range - 1);

            newFormedRanges.push_back({minOverlap, maxOverlap});
            result.push_back({minOverlap + destination - source, maxOverlap - minOverlap + 1});
        }

        sort(newFormedRanges.begin(), newFormedRanges.end());

        int64_t rangeStart = minSeed;
        for (auto &[minOverlap, maxOverlap] : newFormedRanges)
        {
            if (rangeStart < minOverlap)
            {
                result.push_back({rangeStart, minOverlap - rangeStart});
            }

            rangeStart = maxOverlap + 1;
        }

        if (rangeStart <= maxSeed)
        {
            result.push_back({rangeStart, maxSeed - rangeStart + 1});
        }
    }

    return result;
}

int64_t part1(vector<string> &input)
{
    get_info_from_input(input);

    int64_t result = INF;

    for (int64_t seed : seeds)
    {
        for (int i = 0; i < allMaps; ++i)
        {
            seed = get_new_location(seed, i);
        }

        result = min(result, seed);
    }

    return result;
}

int64_t part2()
{
    for (int i = 0; i < (int)seeds.size(); i += 2)
    {
        seedRanges.push_back({seeds[i], seeds[i + 1]});
    }

    int64_t result = INF;

    for (auto range : seedRanges)
    {
        vector<pair<int64_t, int64_t>> currentRanges = {range};

        for (int i = 0; i < allMaps; ++i)
        {
            currentRanges = get_new_ranges(currentRanges, i);
        }

        for (auto finalRange : currentRanges)
        {
            result = min(result, finalRange.first);
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