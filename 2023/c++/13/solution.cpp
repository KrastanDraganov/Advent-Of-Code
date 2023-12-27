#include <iostream>
#include <chrono>
#include <fstream>
#include <sstream>
#include <vector>

#define endl '\n'

using namespace std;

const int MAX_PATTERNS = 103;

const int HORIZONTAL = 0;
const int VERTICAL = 1;

vector<vector<string>> patterns;

pair<int, int> savedPatternInfo[MAX_PATTERNS];
char savedPatternOrientation[MAX_PATTERNS];

int find_reflection(int orientation, vector<string> &lines, int index)
{
    int allLines = (int)lines.size();

    for (int reflectionSize = allLines / 2; reflectionSize >= 1; --reflectionSize)
    {
        vector<int> startLines = {0, allLines - 2 * reflectionSize};
        for (int startLine : startLines)
        {
            if (savedPatternOrientation[index] == orientation and savedPatternInfo[index].first == startLine and savedPatternInfo[index].second == reflectionSize)
            {
                continue;
            }

            bool same = true;

            for (int i = startLine, j = startLine + 2 * reflectionSize - 1; i < j; ++i, --j)
            {
                if (lines[i] != lines[j])
                {
                    same = false;
                    break;
                }
            }

            if (same)
            {
                savedPatternInfo[index] = {startLine, reflectionSize};
                savedPatternOrientation[index] = orientation;

                return startLine + reflectionSize;
            }
        }
    }

    return 0;
}

int find_horizontal_line(vector<string> &rows, int index)
{
    return find_reflection(HORIZONTAL, rows, index);
}

int find_vertical_line(vector<string> &pattern, int index)
{
    int allRows = (int)pattern.size();
    int allCols = (int)pattern[0].size();

    vector<string> cols(allCols);
    for (int col = 0; col < allCols; ++col)
    {
        for (int row = 0; row < allRows; ++row)
        {
            cols[col] += pattern[row][col];
        }
    }

    return find_reflection(VERTICAL, cols, index);
}

int part1()
{
    int result = 0;

    for (int i = 0; i < (int)patterns.size(); ++i)
    {
        int horizontalLine = find_horizontal_line(patterns[i], i);

        if (horizontalLine == 0)
        {
            result += find_vertical_line(patterns[i], i);
        }
        else
        {
            result += 100 * horizontalLine;
        }
    }

    return result;
}

int part2()
{
    int result = 0;

    for (int i = 0; i < (int)patterns.size(); ++i)
    {
        vector<string> &pattern = patterns[i];
        bool foundReflection = false;

        for (int row = 0; row < (int)pattern.size() and !foundReflection; ++row)
        {
            for (int col = 0; col < (int)pattern[0].size(); ++col)
            {
                int oldValue = pattern[row][col];
                pattern[row][col] = (oldValue == '.') ? '#' : '.';

                int horizontalLine = find_horizontal_line(pattern, i);
                if (horizontalLine != 0)
                {
                    // cout << endl;
                    // for (int i = 0; i < (int)pattern.size(); ++i)
                    // {
                    //     for (int j = 0; j < (int)pattern[0].size(); ++j)
                    //     {
                    //         cout << pattern[i][j];
                    //     }
                    //     cout << endl;
                    // }
                    // cout << "SHREK --> " << horizontalLine << endl;

                    result += 100 * horizontalLine;
                    foundReflection = true;
                    break;
                }

                int verticalLine = find_vertical_line(pattern, i);
                if (verticalLine != 0)
                {
                    result += verticalLine;
                    foundReflection = true;
                    break;
                }

                pattern[row][col] = oldValue;
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
        vector<string> pattern;

        do
        {
            pattern.push_back(row);
        } while (getline(fin, row) and row != "");

        patterns.push_back(pattern);
    }

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