
#include <iostream>
#include <chrono>
#include <vector>
#include <algorithm>
#include "primes.h"

primes::PrimesSieve g_primes(10000000);
std::vector<int> g_builderDigits(90,0);

__int64 fac(__int64 num)
{
	_int64 result = 1;
	for (__int64 i = 1; i <= num; i++)
	{
		result *= i;
	}
	return result;
}

__int64 countPrimeSets(std::vector<int>& digits, int position, int prevNumber)
{
	// build prime number starting from position
	// next number found should be greater than prevNumber

	if (position > 4)
	{
		int cmp = g_builderDigits[(9 - position) * 10 + 1];
		if (cmp < prevNumber)
		{
			return 0;
		}
	}

	int digitColumnIx = 0;
	int testNum = 0;
	__int64 cnt = 0;

	for (int i = position; i < 9; i++)
	{
		testNum = testNum * 10 + g_builderDigits[digits[i]];
		if (testNum > prevNumber)
		{
			if (g_primes.isPrimeExt(testNum) && testNum > prevNumber)
			{
				// found prime number
				if (i == 8)
				{
					// last digit
					cnt += 1;
				}
				else
				{
					// continue with next digit
					cnt += countPrimeSets(digits, i + 1, testNum);
				}
			}
		}
	}

	return cnt;
}

void init()
{
	for (int i = 0; i < 10; i++)
	{
		g_builderDigits[i] = i;
	}

	for (int i=10;i<90;i+=10)
	{
		for (int j = 0; j < 10; j++)
		{
			g_builderDigits[i + j] = g_builderDigits[i - 10 + j] * 10;
		}
	}
}

__int64 solve()
{
	init();


	std::vector<int>testSet{ 2,5,4,7,8,9,6,3,1 };

	std::vector<int> digits(9);
	for (int i = 0; i < 9; i++)
	{
		digits[i] = i + 1;
		//digits[i] = testSet[i];
	}

	__int64 cnt = 0;
	do
	{
		cnt += countPrimeSets(digits, 0, 0);
	} while (std::next_permutation(digits.begin(), digits.end()));


	return cnt;
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
