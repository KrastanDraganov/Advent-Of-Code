#include <iostream>
#include <chrono>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#define endl '\n'

using namespace std;

struct Bag
{
    int red, green, blue;

    Bag(int _red, int _green, int _blue) : red(_red), green(_green), blue(_blue) {}
};

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

Bag find_bag(string &gameInfo)
{
    Bag result(0, 0, 0);

    vector<string> takes = split_string(gameInfo, ';');

    for (string &take : takes)
    {
        vector<string> cubes = split_string(take, ',');

        for (string &cube : cubes)
        {
            vector<string> cubeInfo = split_string(cube, ' ');
            int count = stoi(cubeInfo[1]);

            if (cubeInfo[2] == "red")
            {
                result.red = max(result.red, count);
            }
            else if (cubeInfo[2] == "blue")
            {
                result.blue = max(result.blue, count);
            }
            else if (cubeInfo[2] == "green")
            {
                result.green = max(result.green, count);
            }
        }
    }

    return result;
}

int part1(vector<string> &games)
{
    int result = 0;

    for (string &game : games)
    {
        vector<string> gameInfo = split_string(game, ':');

        int gameNumber = stoi(split_string(gameInfo[0], ' ')[1]);
        Bag bag = find_bag(gameInfo[1]);

        if (bag.red <= 12 and bag.green <= 13 and bag.blue <= 14)
        {
            result += gameNumber;
        }
    }

    return result;
}

int part2(vector<string> &games)
{
    int result = 0;

    for (string &game : games)
    {
        vector<string> gameInfo = split_string(game, ':');

        int gameNumber = stoi(split_string(gameInfo[0], ' ')[1]);
        Bag bag = find_bag(gameInfo[1]);

        result += bag.red * bag.green * bag.blue;
    }

    return result;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ifstream fin("input.txt");

    vector<string> games;

    string row;
    while (getline(fin, row))
    {
        games.push_back(row);
    }

    fin.close();

    auto t_begin = chrono::high_resolution_clock::now();
    cout << "Result, part 1: " << part1(games) << endl;
    auto t_end = chrono::high_resolution_clock::now();

    auto part1Time = chrono::duration<double, std::milli>(t_end - t_begin).count();

    t_begin = chrono::high_resolution_clock::now();
    cout << "Result, part 2: " << part2(games) << endl;
    t_end = chrono::high_resolution_clock::now();

    auto part2Time = chrono::duration<double, std::milli>(t_end - t_begin).count();

    cout << "Completed part 1 in: " << part1Time << " ms" << endl;
    cout << "Completed part 2 in: " << part2Time << " ms" << endl;

    return 0;
}