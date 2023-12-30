#include <iostream>
#include <chrono>
#include <fstream>
#include <sstream>
#include <vector>
#include <array>
#include <random>
#include <unordered_map>

#include <Eigen/Dense>

#define endl '\n'

using namespace std;

const double MIN_POSITION = 2e14;
const double MAX_POSITION = 4e14;

const int ITERATIONS = 100;

mt19937 generator(chrono::steady_clock::now().time_since_epoch().count());

struct Stone
{
    Eigen::Vector3d position;
    Eigen::Vector3d velocity;

    Stone(Eigen::Vector3d &_position, Eigen::Vector3d &_velocity) : position(_position), velocity(_velocity) {}
};

int allStones;
vector<Stone> stones;

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

Eigen::Vector3d get_input(string &s)
{
    Eigen::Vector3d result;
    stringstream sin(s);

    for (int i = 0; i < 3; ++i)
    {
        string value;
        sin >> value;

        if (value.back() == ',')
        {
            value.pop_back();
        }

        result[i] = stod(value);
    }

    return result;
}

Eigen::Matrix3d get_coefficient_matrix(const Eigen::Vector3d &velocity)
{
    Eigen::Matrix3d coefficientMatirx;
    coefficientMatirx << 0, -velocity[2], velocity[1], velocity[2], 0, -velocity[0], -velocity[1], velocity[0], 0;

    return coefficientMatirx;
}

int part1()
{
    int result = 0;

    for (int i = 0; i < allStones; ++i)
    {
        for (int j = i + 1; j < allStones; ++j)
        {
            double x1 = stones[i].position[0];
            double y1 = stones[i].position[1];

            double vx1 = stones[i].velocity[0];
            double vy1 = stones[i].velocity[1];

            double x2 = stones[j].position[0];
            double y2 = stones[j].position[1];

            double vx2 = stones[j].velocity[0];
            double vy2 = stones[j].velocity[1];

            double delta = vx1 * vy2 - vy1 * vx2;
            if (delta == 0)
            {
                continue;
            }

            double k1 = (vx2 * (y1 - y2) + vy2 * (x2 - x1)) / delta;
            double k2 = (vx1 * (y1 - y2) + vy1 * (x2 - x1)) / delta;

            if (k1 < 0 or k2 < 0)
            {
                continue;
            }

            double targetX = x2 + vx2 * k2;
            double targetY = y2 + vy2 * k2;

            if (targetX >= MIN_POSITION and targetX <= MAX_POSITION and targetY >= MIN_POSITION and targetY <= MAX_POSITION)
            {
                ++result;
            }
        }
    }

    return result;
}

// Part 2 was done thanks to this guy https://github.com/TSoli/advent-of-code/blob/main/2023/day24b/solution.cpp
// since this whole geometry thing was too complex for me to grasp and implement by myself, I hate geometry :)
int64_t part2()
{
    unordered_map<int64_t, int> counter;

    for (int i = 0; i < 100; ++i)
    {
        array<int, 4> selected;
        for (int j = 0; j < 4; ++j)
        {
            selected[j] = uniform_int_distribution(0, allStones - 1)(generator);
        }

        if (selected[0] == selected[1] or selected[0] == selected[2] or selected[0] == selected[3] or selected[1] == selected[2] or selected[1] == selected[3] or selected[2] == selected[3])
        {
            continue;
        }

        Eigen::MatrixXd equations(6, 6);
        Eigen::VectorXd rhs(6);

        rhs.segment<3>(0) = stones[selected[0]].position.cross(stones[selected[0]].velocity) - stones[selected[1]].position.cross(stones[selected[1]].velocity);
        rhs.segment<3>(3) = stones[selected[2]].position.cross(stones[selected[2]].velocity) - stones[selected[3]].position.cross(stones[selected[3]].velocity);

        equations.block<3, 3>(0, 0) = get_coefficient_matrix(stones[selected[1]].velocity - stones[selected[0]].velocity);
        equations.block<3, 3>(3, 0) = get_coefficient_matrix(stones[selected[3]].velocity - stones[selected[2]].velocity);
        equations.block<3, 3>(0, 3) = get_coefficient_matrix(stones[selected[0]].position - stones[selected[1]].position);
        equations.block<3, 3>(3, 3) = get_coefficient_matrix(stones[selected[2]].position - stones[selected[3]].position);

        Eigen::VectorXd result = equations.inverse() * rhs;

        int64_t positionSum = 0;
        for (int j = 0; j < 3; ++j)
        {
            positionSum += llround(result[j]);
        }

        ++counter[positionSum];
    }

    int64_t result = 0;
    int maxCount = 0;

    for (auto &[value, count] : counter)
    {
        if (count > maxCount)
        {
            count = maxCount;
            result = value;
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
        vector<string> parts = split_string(row, '@');

        Eigen::Vector3d position = get_input(parts[0]);
        Eigen::Vector3d velocity = get_input(parts[1]);

        stones.push_back(Stone(position, velocity));
    }

    allStones = (int)stones.size();

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