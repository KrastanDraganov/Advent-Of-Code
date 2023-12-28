#include <iostream>
#include <chrono>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <map>

#define endl '\n'

using namespace std;

const int ALL_STEPS_PART_1 = 64;
const int ALL_STEPS_PART_2 = 26501365;

const int ALL_DIRS = 4;
const int dirs[4][2] = {{0, -1}, {0, 1}, {1, 0}, {-1, 0}};

int allRows, allCols;
int startX, startY;
vector<string> garden;

int part1()
{
    for (int x = 0; x < allRows; ++x)
    {
        for (int y = 0; y < allCols; ++y)
        {
            if (garden[x][y] == 'S')
            {
                startX = x;
                startY = y;
                break;
            }
        }
    }

    queue<pair<int, int>> bfs;
    bfs.push({startX, startY});

    for (int steps = 1; steps <= ALL_STEPS_PART_1; ++steps)
    {
        int queueSize = (int)bfs.size();
        vector<vector<bool>> addedToQueue(allRows, vector<bool>(allCols, false));

        for (int i = 0; i < queueSize; ++i)
        {
            auto [currentX, currentY] = bfs.front();
            bfs.pop();

            for (int dir = 0; dir < ALL_DIRS; ++dir)
            {
                int nextX = currentX + dirs[dir][0];
                int nextY = currentY + dirs[dir][1];

                if (nextX < 0 or nextX >= allRows or nextY < 0 or nextY >= allCols)
                {
                    continue;
                }

                if (garden[nextX][nextY] == '#')
                {
                    continue;
                }

                if (!addedToQueue[nextX][nextY])
                {
                    bfs.push({nextX, nextY});
                    addedToQueue[nextX][nextY] = true;
                }
            }
        }
    }

    return (int)bfs.size();
}

int64_t part2()
{
    queue<pair<int, int>> bfs;
    bfs.push({startX, startY});

    map<pair<int, int>, bool> addedToQueue;
    addedToQueue[{startX, startY}] = true;

    vector<int> nodesCounter;
    nodesCounter.push_back(1);

    vector<int> cycles;

    for (int steps = 1; steps <= ALL_STEPS_PART_2 and (int) cycles.size() < 3; ++steps)
    {
        int queueSize = nodesCounter.back();

        for (int i = 0; i < queueSize; ++i)
        {
            auto [currentX, currentY] = bfs.front();
            bfs.pop();

            for (int dir = 0; dir < ALL_DIRS; ++dir)
            {
                int nextX = ((currentX + dirs[dir][0]) % allRows + allRows) % allRows;
                int nextY = ((currentY + dirs[dir][1]) % allCols + allCols) % allCols;

                if (garden[nextX][nextY] == '#')
                {
                    continue;
                }

                nextX = currentX + dirs[dir][0];
                nextY = currentY + dirs[dir][1];

                if (!addedToQueue[{nextX, nextY}])
                {
                    bfs.push({nextX, nextY});
                    addedToQueue[{nextX, nextY}] = true;
                }
            }
        }

        if ((steps - 1) % allRows == ALL_STEPS_PART_2 % allRows)
        {
            int counterSum = 0;

            for (int i = 0; i < (int)nodesCounter.size(); ++i)
            {
                if ((steps - 1) % 2 == i % 2)
                {
                    counterSum += nodesCounter[i];
                }
            }

            cycles.push_back(counterSum);
        }

        nodesCounter.push_back((int)bfs.size());
    }

    int x = cycles[0], y = cycles[1] - cycles[0], z = cycles[2] - cycles[1];
    int k = ALL_STEPS_PART_2 / allRows;

    return 1ll * x + 1ll * y * k + (1ll * k * (k - 1) / 2ll) * (z - y);
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ifstream fin("input.txt");

    string row;
    while (getline(fin, row))
    {
        garden.push_back(row);
    }

    allRows = (int)garden.size();
    allCols = (int)garden[0].size();

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