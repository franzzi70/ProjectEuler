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


primes::PrimesSieve g_primes(1'000'000);

int MAXNUM = 1'000'000;


int getNextN(int n)
{
	int nextN = n + 1;
	while (g_primes.isPrime(nextN))
	{
		nextN++;
	}
	return nextN;
}

bool g_validDigits[10] = { false, true, false, true, false, false, false, true, false, true };

int solve()
{
    int THRESHOLD = 1'000'000;

    //RepUnit ru(41);
    //std::cout << "oneCount: " << ru.m_oneCount << std::endl;

    int prefix = THRESHOLD;
    RepUnit ru;

    int lastN = 7;
    int lastDigit = 7;
    int sum = 0;
    int foundCount = 0;

    while (lastN < MAXNUM)
    {
        if (!g_primes.isPrime(lastN))
        {
            ru.calculate(lastN);
            int oneCount = ru.m_oneCount;
            if ((lastN-1) % oneCount == 0)
            {
                std::cout << "n: " << lastN << " oneCount: " << oneCount << std::endl;

                sum += lastN;
                foundCount += 1;

                if (foundCount >= 25)
                    return sum;
			}
        }

        switch (lastDigit)
        {
        case 1:
            lastDigit = 3;
            lastN += 2;
            break;
        case 3:
            lastDigit = 7;
			lastN += 4;
			break;
        case 7:
			lastDigit = 9;
            lastN += 2;
            break;
        case 9:
            lastDigit = 1;
			lastN += 2;
			break;
        }
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
