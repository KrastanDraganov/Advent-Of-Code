#include <iostream>
#include <chrono>
#include <fstream>
#include <sstream>

#define endl '\n'

using namespace std;

vector<int> times;
vector<int> durations;

int part1()
{
    int result = 1;

    for (int i = 0; i < (int)times.size(); ++i)
    {
        int value = 1;

        while (value * (times[i] - value) <= durations[i])
        {
            ++value;
        }

        int possible = (times[i] / 2 - value + 1) * 2;

        if (times[i] % 2 == 0)
        {
            --possible;
        }

        result *= possible;
    }

    return result;
}

int64_t part2()
{
    string timeString = "";
    string durationString = "";

    for (int i = 0; i < (int)times.size(); ++i)
    {
        timeString += to_string(times[i]);
        durationString += to_string(durations[i]);
    }

    int64_t newTime = stoll(timeString);
    int64_t newDuration = stoll(durationString);

    int64_t l = 1, r = newTime / 2;
    while (l <= r)
    {
        int64_t mid = (l + r) / 2;

        if (mid * (newTime - mid) <= newDuration)
        {
            l = mid + 1;
        }
        else
        {
            r = mid - 1;
        }
    }

    int64_t result = (newTime / 2ll - l + 1ll) * 2ll;

    if (newTime % 2ll == 0ll)
    {
        --result;
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
        stringstream sin(row);

        string type;
        sin >> type;

        int value;
        while (sin >> value)
        {
            if (type[0] == 'T')
            {
                times.push_back(value);
            }
            else
            {
                durations.push_back(value);
            }
        }
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