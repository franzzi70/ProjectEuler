// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <iostream>
#include <chrono>
#include <vector>
#include <bitset>
#include "primes.h"

#define MAX_NUMBER 1E10
const int PRIMESIEVE_HIGH = constexpr(sqrt(MAX_NUMBER) + 1);

primes::PrimesSieve g_sieve(PRIMESIEVE_HIGH+100);
std::vector<std::vector<__int64>>g_digitMap;

void initDigitMap() {
    std::vector<__int64> values{ 0,1,2,3,4,5,6,7,8,9 };
    for (int i = 0; i < 10; i++)
    {
        g_digitMap.push_back(std::vector<__int64>(values));
        for (int j = 0; j < 10; j++)
        {
            values[j] *= 10;
        }
    }
}

bool isPrime(__int64 number)
{
    int testPrime = 2;
    do {
        if (number % testPrime == 0)
            return false;
        testPrime = g_sieve.nextPrime(testPrime);
    } while (
        testPrime <= PRIMESIEVE_HIGH
        );
    return true;
}

__int64 generateNumber(int digit, int replacePos, int replaceDigit)
{
    __int64 number = 0;
    for (int i = 0; i < 10; i++)
    {
        if (i != replacePos)
            number += g_digitMap[i][digit];
        else
            number += g_digitMap[i][replaceDigit];
    }
    return number;
}

__int64 generateNumber(int digit, std::vector<std::pair<int, int>> replacements)
{
    __int64 number = 0;
    std::bitset<10> bits(0);
    int size = (int)replacements.size();
    for (int i = 0; i < size; i++)
    {
        std::pair<int, int> r = replacements[i];
        bits.set(r.first);
        number += g_digitMap[r.first][r.second];
    }

    for (int i = 0; i < 10; i++)
    {
        if (!bits.test(i))
            number += g_digitMap[i][digit];
    }
    return number;
}

__int64 solveForDigit(int digit)
{
    __int64 sum = 0;
    if (digit % 2 == 1)
    {
        for (int i = 0; i < 10; i++)
        {
            for (int j = 0; j < 10; j++)
            {
                __int64 testNumber;
                // prevent 0 being highest digit
                if (i == 9 && j == 0)
                    continue;
                if (j != digit)
                {
                    testNumber = generateNumber(digit, i, j);
                    if (isPrime(testNumber))
                        sum += testNumber;
                }
            }
        }
    }
    if (digit % 2 == 0 && digit != 0)
    {

        std::vector<int> lastTestDigit{ 1,3,7,9 };

        for (int i = 0; i < 4; i++)
        {
            __int64 testNumber;
            testNumber = generateNumber(digit, 0, lastTestDigit[i]);
            if (isPrime(testNumber))
                sum += testNumber;
        }
        if (sum == 0) // no primes found, then replace more
        {
            std::vector<std::pair<int, int>> v(2);

            std::pair<int, int> r;
            for (int i = 0; i < 4; i++)
            {
                //r.first = 0;
                //r.second = lastTestDigit[i];
                v[0] = std::pair<int, int>(0, lastTestDigit[i]);

                for (int i = 1; i < 10; i++)
                {
                    for (int j = 0; j < 10; j++)
                    {
                        __int64 testNumber;
                        // prevent 0 being highest digit
                        if (i == 9 && j == 0)
                            continue;
                        if (j != digit)
                        {
                            v[1] = std::pair<int, int>(i, j);
                            testNumber = generateNumber(digit, v);
                            if (isPrime(testNumber))
                                sum += testNumber;
                        }
                    }
                }
            }
        }
    }

    if (digit == 0)
    {

        std::vector<int> lastTestDigit{ 1,3,7,9 };
        std::vector<std::pair<int, int>> v(2);

        for (int i = 0; i < 4; i++)
        {
            v[0] = std::pair<int, int>(0, lastTestDigit[i]);

            for (int j = 1; j < 9; j++)
            {

                v[1] = std::pair<int, int>(9, j);
                
                __int64 testNumber;
                testNumber = generateNumber(digit, v);
                if (isPrime(testNumber))
                    sum += testNumber;
            }
        }

    }
    return sum;
}

__int64 solve()
{
    initDigitMap();

    __int64 solution = 0;
    for (int i = 0; i < 10; i++)
    {
        solution+= solveForDigit(i);
    }
    return solution;
}

int main()
{

    auto t1 = std::chrono::high_resolution_clock::now();
    __int64 solution = solve();
    auto t2 = std::chrono::high_resolution_clock::now();
    auto microSec = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
    std::cout << "solution: " << solution << std::endl << "duration: " << microSec << " micro seconds (" << ms << " ms)" << std::endl;
}
