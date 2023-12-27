#include <iostream>
#include <chrono>
#include <fstream>
#include <sstream>
#include <vector>

#define endl '\n'
#define int int64_t

using namespace std;

const int EXPANSION_PART_1 = 2;
const int EXPANSION_PART_2 = 1e6;

const int MAX_ROWS = 150;
const int MAX_COLS = 150;

int rows, cols;
vector<string> map;

vector<pair<int, int>> galaxies;

bool hasRowGalaxy[MAX_ROWS];
int emptyRowsCounter[MAX_ROWS];

bool hasColGalaxy[MAX_COLS];
int emptyColsCounter[MAX_COLS];

void expand_galaxies(int factor)
{
    for (int row = 1; row <= rows; ++row)
    {
        emptyRowsCounter[row] = emptyRowsCounter[row - 1] + !hasRowGalaxy[row] * (factor - 1);
    }

    for (int col = 1; col <= cols; ++col)
    {
        emptyColsCounter[col] = emptyColsCounter[col - 1] + !hasColGalaxy[col] * (factor - 1);
    }
}

int64_t calculate_distance()
{
    int64_t result = 0;
    for (int i = 0; i < (int)galaxies.size(); ++i)
    {
        for (int j = i + 1; j < (int)galaxies.size(); ++j)
        {
            auto [x1, y1] = galaxies[i];
            auto [x2, y2] = galaxies[j];

            if (x1 > x2)
            {
                swap(x1, x2);
            }
            if (y1 > y2)
            {
                swap(y1, y2);
            }

            result += (x2 - x1) + (y2 - y1);
            result += (emptyRowsCounter[x2] - emptyRowsCounter[x1 - 1]) + (emptyColsCounter[y2] - emptyColsCounter[y1 - 1]);
        }
    }

    return result;
}

int64_t part1()
{
    for (int x = 0; x < rows; ++x)
    {
        for (int y = 0; y < cols; ++y)
        {
            if (map[x][y] == '#')
            {
                galaxies.push_back({x + 1, y + 1});
                hasRowGalaxy[x + 1] = true;
                hasColGalaxy[y + 1] = true;
            }
        }
    }

    expand_galaxies(EXPANSION_PART_1);

    return calculate_distance();
}

int64_t part2()
{
    expand_galaxies(EXPANSION_PART_2);

    return calculate_distance();
}

signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ifstream fin("input.txt");

    string row;
    while (getline(fin, row))
    {
        map.push_back(row);
    }

    rows = (int)map.size();
    cols = (int)map[0].size();

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