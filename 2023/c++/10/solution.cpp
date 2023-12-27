#include <iostream>
#include <chrono>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>

#define endl '\n'

using namespace std;

const int INF = 1e9 + 3;

const int MAX_ROWS = 150;
const int MAX_COLS = 150;

const int ALL_DIRS = 4;
const int LEFT = 0;
const int RIGHT = 1;
const int UP = 2;
const int DOWN = 3;

const int dirs[4][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};
const int oppositeDirection[4] = {RIGHT, LEFT, DOWN, UP};

int rows;
int cols;
vector<string> map;

int edges[MAX_ROWS][MAX_COLS];
int dist[MAX_ROWS][MAX_COLS];

// The idea is too add new nodes to the graph that represent the edges
bool isPartOfNewGraph[2 * MAX_ROWS][2 * MAX_COLS];
bool visited[2 * MAX_ROWS][2 * MAX_COLS];

bool isNode(int x, int y)
{
    return x % 2 == 0 and y % 2 == 0;
}

pair<int, bool> dfs(int currentX, int currentY)
{
    visited[currentX][currentY] = true;

    int componentSize = isNode(currentX, currentY);
    bool isSurroundedByLoop = true;

    for (int dir = 0; dir < ALL_DIRS; ++dir)
    {
        int nextX = currentX + dirs[dir][0];
        int nextY = currentY + dirs[dir][1];

        if (nextX < 0 or nextX >= 2 * rows or nextY < 0 or nextY >= 2 * cols)
        {
            isSurroundedByLoop = false;
            continue;
        }

        if (visited[nextX][nextY])
        {
            continue;
        }

        auto [newNodes, isNextSurroundedByLoop] = dfs(nextX, nextY);
        componentSize += newNodes;
        isSurroundedByLoop &= isNextSurroundedByLoop;
    }

    return {componentSize, isSurroundedByLoop};
}

int part1()
{
    pair<int, int> startPosition = {-1, -1};

    for (int x = 0; x < rows; ++x)
    {
        for (int y = 0; y < cols; ++y)
        {
            switch (map[x][y])
            {
            case 'S':
                startPosition = {x, y};
                edges[x][y] = (1 << ALL_DIRS) - 1;
                break;
            case '|':
                edges[x][y] |= (1 << UP);
                edges[x][y] |= (1 << DOWN);
                break;
            case '-':
                edges[x][y] |= (1 << LEFT);
                edges[x][y] |= (1 << RIGHT);
                break;
            case 'L':
                edges[x][y] |= (1 << UP);
                edges[x][y] |= (1 << RIGHT);
                break;
            case 'J':
                edges[x][y] |= (1 << UP);
                edges[x][y] |= (1 << LEFT);
                break;
            case '7':
                edges[x][y] |= (1 << LEFT);
                edges[x][y] |= (1 << DOWN);
                break;
            case 'F':
                edges[x][y] |= (1 << RIGHT);
                edges[x][y] |= (1 << DOWN);
                break;
            default:
                break;
            }
        }
    }

    for (int x = 0; x < rows; ++x)
    {
        for (int y = 0; y < cols; ++y)
        {
            dist[x][y] = INF;
        }
    }

    queue<pair<int, int>> bfs;

    bfs.push(startPosition);
    dist[startPosition.first][startPosition.second] = 0;

    int result = 0;
    while (!bfs.empty())
    {
        auto [currentX, currentY] = bfs.front();
        bfs.pop();

        result = max(result, dist[currentX][currentY]);

        isPartOfNewGraph[currentX * 2][currentY * 2] = true;

        for (int dir = 0; dir < ALL_DIRS; ++dir)
        {
            if ((edges[currentX][currentY] & (1 << dir)) == 0)
            {
                continue;
            }

            int nextX = currentX + dirs[dir][0];
            int nextY = currentY + dirs[dir][1];

            if (nextX < 0 or nextX >= rows or nextY < 0 or nextY >= cols)
            {
                continue;
            }

            if ((edges[nextX][nextY] & (1 << oppositeDirection[dir])) == 0)
            {
                continue;
            }

            int edgeX = 2 * currentX + dirs[dir][0];
            int edgeY = 2 * currentY + dirs[dir][1];

            isPartOfNewGraph[edgeX][edgeY] = true;

            int newDist = dist[currentX][currentY] + 1;
            if (newDist >= dist[nextX][nextY])
            {
                continue;
            }

            dist[nextX][nextY] = newDist;
            bfs.push({nextX, nextY});
        }
    }

    return result;
}

int part2()
{
    for (int x = 0; x < 2 * rows; ++x)
    {
        for (int y = 0; y < 2 * cols; ++y)
        {
            visited[x][y] = isPartOfNewGraph[x][y];
        }
    }

    int result = 0;
    for (int x = 0; x < 2 * rows; ++x)
    {
        for (int y = 0; y < 2 * cols; ++y)
        {
            if (visited[x][y])
            {
                continue;
            }

            auto [componentSize, isSurroundedByLoop] = dfs(x, y);

            if (isSurroundedByLoop)
            {
                result += componentSize;
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

    string row;
    while (getline(fin, row))
    {
        map.push_back(row);
    }

    fin.close();

    rows = (int)map.size();
    cols = (int)map[0].size();

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