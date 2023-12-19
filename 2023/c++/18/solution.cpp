#include <iostream>
#include <chrono>
#include <fstream>
#include <vector>
#include <unordered_map>

#define endl '\n'

using namespace std;

const int INF = 1e9 + 3;
const int MAX_COORDINATE = 503;

const int DIRECTIONS = 4; // LEFT, RIGHT, UP or DOWN

const int RIGHT = 0;
const int DOWN = 1;
const int LEFT = 2;
const int UP = 3;

const int forwardMovement[4][2] = {{0, +1}, {+1, 0}, {0, -1}, {-1, 0}};

unordered_map<char, int> letterToDirection = {{'L', LEFT}, {'R', RIGHT}, {'U', UP}, {'D', DOWN}};

struct DigInfo
{
    int direction;
    int steps;
    string color;

    DigInfo(int _direction, int _steps, string _color) : direction(_direction), steps(_steps), color(_color) {}
};

vector<DigInfo> digs;

int minX, minY;
int maxX, maxY;

bool isDug[MAX_COORDINATE][MAX_COORDINATE];
bool visited[MAX_COORDINATE][MAX_COORDINATE];

void extract_dig_info(vector<string> &plan)
{
    for (string &dig : plan)
    {
        int direction = letterToDirection[dig[0]];

        int steps = 0, i = 2;
        while (dig[i] >= '0' and dig[i] <= '9')
        {
            steps = 10 * steps + (dig[i] - '0');
            ++i;
        }

        string color = dig.substr(i + 3, 6);

        digs.push_back(DigInfo(direction, steps, color));
    }
}

void fix_dig_info()
{
    for (int i = 0; i < (int)digs.size(); ++i)
    {
        string color = digs[i].color;

        digs[i].steps = 0;
        for (int j = 0; j < 5; ++j)
        {
            int digit = (color[j] >= '0' and color[j] <= '9')
                            ? color[j] - '0'
                            : color[j] - 'a' + 10;
            digs[i].steps = 16 * digs[i].steps + digit;
        }

        digs[i].direction = color[5] - '0';
    }
}

int dfs(int x, int y)
{
    visited[x][y] = true;

    int result = 1;
    for (int i = 0; i < DIRECTIONS; ++i)
    {
        int nextX = x + forwardMovement[i][0];
        int nextY = y + forwardMovement[i][1];

        if (nextX < 0 or nextX > maxX or nextY < 0 or nextY > maxY)
        {
            continue;
        }

        if (isDug[nextX][nextY] or visited[nextX][nextY])
        {
            continue;
        }

        result += dfs(nextX, nextY);
    }

    return result;
}

int find_lava_volume_with_dfs()
{
    int x = 0, y = 0;

    minX = 0;
    minY = 0;

    for (auto &dig : digs)
    {
        x += forwardMovement[dig.direction][0] * dig.steps;
        y += forwardMovement[dig.direction][1] * dig.steps;

        minX = min(minX, x);
        minY = min(minY, y);
    }

    x = -minX;
    y = -minY;
    isDug[x][y] = true;

    maxX = 0;
    maxY = 0;

    for (auto &dig : digs)
    {
        for (int i = 0; i < dig.steps; ++i)
        {
            x += forwardMovement[dig.direction][0];
            y += forwardMovement[dig.direction][1];

            isDug[x][y] = true;
        }

        maxX = max(maxX, x);
        maxY = max(maxY, y);
    }

    int leftCells = (maxX + 1) * (maxY + 1);

    for (int x = 0; x <= maxX; ++x)
    {
        if (!isDug[x][0] and !visited[x][0])
        {
            leftCells -= dfs(x, 0);
        }

        if (!isDug[x][maxY] and !visited[x][maxY])
        {
            leftCells -= dfs(x, maxY);
        }
    }

    return leftCells;
}

int64_t find_lava_volume_with_formula()
{
    int x = 0, y = 0;

    int64_t area = 0ll;
    int64_t perimeter = 0ll;

    for (int i = 0; i < (int)digs.size(); ++i)
    {
        int newX = x + forwardMovement[digs[i].direction][1] * digs[i].steps;
        int newY = y + forwardMovement[digs[i].direction][0] * digs[i].steps;

        area += 1ll * x * newY - 1ll * y * newX;
        perimeter += 1ll * digs[i].steps;

        x = newX;
        y = newY;
    }

    if (area < 0)
    {
        area = -area;
    }

    return area / 2ll + perimeter / 2ll + 1ll;
}

int part1(vector<string> &plan)
{
    extract_dig_info(plan);

    return find_lava_volume_with_dfs();
}

int64_t part2()
{
    fix_dig_info();

    return find_lava_volume_with_formula();
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ifstream fin("input.txt");

    vector<string> plan;

    string row;
    while (getline(fin, row))
    {
        plan.push_back(row);
    }

    fin.close();

    auto t_begin = chrono::high_resolution_clock::now();
    cout << "Result, part 1: " << part1(plan) << endl;
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