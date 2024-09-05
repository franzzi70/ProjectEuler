
#include <iostream>
#include <chrono>
#include "primes.h"

#include <vector>


std::vector<std::vector<int>> factorTable;

static __int64 calculate(int p1, int p2)
{
    int p1Rest = p1;
    __int64 accu = 0;
	int lastP2Digit = p2 % 10;
    int tenScale = 1;
    __int64 p2TenScaled = p2;

    while (p1Rest > 0)
	{
		int lastDigit = (10 + p1Rest - ((accu/tenScale)%10) ) % 10; // hier weiter
		int factor = factorTable[lastDigit][lastP2Digit];
        accu += factor * p2TenScaled;

        p1Rest /= 10;
        p2TenScaled *= 10;
		tenScale *= 10;
    }

    return accu;
}


void init()
{
    for (int i = 0; i < 10; ++i)
    {
        factorTable.push_back(std::vector<int>(10,-1));
    }


    for (int i = 0; i < 10; ++i)
    {
        for (int j = 0; j < 10; j++)
        {
            int f = i * j;
            int lastDigit = f % 10;
            int fieldValue = factorTable[lastDigit][i];
            if (fieldValue == -1)
                factorTable[lastDigit][i] = j;
            else
                if (j<fieldValue)
                    factorTable[lastDigit][i] = j;
        }
    }
}

const int MAXPRIME = 1'000'000;
primes::PrimesSieve g_primes(MAXPRIME+100);

__int64 solve()
{
    init();

    __int64 n1 = calculate(5, 7);
    __int64 n2 = calculate(19, 23);

    __int64 sum = 0;
	int p1 = 5;
    while (p1 <= MAXPRIME)
    {
		int p2 = g_primes.nextPrime(p1);
		sum += calculate(p1, p2);

		p1 = p2;
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
