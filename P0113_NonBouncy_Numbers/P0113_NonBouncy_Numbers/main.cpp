// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <iostream>
#include <chrono>
#include <vector>

#define DIGITCOUNT 100
const int g_digitCount = DIGITCOUNT;

std::vector<__int64> g_memoDecr(DIGITCOUNT * 10, -1);
std::vector<__int64> g_memoIncr(DIGITCOUNT * 10, -1);


__int64 numberCountDecr(int digitCount, int position, int prevDigit)
{
    __int64 memValue = g_memoDecr[position * 10 + prevDigit];
    if (memValue != -1)
        return memValue;

    __int64 value = 0;
    if (position == 0)
    {
        value = prevDigit + 1;   // count = all digits <= prev digit.
    }
    else
    {
        for (int i = 0; i <= prevDigit; i++)
        {
            // do not allow leading 0
            if (position == digitCount - 1 && i == 0)
                continue;
            value += numberCountDecr(digitCount, position - 1, i);
        }
    }
    g_memoDecr[position * 10 + prevDigit] = value;
    return value;
}


__int64 numberCountIncr(int position, int prevDigit)
{
    __int64 memValue = g_memoIncr[position * 10 + prevDigit];
    if (memValue != -1)
        return memValue;

    __int64 value = 0;
    if (position == 0)
    {
        value = 10 - prevDigit;   // count = all digits <= prev digit.
    }
    else
    {
        for (int i = prevDigit; i <= 9; i++)
        {
            value += numberCountIncr(position - 1, i);
        }
    }
    g_memoIncr[position * 10 + prevDigit] = value;
    return value;
}

void initMem()
{
    int sz = g_memoDecr.size();
    for (int i = 0; i < sz; i++)
    {
        g_memoDecr[i] = -1;
        g_memoIncr[i] = -1;
    }
}

__int64 solve(int digitCount)
{
    __int64 sum = 0;
    for (int i = 2; i <= digitCount; i++)
    {
        initMem();
        sum += numberCountDecr(i, i - 1, 9);
        sum += numberCountIncr(i - 1, 1);
        sum -= 9;   // remove the 9 numbers that are both increasing and decreasing
    }
    sum += 9;   // 9 one-digit numbers

    return sum;
}

__int64 solve()
{
    return solve(g_digitCount);
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
