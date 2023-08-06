// P0089_RomanNumerals.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <iostream>
#include <time.h>
#include <vector>
#include <algorithm>
#include <set>


bool checkPairs(std::vector<bool>& c1, std::vector<bool>& c2)
{
    static const std::vector<std::vector<int>> pairs{
        {0,1},
        {0,4},
        {0,9},
        {1,6},
        {2,5},
        {3,6},
        {4,9},
        {6,4},
        {8,1}
    };

    int pairCount = pairs.size();

    for (int i = 0; i < pairCount; i++)
    {
        int n1 = pairs[i][0];
        int n2 = pairs[i][1];
        
        if (
            (!(c1[n1] && c2[n2]))
            &&
            (!(c1[n2] && c2[n1]))
            )
        {
            bool swapped = false;
            // second change: replace 6 with 9 and vice versa
            if (n1 == 6)
            {
                n1 = 9;
                swapped = true;
            }
            else
                if (n1 == 9)
                {
                    n1 = 6;
                    swapped = true;
                }

            if (n2 == 6)
            {
                n2 = 9;
                swapped = true;
            }
            else
                if (n2 == 9)
                {
                    n2 = 6;
                    swapped = true;
                }

            if (!swapped)
                return false;

            if (
                (!(c1[n1] && c2[n2]))
                &&
                (!(c1[n2] && c2[n1]))
                )
                return false;
        }
    }

    return true;
}

bool checkCubes(std::vector<bool>& c1, std::vector<bool>& c2, int combN1, int combN2, std::set<__int64>& foundCombinations)
{
    //if (combN1 != 0)
    //    return false;

    __int64 combKey = (((__int64)((__int32)combN1)) << 32) + (__int64)combN2;
    __int64 combReverseKey = (((__int64)((__int32)combN2)) << 32) + (__int64)combN1;

    if (foundCombinations.count(combKey) == 0)
    {
        if (foundCombinations.count(combReverseKey) == 0)
        {
            if (checkPairs(c1, c2))
            {
                foundCombinations.insert(combKey);
                return true;
            }
        }
    }

    return false;
}

int solve()
{
    const int n = 10;
    const int k = 6;

    int possibleCubesCount = 0;

    std::set<__int64> foundCominbationKeys;
    std::vector<bool> cube1(n);
    std::vector<bool> cube2(n);
    std::fill(cube1.end() - k, cube1.end(), true);
    std::fill(cube2.end() - k, cube2.end(), true);

    int combNumber1 = 0;
    do {
        int combNumber2 = 0;
        do {
            if (checkCubes(cube1, cube2, combNumber1, combNumber2, foundCominbationKeys))
            {
                possibleCubesCount += 1;
            }
            combNumber2 += 1;
        } while (std::next_permutation(cube2.begin(), cube2.end()));
        combNumber1 += 1;
    } while (std::next_permutation(cube1.begin(), cube1.end()));

    return possibleCubesCount;
}

int main()
{
    time_t t1 = clock();
    int solution = solve();
    time_t t2 = clock();
    int ms = (int)(t2 - t1) * 1000 / CLOCKS_PER_SEC;

    std::cout << "solution: " << solution << std::endl << "duration: " << ms << " ms" << std::endl;
}
