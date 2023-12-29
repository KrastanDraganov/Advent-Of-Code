#include <iostream>
#include <chrono>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <unordered_set>

#define endl '\n'

using namespace std;

const int MAX_ROWS = 150;
const int MAX_COLS = 150;
const int NEW_GRAPH_NODES = MAX_ROWS * MAX_COLS;

const int ALL_DIRS = 4;

const int LEFT = 0;
const int RIGHT = 1;
const int UP = 2;
const int DOWN = 3;

const int dirs[4][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};
const int oppositeDirection[4] = {RIGHT, LEFT, DOWN, UP};

int startX, startY;
int endX, endY;

int allRows, allCols;
vector<string> trailMap;

int allowedDirections[MAX_ROWS][MAX_COLS];

int startNode, endNode;
vector<int> graph[NEW_GRAPH_NODES];
int edgeLen[NEW_GRAPH_NODES][NEW_GRAPH_NODES];

bool are_coordinates_valid(int x, int y)
{
    return x >= 0 and x < allRows and y >= 0 and y < allCols;
}

void dfs_grid(int currentX, int currentY, int steps, vector<vector<int>> &dist, vector<vector<bool>> &inPath)
{
    inPath[currentX][currentY] = true;

    dist[currentX][currentY] = max(dist[currentX][currentY], steps);

    for (int dir = 0; dir < ALL_DIRS; ++dir)
    {
        if (!(allowedDirections[currentX][currentY] & (1 << dir)))
        {
            continue;
        }

        int nextX = currentX + dirs[dir][0];
        int nextY = currentY + dirs[dir][1];

        if (!are_coordinates_valid(nextX, nextY))
        {
            continue;
        }

        if (inPath[nextX][nextY])
        {
            continue;
        }

        dfs_grid(nextX, nextY, steps + 1, dist, inPath);
    }

    inPath[currentX][currentY] = false;
}

int find_max_dist_grid()
{
    vector<vector<bool>> inPath(allRows, vector<bool>(allCols, false));

    vector<vector<int>> dist(allRows, vector<int>(allCols, -1));
    dist[startX][startY] = 0;

    dfs_grid(startX, startY, 0, dist, inPath);

    return dist[endX][endY];
}

void build_edge(int node1, int node2, int steps)
{
    if (edgeLen[node1][node2] == 0)
    {
        graph[node1].push_back(node2);
        graph[node2].push_back(node1);
    }

    edgeLen[node1][node2] = edgeLen[node2][node1] = max(edgeLen[node1][node2], steps);
}

void find_edge(int x, int y, int startDir, vector<vector<bool>> &isCrossroad)
{
    int currentNode = x * MAX_ROWS + y;

    x = x + dirs[startDir][0];
    y = y + dirs[startDir][1];

    if (!are_coordinates_valid(x, y))
    {
        return;
    }

    if (trailMap[x][y] == '#')
    {
        return;
    }

    int lastDirection = startDir;
    int steps = 1;

    while (!isCrossroad[x][y])
    {
        bool isDeadEnd = true;

        for (int dir = 0; dir < ALL_DIRS; ++dir)
        {
            if (!(allowedDirections[x][y] & (1 << dir)))
            {
                continue;
            }

            if (oppositeDirection[dir] == lastDirection)
            {
                continue;
            }

            int nextX = x + dirs[dir][0];
            int nextY = y + dirs[dir][1];

            if (!are_coordinates_valid(nextX, nextY))
            {
                continue;
            }

            if (trailMap[nextX][nextY] == '#')
            {
                continue;
            }

            isDeadEnd = false;
            ++steps;

            x = nextX;
            y = nextY;
            lastDirection = dir;

            break;
        }

        if (isDeadEnd)
        {
            if (x == startX and y == startY)
            {
                build_edge(currentNode, startNode, steps);
            }
            else if (x == endX and y == endY)
            {
                build_edge(currentNode, endNode, steps);
            }

            return;
        }
    }

    int nextNode = x * MAX_ROWS + y;
    build_edge(currentNode, nextNode, steps);
}

void build_new_graph()
{
    vector<vector<bool>> isCrossroad(allRows, vector<bool>(allCols, false));
    vector<pair<int, int>> corssroads;

    for (int x = 0; x < allRows; ++x)
    {
        for (int y = 0; y < allCols; ++y)
        {
            int neighbours = 0;

            for (int dir = 0; dir < ALL_DIRS; ++dir)
            {
                if (!(allowedDirections[x][y] & (1 << dir)))
                {
                    continue;
                }

                int nextX = x + dirs[dir][0];
                int nextY = y + dirs[dir][1];

                if (!are_coordinates_valid(nextX, nextY))
                {
                    continue;
                }

                neighbours += (trailMap[nextX][nextY] != '#');
            }

            if (neighbours > 2)
            {
                isCrossroad[x][y] = true;
            }
        }
    }

    startNode = startX * MAX_ROWS + startY;
    endNode = endX * MAX_ROWS + endY;

    for (int x = 0; x < allRows; ++x)
    {
        for (int y = 0; y < allCols; ++y)
        {
            if (!isCrossroad[x][y])
            {
                continue;
            }

            for (int dir = 0; dir < ALL_DIRS; ++dir)
            {
                if (allowedDirections[x][y] & (1 << dir))
                {
                    find_edge(x, y, dir, isCrossroad);
                }
            }
        }
    }
}

void dfs_graph(int currentNode, int steps, vector<int> &dist, vector<bool> &inPath)
{
    inPath[currentNode] = true;

    dist[currentNode] = max(dist[currentNode], steps);

    for (int nextNode : graph[currentNode])
    {
        if (!inPath[nextNode])
        {
            dfs_graph(nextNode, steps + edgeLen[currentNode][nextNode], dist, inPath);
        }
    }

    inPath[currentNode] = false;
}

int find_max_dist_graph()
{
    vector<bool> inPath(NEW_GRAPH_NODES, false);

    vector<int> dist(NEW_GRAPH_NODES, -1);
    dist[startNode] = 0;

    dfs_graph(startNode, 0, dist, inPath);

    return dist[endNode];
}

int part1()
{
    for (int y = 0; y < allCols; ++y)
    {
        if (trailMap[0][y] == '.')
        {
            startX = 0;
            startY = y;
        }

        if (trailMap[allRows - 1][y] == '.')
        {
            endX = allRows - 1;
            endY = y;
        }
    }

    for (int x = 0; x < allRows; ++x)
    {
        for (int y = 0; y < allCols; ++y)
        {
            switch (trailMap[x][y])
            {
            case '.':
                allowedDirections[x][y] = (1 << ALL_DIRS) - 1;
                break;
            case '>':
                allowedDirections[x][y] |= (1 << RIGHT);
                break;
            case '<':
                allowedDirections[x][y] |= (1 << LEFT);
                break;
            case '^':
                allowedDirections[x][y] |= (1 << UP);
                break;
            case 'v':
                allowedDirections[x][y] |= (1 << DOWN);
                break;
            default:
                allowedDirections[x][y] = 0;
                break;
            }
        }
    }

    return find_max_dist_grid();
}

int part2()
{
    for (int x = 0; x < allRows; ++x)
    {
        for (int y = 0; y < allCols; ++y)
        {
            allowedDirections[x][y] = (trailMap[x][y] == '#') ? 0 : ((1 << ALL_DIRS) - 1);
        }
    }

    build_new_graph();

    return find_max_dist_graph();
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ifstream fin("input.txt");

    string row;
    while (getline(fin, row))
    {
        trailMap.push_back(row);
    }

    allRows = (int)trailMap.size();
    allCols = (int)trailMap[0].size();

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