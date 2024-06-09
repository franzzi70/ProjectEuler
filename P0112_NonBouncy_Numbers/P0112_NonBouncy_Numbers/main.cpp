// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <iostream>
#include <chrono>
#include <vector>

#define DIGITCOUNT 9
const int g_digitCount = DIGITCOUNT;

std::vector<__int64> g_memoDecr (DIGITCOUNT * 10, -1);
std::vector<__int64> g_memoIncr (DIGITCOUNT * 10, -1);


__int64 numberCountDecr(int totalDigits, int position, int prevDigit)
{
    if (position == 0)
    {
        return prevDigit + 1;   // count = all digits <= prev digit.
    }

    __int64 sum = 0;
    for (int i = 0; i <= prevDigit; i++)
    {
        // prevent leading 0
        if (position == totalDigits - 1 && i==0)
            continue;

        __int64 memValue = g_memoDecr[position * 10 + i];
        if (memValue == -1)
        {
            __int64 value = numberCountDecr(totalDigits, position - 1, i);
            sum += value;
            g_memoDecr[position * 10 + i] = value;
        }
        else
            sum += memValue;
    }
    return sum;
}


__int64 numberCountIncr(int totalDigits, int position, int prevDigit)
{
    if (position == 0)
    {
        return (10 - prevDigit);   // count = all digits <= prev digit.
    }

    __int64 sum = 0;
    for (int i = prevDigit; i <= 9; i++)
    {
        // prevent leading 0
        if (position == totalDigits - 1 && i == 0)
            continue;

        __int64 memValue = g_memoIncr[position * 10 + i];
        if (memValue == -1)
        {
            __int64 value = numberCountDecr(totalDigits, position - 1, i);
            sum += value;
            g_memoIncr[position * 10 + i] = value;
        }
        else
            sum += memValue;
    }
    return sum;
}

__int64 solve(int digitCount)
{
    __int64 sum = 0;
    sum += numberCountDecr(digitCount, digitCount - 1, 9);
    sum += numberCountIncr(digitCount, digitCount - 1, 1);

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
