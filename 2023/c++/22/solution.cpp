#include <iostream>
#include <chrono>
#include <fstream>
#include <sstream>
#include <vector>
#include <array>
#include <algorithm>
#include <unordered_set>
#include <queue>

#define endl '\n'

using namespace std;

const int MAX_BRICKS = 1500;
const int MAX_COORDINATE = 380;

struct Brick
{
    array<int, 3> p1;
    array<int, 3> p2;

    Brick(array<int, 3> &_p1, array<int, 3> &_p2) : p1(_p1), p2(_p2) {}

    bool operator<(const Brick &comp) const
    {
        return p1[2] < comp.p1[2];
    }
};

int allBricks;
vector<Brick> bricks;

int maxAvailableCoord[MAX_COORDINATE][MAX_COORDINATE];
int brickId[MAX_COORDINATE][MAX_COORDINATE];

vector<int> graph[MAX_BRICKS];
int inDegree[MAX_BRICKS];

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

array<int, 3> get_coordinates(string &input)
{
    vector<string> coords = split_string(input, ',');

    array<int, 3> result;
    for (int i = 0; i < 3; ++i)
    {
        result[i] = stoi(coords[i]);
    }

    return result;
}

int part1()
{
    sort(bricks.begin(), bricks.end());

    for (int i = 0; i < MAX_COORDINATE; ++i)
    {
        for (int j = 0; j < MAX_COORDINATE; ++j)
        {
            maxAvailableCoord[i][j] = 0;
            brickId[i][j] = -1;
        }
    }

    unordered_set<int> neededBricks;

    for (int i = 0; i < allBricks; ++i)
    {
        auto [x1, y1, z1] = bricks[i].p1;
        auto [x2, y2, z2] = bricks[i].p2;

        int currentMax = 0;
        for (int x = min(x1, x2); x <= max(x1, x2); ++x)
        {
            for (int y = min(y1, y2); y <= max(y1, y2); ++y)
            {
                currentMax = max(currentMax, maxAvailableCoord[x][y]);
            }
        }

        unordered_set<int> dependentBricks;
        for (int x = min(x1, x2); x <= max(x1, x2); ++x)
        {
            for (int y = min(y1, y2); y <= max(y1, y2); ++y)
            {
                if (maxAvailableCoord[x][y] == currentMax and brickId[x][y] != -1)
                {
                    dependentBricks.insert(brickId[x][y]);
                }

                maxAvailableCoord[x][y] = currentMax + z2 - z1 + 1;
                brickId[x][y] = i;
            }
        }

        for (int neighbour : dependentBricks)
        {
            graph[neighbour].push_back(i);
            ++inDegree[i];
        }

        if ((int)dependentBricks.size() == 1)
        {
            neededBricks.insert(*dependentBricks.begin());
        }
    }

    return allBricks - (int)neededBricks.size();
}

int part2()
{
    int result = 0;

    for (int startBrick = 0; startBrick < allBricks; ++startBrick)
    {
        vector<int> leftDependentBricks(allBricks);

        for (int i = 0; i < allBricks; ++i)
        {
            leftDependentBricks[i] = inDegree[i];
        }

        queue<int> bfs;
        bfs.push(startBrick);

        while (!bfs.empty())
        {
            int currentBrick = bfs.front();
            bfs.pop();

            if (currentBrick != startBrick)
            {
                ++result;
            }

            for (int nextBrick : graph[currentBrick])
            {
                --leftDependentBricks[nextBrick];

                if (leftDependentBricks[nextBrick] == 0)
                {
                    bfs.push(nextBrick);
                }
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
        vector<string> points = split_string(row, '~');

        array<int, 3> p1 = get_coordinates(points[0]);
        array<int, 3> p2 = get_coordinates(points[1]);

        if (p1[2] > p2[2])
        {
            swap(p1, p2);
        }

        bricks.push_back(Brick(p1, p2));
    }

    allBricks = (int)bricks.size();

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