// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <iostream>
#include <chrono>
#include <vector>
#include "primes.h"

class RepUnit {
public:
    RepUnit()
    {
        m_oneCount = 0;
    }
    RepUnit(int n)
    {
        calculate(n);
    }
    void calculate(int n)
    {

        m_oneCount = 1;
        int lastDigit = n % 10;
        int factorTableIndex = 0;

        switch (lastDigit) {
        case 1: factorTableIndex = 0; break;
        case 3: factorTableIndex = 1; break;
        case 7: factorTableIndex = 2; break;
        case 9: factorTableIndex = 3; break;
        };


        int diff = 1;
        int carry = 0;

        while (true)
        {
            int factor = factorTable[factorTableIndex][diff];
            carry += factor * n;
            carry /= 10;
            if (carry == 0)
            {
                //std::cout << n << ": " << m_oneCount << std::endl;
                break;
            }
            lastDigit = carry % 10;
            diff = 11 - lastDigit;
            if (diff >= 10)
            {
                diff -= 10;
            }
            m_oneCount += 1;
        }
    }
    int m_oneCount;
    static std::vector<std::vector<int>> factorTable;
};

std::vector<std::vector<int>> RepUnit::factorTable{
    {0,1,2,3,4,5,6,7,8,9},  // 1
    {0,7,4,1,8,5,2,9,6,3},  // 3
    {0,3,6,9,2,5,8,1,4,7},  // 7
    {0,9,8,7,6,5,4,3,2,1}   // 9
};


int MAXNUM = 100'000;
primes::PrimesSieve g_primes(MAXNUM + 100);

int BIGNUMBERONECOUNT = 1'000'000'000;

bool checkFactors(int n)
{
    int rest = n;
	while (rest > 1)
	{
		if (rest % 2 == 0)
		{
			rest /= 2;
		}
		else if (rest % 5 == 0)
		{
			rest /= 5;
		}
		else
		{
			return false;
		}
	}
    return true;
}

__int64 solve()
{
    __int64 foundCount = 0;
    __int64 sum = 2+3+5;

    RepUnit ru;

    for (int p = 7; p < MAXNUM; p = g_primes.nextPrime(p))
    {
        ru.calculate(p);
		int oneCount = ru.m_oneCount;
        if (!checkFactors(oneCount))
        {
			sum += p;
			foundCount += 1;
        }
        else
        {
            std::cout << p << std::endl;
        }
    }
    return sum;
}

int main()
{

    auto t1 = std::chrono::high_resolution_clock::now();
    __int64 solution = solve();
    auto t2 = std::chrono::high_resolution_clock::now();
    auto microSec = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
    std::cout << "solution: " << solution << std::endl << "duration: " << microSec << " micro seconds (" << ms << " ms)" << std::endl;
    if (microSec > 300'000'000)
    {
        std::cout << "(" << ((float)microSec) / 60'000'000 << " minutes )" << std::endl;
    }
}
