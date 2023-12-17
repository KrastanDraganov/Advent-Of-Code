#include <iostream>
#include <chrono>
#include <fstream>
#include <vector>
#include <queue>

#define endl '\n'

using namespace std;

const int INF = 1e9 + 3;

const int MAX_ROWS = 150;
const int MAX_COLS = 150;
const int DIRECTIONS = 4; // LEFT, RIGHT, UP or DOWN

const int INITIAL_STEPS_PART_1 = 0;
const int INITIAL_STEPS_PART_2 = 4;

const int FORWARD_STEPS_LIMIT_PART_1 = 3;
const int FORWARD_STEPS_LIMIT_PART_2 = 10;

const int POSSIBLE_STEPS = 3; // LEFT, RIGHT or FORWARD

const int LEFT = 0;
const int RIGHT = 1;
const int UP = 2;
const int DOWN = 3;

const int FORWARD = 2;

const int forwardMovement[4][2] = {{0, -1}, {0, +1}, {-1, 0}, {+1, 0}};

const int turnLeft[4] = {DOWN, UP, LEFT, RIGHT};
const int turnRight[4] = {UP, DOWN, RIGHT, LEFT};

int prefixSums[MAX_ROWS + 1][MAX_COLS + 1]; // 1-indexed

int minHeat[MAX_ROWS][MAX_COLS][DIRECTIONS][FORWARD_STEPS_LIMIT_PART_2 + 1];
bool inQueue[MAX_ROWS][MAX_COLS][DIRECTIONS][FORWARD_STEPS_LIMIT_PART_2 + 1];

struct Position
{
    int x, y;
    int direction;
    int madeForwardSteps;

    Position(int _x, int _y, int _direction, int _madeForwardSteps) : x(_x), y(_y), direction(_direction), madeForwardSteps(_madeForwardSteps) {}
};

void calculate_prefix_sums(vector<vector<int>> &map, int rows, int cols)
{
    prefixSums[0][0] = 0;

    for (int x = 1; x <= rows; ++x)
    {
        prefixSums[x][0] = 0;

        for (int y = 1; y <= cols; ++y)
        {
            prefixSums[x][y] = prefixSums[x - 1][y] + prefixSums[x][y - 1] - prefixSums[x - 1][y - 1] + map[x - 1][y - 1];
        }
    }
}

int find_sum(int x1, int y1, int x2, int y2)
{
    if (x1 > x2 or y1 > y2)
    {
        swap(x1, x2);
        swap(y1, y2);
    }

    return prefixSums[x2 + 1][y2 + 1] - prefixSums[x1][y2 + 1] - prefixSums[x2 + 1][y1] + prefixSums[x1][y1];
}

void goForward(queue<Position> &spfa, int stepsToMake, int direction, int x, int y, int madeForwardSteps, int rows, int cols, vector<vector<int>> &map)
{
    if (stepsToMake <= 0)
    {
        stepsToMake = 1;
    }

    int nextX = x + forwardMovement[direction][0] * stepsToMake;
    int nextY = y + forwardMovement[direction][1] * stepsToMake;

    if (nextX < 0 or nextX >= rows or nextY < 0 or nextY >= cols)
    {
        return;
    }

    int currentHeat = minHeat[x][y][direction][madeForwardSteps] + find_sum(x, y, nextX, nextY) - map[x][y];

    if (currentHeat >= minHeat[nextX][nextY][direction][madeForwardSteps + stepsToMake])
    {
        return;
    }

    minHeat[nextX][nextY][direction][madeForwardSteps + stepsToMake] = currentHeat;

    if (!inQueue[nextX][nextY][direction][madeForwardSteps + stepsToMake])
    {
        spfa.push(Position(nextX, nextY, direction, madeForwardSteps + stepsToMake));
        inQueue[nextX][nextY][direction][madeForwardSteps + stepsToMake] = true;
    }
}

int find_min_heat(vector<vector<int>> &map, int startingX, int startingY, int endingX, int endingY, int rows, int cols, int forwardStepsLimit, int initialSteps)
{
    for (int x = 0; x < rows; ++x)
    {
        for (int y = 0; y < cols; ++y)
        {
            for (int direction = 0; direction < DIRECTIONS; ++direction)
            {
                for (int steps = 0; steps <= forwardStepsLimit; ++steps)
                {
                    minHeat[x][y][direction][steps] = INF;
                    inQueue[x][y][direction][steps] = false;
                }
            }
        }
    }

    queue<Position> spfa;

    spfa.push(Position(startingX, startingY + initialSteps, RIGHT, initialSteps));
    minHeat[startingX][startingY + initialSteps][RIGHT][initialSteps] = initialSteps <= 0 ? 0 : find_sum(startingX, startingY + 1, startingX, startingY + initialSteps);
    inQueue[startingX][startingY + initialSteps][RIGHT][initialSteps] = true;

    spfa.push(Position(startingX + initialSteps, startingY, DOWN, initialSteps));
    minHeat[startingX + initialSteps][startingY][DOWN][initialSteps] = initialSteps <= 0 ? 0 : find_sum(startingX + 1, startingY, startingX + initialSteps, startingY);
    inQueue[startingX + initialSteps][startingY][DOWN][initialSteps] = true;

    while (!spfa.empty())
    {
        auto [x, y, direction, madeForwardSteps] = spfa.front();

        spfa.pop();
        inQueue[x][y][direction][madeForwardSteps] = false;

        for (int nextStep = 0; nextStep < POSSIBLE_STEPS; ++nextStep)
        {
            if (nextStep == FORWARD)
            {
                if (madeForwardSteps == forwardStepsLimit)
                {
                    continue;
                }

                goForward(spfa, 1, direction, x, y, madeForwardSteps, rows, cols, map);
            }
            else if (nextStep == LEFT)
            {
                int nextDirection = turnLeft[direction];

                if (minHeat[x][y][direction][madeForwardSteps] >= minHeat[x][y][nextDirection][0])
                {
                    continue;
                }

                minHeat[x][y][nextDirection][0] = minHeat[x][y][direction][madeForwardSteps];

                goForward(spfa, initialSteps, nextDirection, x, y, 0, rows, cols, map);
            }
            else if (nextStep == RIGHT)
            {
                int nextDirection = turnRight[direction];

                if (minHeat[x][y][direction][madeForwardSteps] >= minHeat[x][y][nextDirection][0])
                {
                    continue;
                }

                minHeat[x][y][nextDirection][0] = minHeat[x][y][direction][madeForwardSteps];

                goForward(spfa, initialSteps, nextDirection, x, y, 0, rows, cols, map);
            }
        }
    }

    int result = INF;
    for (int direction = 0; direction < DIRECTIONS; ++direction)
    {
        for (int steps = initialSteps; steps <= forwardStepsLimit; ++steps)
        {
            result = min(result, minHeat[endingX][endingY][direction][steps]);
        }
    }

    return result;
}

int part1(vector<vector<int>> &map)
{
    int rows = (int)map.size();
    int cols = (int)map[0].size();

    calculate_prefix_sums(map, rows, cols);

    return find_min_heat(map, 0, 0, rows - 1, cols - 1, rows, cols, FORWARD_STEPS_LIMIT_PART_1, INITIAL_STEPS_PART_1);
}

int part2(vector<vector<int>> &map)
{
    int rows = (int)map.size();
    int cols = (int)map[0].size();

    return find_min_heat(map, 0, 0, rows - 1, cols - 1, rows, cols, FORWARD_STEPS_LIMIT_PART_2, INITIAL_STEPS_PART_2);
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ifstream fin("input.txt");

    vector<vector<int>> map;

    string row;
    while (getline(fin, row))
    {
        map.push_back({});
        for (char digit : row)
        {
            map.back().push_back(digit - '0');
        }
    }

    fin.close();

    auto t_begin = chrono::high_resolution_clock::now();
    cout << "Result, part 1: " << part1(map) << endl;
    auto t_end = chrono::high_resolution_clock::now();

    auto part1Time = chrono::duration<double, std::milli>(t_end - t_begin).count();

    t_begin = chrono::high_resolution_clock::now();
    cout << "Result, part 2: " << part2(map) << endl;
    t_end = chrono::high_resolution_clock::now();

    auto part2Time = chrono::duration<double, std::milli>(t_end - t_begin).count();

    cout << "Completed part 1 in: " << part1Time << " ms" << endl;
    cout << "Completed part 2 in: " << part2Time << " ms" << endl;

    return 0;
}