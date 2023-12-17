#include <iostream>
#include <chrono>
#include <fstream>
#include <unordered_set>

#define endl '\n'

using namespace std;

const int ALL_DIRS = 8;

const int dirs[ALL_DIRS][2] = {{0, -1}, {0, +1}, {-1, 0}, {+1, 0}, {-1, -1}, {-1, +1}, {+1, -1}, {+1, +1}};

int part1(vector<string> &engine)
{
    int rows = (int)engine.size();
    int cols = (int)engine[0].size();

    vector<vector<bool>> isNearSymbol(rows, vector<bool>(cols, false));

    for (int x = 0; x < rows; ++x)
    {
        for (int y = 0; y < cols; ++y)
        {
            if (engine[x][y] == '.' or (engine[x][y] >= '0' and engine[x][y] <= '9'))
            {
                continue;
            }

            for (int dir = 0; dir < ALL_DIRS; ++dir)
            {
                int nextX = x + dirs[dir][0];
                int nextY = y + dirs[dir][1];

                if (nextX >= 0 and nextX < rows and nextY >= 0 and nextY < cols)
                {
                    isNearSymbol[nextX][nextY] = true;
                }
            }
        }
    }

    int result = 0;

    for (int x = 0; x < rows; ++x)
    {
        int number = 0;
        bool isPartOfEngine = false;

        for (int y = 0; y < cols; ++y)
        {
            if (engine[x][y] >= '0' and engine[x][y] <= '9')
            {
                number = 10 * number + (engine[x][y] - '0');
                isPartOfEngine |= isNearSymbol[x][y];
            }
            else
            {
                if (isPartOfEngine)
                {
                    result += number;
                }

                number = 0;
                isPartOfEngine = false;
            }
        }

        if (isPartOfEngine)
        {
            result += number;
        }
    }

    return result;
}

int part2(vector<string> &engine)
{
    int rows = (int)engine.size();
    int cols = (int)engine[0].size();

    vector<vector<int>> numbers(rows, vector<int>(cols, 0));

    for (int x = 0; x < rows; ++x)
    {
        int currentNumber = 0;
        int startIndex = -1;

        for (int y = 0; y < cols; ++y)
        {
            if (engine[x][y] >= '0' and engine[x][y] <= '9')
            {
                if (currentNumber == 0)
                {
                    startIndex = y;
                }

                currentNumber = 10 * currentNumber + (engine[x][y] - '0');
            }
            else if (startIndex != -1)
            {
                for (int i = startIndex; i <= y - 1; ++i)
                {
                    numbers[x][i] = currentNumber;
                }

                currentNumber = 0;
                startIndex = -1;
            }
        }

        if (startIndex != -1)
        {
            for (int i = startIndex; i <= cols - 1; ++i)
            {
                numbers[x][i] = currentNumber;
            }
        }
    }

    int result = 0;

    for (int x = 0; x < rows; ++x)
    {
        for (int y = 0; y < cols; ++y)
        {
            if (engine[x][y] != '*')
            {
                continue;
            }

            unordered_set<int> adjacentNumbers;
            for (int dir = 0; dir < ALL_DIRS; ++dir)
            {
                int nextX = x + dirs[dir][0];
                int nextY = y + dirs[dir][1];

                if (nextX >= 0 and nextX < rows and nextY >= 0 and nextY < cols and numbers[nextX][nextY] != 0)
                {
                    adjacentNumbers.insert(numbers[nextX][nextY]);
                }
            }

            if ((int)adjacentNumbers.size() == 2)
            {
                int ratio = 1;
                for (int num : adjacentNumbers)
                {
                    ratio *= num;
                }

                result += ratio;
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

    vector<string> engine;

    string row;
    while (getline(fin, row))
    {
        engine.push_back(row);
    }

    fin.close();

    auto t_begin = chrono::high_resolution_clock::now();
    cout << "Result, part 1: " << part1(engine) << endl;
    auto t_end = chrono::high_resolution_clock::now();

    auto part1Time = chrono::duration<double, std::milli>(t_end - t_begin).count();

    t_begin = chrono::high_resolution_clock::now();
    cout << "Result, part 2: " << part2(engine) << endl;
    t_end = chrono::high_resolution_clock::now();

    auto part2Time = chrono::duration<double, std::milli>(t_end - t_begin).count();

    cout << "Completed part 1 in: " << part1Time << " ms" << endl;
    cout << "Completed part 2 in: " << part2Time << " ms" << endl;

    return 0;
}