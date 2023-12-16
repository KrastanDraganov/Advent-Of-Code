#include <iostream>
#include <chrono>
#include <fstream>
#include <vector>
#include <queue>

#define endl '\n'

using namespace std;

const int LEFT = 0;
const int RIGHT = 1;
const int UP = 2;
const int DOWN = 3;

const int forwardMovement[4][2] = {{0, -1}, {0, +1}, {-1, 0}, {+1, 0}};

const int backSlashMirror[4] = {UP, DOWN, LEFT, RIGHT};
const int forwardSlashMirror[4] = {DOWN, UP, RIGHT, LEFT};

struct BeamPosition
{
    int direction;
    int x, y;

    BeamPosition(int _direction, int _x, int _y) : direction(_direction), x(_x), y(_y) {}
};

void goForward(queue<BeamPosition> &bfs, vector<vector<int>> &isEnergized, int direction, int x, int y, int rows, int cols)
{
    int nextX = x + forwardMovement[direction][0];
    int nextY = y + forwardMovement[direction][1];

    if (nextX < 0 or nextX >= rows or nextY < 0 or nextY >= cols)
    {
        return;
    }

    if (isEnergized[nextX][nextY] & (1 << direction))
    {
        return;
    }

    bfs.push(BeamPosition(direction, nextX, nextY));
}

int find_energized_tiles(vector<string> &contraption, int startingDirection, int startingX, int startingY, int rows, int cols)
{
    queue<BeamPosition> bfs;
    bfs.push(BeamPosition(startingDirection, startingX, startingY));

    vector<vector<int>> isEnergized(rows, vector<int>(cols, 0));
    int energizedCounter = 0;

    while (!bfs.empty())
    {
        auto [direction, x, y] = bfs.front();
        bfs.pop();

        if (isEnergized[x][y] == 0)
        {
            ++energizedCounter;
        }

        isEnergized[x][y] |= (1 << direction);

        if (contraption[x][y] == '.')
        {
            goForward(bfs, isEnergized, direction, x, y, rows, cols);
        }
        else if (contraption[x][y] == '|')
        {
            if (direction == UP or direction == DOWN)
            {
                goForward(bfs, isEnergized, direction, x, y, rows, cols);
            }
            else
            {
                goForward(bfs, isEnergized, UP, x, y, rows, cols);
                goForward(bfs, isEnergized, DOWN, x, y, rows, cols);
            }
        }
        else if (contraption[x][y] == '-')
        {
            if (direction == LEFT or direction == RIGHT)
            {
                goForward(bfs, isEnergized, direction, x, y, rows, cols);
            }
            else
            {
                goForward(bfs, isEnergized, LEFT, x, y, rows, cols);
                goForward(bfs, isEnergized, RIGHT, x, y, rows, cols);
            }
        }
        else if (contraption[x][y] == '/')
        {
            goForward(bfs, isEnergized, forwardSlashMirror[direction], x, y, rows, cols);
        }
        else if (contraption[x][y] == '\\')
        {
            goForward(bfs, isEnergized, backSlashMirror[direction], x, y, rows, cols);
        }
    }

    return energizedCounter;
}

int part1(vector<string> &contraption)
{
    int rows = (int)contraption.size();
    int cols = (int)contraption[0].size();

    return find_energized_tiles(contraption, RIGHT, 0, 0, rows, cols);
}

int part2(vector<string> &contraption)
{
    int rows = (int)contraption.size();
    int cols = (int)contraption[0].size();

    int maxEnergizedTiles = 0;

    for (int x = 0; x < rows; ++x)
    {
        maxEnergizedTiles = max(maxEnergizedTiles, find_energized_tiles(contraption, RIGHT, x, 0, rows, cols));
        maxEnergizedTiles = max(maxEnergizedTiles, find_energized_tiles(contraption, LEFT, x, cols - 1, rows, cols));
    }

    for (int y = 0; y < cols; ++y)
    {
        maxEnergizedTiles = max(maxEnergizedTiles, find_energized_tiles(contraption, DOWN, 0, y, rows, cols));
        maxEnergizedTiles = max(maxEnergizedTiles, find_energized_tiles(contraption, UP, rows - 1, y, rows, cols));
    }

    return maxEnergizedTiles;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ifstream fin("input.txt");

    vector<string> contraption;

    string row;
    while (getline(fin, row))
    {
        contraption.push_back(row);
    }

    fin.close();

    auto t_begin = chrono::high_resolution_clock::now();
    cout << "Result, part 1: " << part1(contraption) << endl;
    auto t_end = chrono::high_resolution_clock::now();

    auto part1Time = chrono::duration<double, std::milli>(t_end - t_begin).count();

    t_begin = chrono::high_resolution_clock::now();
    cout << "Result, part 2: " << part2(contraption) << endl;
    t_end = chrono::high_resolution_clock::now();

    auto part2Time = chrono::duration<double, std::milli>(t_end - t_begin).count();

    cout << "Completed part 1 in: " << part1Time << " ms" << endl;
    cout << "Completed part 2 in: " << part2Time << " ms" << endl;
}