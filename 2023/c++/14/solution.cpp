#include <iostream>
#include <chrono>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>

#define endl '\n'

using namespace std;

const int ALL_CYCLES = 1e9;

int allRows, allCols;
vector<string> map;

void roll_north()
{
    for (int col = 0; col < allCols; ++col)
    {
        int availableRow = 0;

        for (int row = 0; row < allRows; ++row)
        {
            if (map[row][col] == '#')
            {
                availableRow = row + 1;
            }
            else if (map[row][col] == 'O')
            {
                swap(map[availableRow][col], map[row][col]);
                ++availableRow;
            }
        }
    }
}

void roll_west()
{
    for (int row = 0; row < allRows; ++row)
    {
        int availableCol = 0;

        for (int col = 0; col < allCols; ++col)
        {
            if (map[row][col] == '#')
            {
                availableCol = col + 1;
            }
            else if (map[row][col] == 'O')
            {
                swap(map[row][availableCol], map[row][col]);
                ++availableCol;
            }
        }
    }
}

void roll_south()
{
    for (int col = 0; col < allCols; ++col)
    {
        int availableRow = allRows - 1;

        for (int row = allRows - 1; row >= 0; --row)
        {
            if (map[row][col] == '#')
            {
                availableRow = row - 1;
            }
            else if (map[row][col] == 'O')
            {
                swap(map[availableRow][col], map[row][col]);
                --availableRow;
            }
        }
    }
}

void roll_east()
{
    for (int row = 0; row < allRows; ++row)
    {
        int availableCol = allCols - 1;

        for (int col = allCols - 1; col >= 0; --col)
        {
            if (map[row][col] == '#')
            {
                availableCol = col - 1;
            }
            else if (map[row][col] == 'O')
            {
                swap(map[row][availableCol], map[row][col]);
                --availableCol;
            }
        }
    }
}

string get_string_from_map()
{
    string result = "";

    for (string &row : map)
    {
        result += row;
    }

    return result;
}

int calculate_north_support(bool moveRocks)
{
    vector<int> rockCounterOnRow(allRows, 0);

    for (int col = 0; col < allCols; ++col)
    {
        int availableRow = 0;

        for (int row = 0; row < allRows; ++row)
        {
            if (map[row][col] == '#')
            {
                availableRow = row + 1;
            }
            else if (map[row][col] == 'O')
            {
                if (moveRocks)
                {
                    ++rockCounterOnRow[availableRow];
                }
                else
                {
                    ++rockCounterOnRow[row];
                }

                ++availableRow;
            }
        }
    }

    int result = 0;
    for (int row = 0; row < allRows; ++row)
    {
        result += rockCounterOnRow[row] * (allRows - row);
    }

    return result;
}

int part1()
{
    return calculate_north_support(true);
}

int part2()
{
    vector<int> cycleLoad;
    unordered_map<string, int> cachedMap;

    for (int cycle = 0; cycle < ALL_CYCLES; ++cycle)
    {
        roll_north();
        roll_west();
        roll_south();
        roll_east();

        string currentMapString = get_string_from_map();

        if (cachedMap.count(currentMapString) > 0)
        {
            int repetitionStart = cachedMap[currentMapString];
            int repetitionSize = cycle - repetitionStart;
            int cyclesInRepetition = ALL_CYCLES - repetitionStart;

            int targetCycle = cyclesInRepetition % repetitionSize == 0 ? repetitionStart : cyclesInRepetition % repetitionSize + repetitionStart - 1;

            return cycleLoad[targetCycle];
        }

        int currentNorthSupport = calculate_north_support(false);

        cycleLoad.push_back(currentNorthSupport);
        cachedMap[currentMapString] = cycle;
    }

    return -1;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ifstream fin("input.txt");

    string row;
    while (getline(fin, row))
    {
        map.push_back(row);
    }

    allRows = (int)map.size();
    allCols = (int)map[0].size();

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