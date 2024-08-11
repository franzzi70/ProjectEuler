// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <iostream>
#include <chrono>
#include "primes.h"

const __int64 REMAINDER_THRESHOLD = 10'000'000'000;
const int MAXPRIME = 1000000;
primes::PrimesSieve g_primes(MAXPRIME);

__int64 nPowMod(__int64 n, __int64 p, __int64 mod)
{
	__int64 f = 1;
	for (__int64 i = 0; i < p; i++)
	{
        f *= n;
        f %= mod;
	}
	return f;
}

bool testNumber(__int64 n, __int64 p)
{
    __int64 sq = p * p;
    __int64 exp = nPowMod(p-1, n, sq) + nPowMod(p + 1, n, sq);
    __int64 rem = exp % sq;
    if (rem > REMAINDER_THRESHOLD)
    {
        return 1;
    }
    return false;
}

int solve()
{
    __int64 prime = 2;
    __int64 n = 1;
    while (true)
    {
        n += 1;
        __int64 nextPrime = g_primes.nextPrime((int)prime);
        if (nextPrime > MAXPRIME)
        {
			std::cout << "max prime reached" << std::endl;
			return 0;
		}
        if (testNumber(n, nextPrime))
		{
			return (int)n;
		}
        prime = nextPrime;
    }
    return 0;
}

int main()
{
    auto t1 = std::chrono::high_resolution_clock::now();
    int solution = solve();
    auto t2 = std::chrono::high_resolution_clock::now();
    auto microSec = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
    std::cout << "solution: " << solution << std::endl << "duration: " << microSec << " micro seconds (" << ms << " ms)" << std::endl;
}
