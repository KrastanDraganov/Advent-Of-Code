#include <iostream>
#include <chrono>
#include <fstream>
#include <vector>

#define endl '\n'

using namespace std;

const uint64_t BASE = 31;

const int ALL_DIGITS = 10;
const int ALPHABET_SIZE = 26;

const int MAX_LEN = 55e3 + 3;

uint64_t customPow[MAX_LEN];
uint64_t targetHash[MAX_LEN];

string digitToString[ALL_DIGITS];
uint64_t digitToHash[ALL_DIGITS];

int part1(vector<string> &words)
{
    int result = 0;

    for (string word : words)
    {
        int firstDigit = -1;
        int lastDigit = -1;

        for (char c : word)
        {
            if (c >= '0' and c <= '9')
            {
                if (firstDigit == -1)
                {
                    firstDigit = c - '0';
                }

                lastDigit = c - '0';
            }
        }

        result += firstDigit * 10 + lastDigit;
    }

    return result;
}

int part2(vector<string> &words)
{
    vector<vector<int>> startingLetterToDigit(ALPHABET_SIZE);
    vector<vector<int>> lastLetterToDigit(ALPHABET_SIZE);

    digitToString[1] = "one";
    digitToString[2] = "two";
    digitToString[3] = "three";
    digitToString[4] = "four";
    digitToString[5] = "five";
    digitToString[6] = "six";
    digitToString[7] = "seven";
    digitToString[8] = "eight";
    digitToString[9] = "nine";

    customPow[0] = 1ull;
    for (int i = 1; i < MAX_LEN; ++i)
    {
        customPow[i] = customPow[i - 1] * BASE;
    }

    for (int digit = 1; digit < ALL_DIGITS; ++digit)
    {
        string word = digitToString[digit];

        startingLetterToDigit[word[0] - 'a'].push_back(digit);
        lastLetterToDigit[word.back() - 'a'].push_back(digit);

        for (int i = 0; i < (int)word.size(); ++i)
        {
            digitToHash[digit] += 1ull * (word[i] - 'a' + 1) * customPow[i];
        }
    }

    int result = 0;

    for (string word : words)
    {
        int firstDigit = -1;
        int lastDigit = -1;

        targetHash[0] = 0;
        for (int i = 0; i < (int)word.size(); ++i)
        {
            targetHash[i + 1] = targetHash[i] + 1ull * (word[i] - 'a' + 1) * customPow[i];
        }

        for (int i = 0; i < (int)word.size(); ++i)
        {
            if (word[i] >= '0' and word[i] <= '9')
            {
                if (firstDigit == -1)
                {
                    firstDigit = word[i] - '0';
                }

                lastDigit = word[i] - '0';

                continue;
            }

            int letter = word[i] - 'a';

            if (firstDigit == -1)
            {
                for (int currentDigit : startingLetterToDigit[letter])
                {
                    int digitWordSize = (int)digitToString[currentDigit].size();

                    if (i + digitWordSize <= (int)word.size() and targetHash[i + digitWordSize] - targetHash[i] == digitToHash[currentDigit] * customPow[i])
                    {
                        firstDigit = currentDigit;
                        break;
                    }
                }
            }

            for (int currentDigit : lastLetterToDigit[letter])
            {
                int digitWordSize = (int)digitToString[currentDigit].size();

                if (i + 1 >= digitWordSize and targetHash[i + 1] - targetHash[i + 1 - digitWordSize] == digitToHash[currentDigit] * customPow[i + 1 - digitWordSize])
                {
                    lastDigit = currentDigit;
                    break;
                }
            }
        }

        result += firstDigit * 10 + lastDigit;
    }

    return result;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ifstream fin("input.txt");

    vector<string> words;

    string row;
    while (getline(fin, row))
    {
        words.push_back(row);
    }

    fin.close();

    auto t_begin = chrono::high_resolution_clock::now();
    cout << "Result, part 1: " << part1(words) << endl;
    auto t_end = chrono::high_resolution_clock::now();

    auto part1Time = chrono::duration<double, std::milli>(t_end - t_begin).count();

    t_begin = chrono::high_resolution_clock::now();
    cout << "Result, part 2: " << part2(words) << endl;
    t_end = chrono::high_resolution_clock::now();

    auto part2Time = chrono::duration<double, std::milli>(t_end - t_begin).count();

    cout << "Completed part 1 in: " << part1Time << " ms" << endl;
    cout << "Completed part 2 in: " << part2Time << " ms" << endl;

    return 0;
}