// CircularPrimes.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <bitset>
#include <ctime>

std::bitset<1000000> isPrime;


bool checkCircularPrime(int num)
{
	if (!isPrime[num])
		return false;

	int digitCount = 0;
	int rest = num;
	while (rest != 0)
	{
		rest /= 10;
		digitCount += 1;
	}

	int checkNum = num;
	if (digitCount > 1)
	{
		for (int rot = 1; rot < digitCount; rot++)
		{
			int lowDigit = checkNum % 10;
			int factor = 1;
			for (int times = 1; times < digitCount; times++)
				factor *= 10;
			checkNum = lowDigit * factor + checkNum / 10;

			if (!isPrime[checkNum])
				return false;
		}
	}
	return true;
}

void init()
{
	isPrime[2] = true;
	int primeSize = isPrime.size();
	for (int i = 3; i < primeSize ; i+= 2)
	{
		bool divisible = false;
		//check if divisible by any prime <= sqrt (i)
		if (i % 2 == 0)
			divisible = true;
		else
			for (int j = 3; j*j <= i; j+= 2)
			{
				if (i%j == 0)
					divisible = true;
			}
		if (!divisible)
			isPrime[i] = true;
	}
}

int main()
{
	time_t start_time = clock();

	init();


	//checkCircularPrime(197);

	int circularPrimeCount = 0;
	for (int i = 0; i < 1000000; i++)
	{
		if (checkCircularPrime(i))
		{
			printf("found one: %d\n", i);
			circularPrimeCount += 1;
		}
	}

	time_t end_time = clock();

	printf("number of circularPrimes: %d\n", circularPrimeCount);
	printf("time: %g seconds.\n", (double)(end_time - start_time) / CLOCKS_PER_SEC);
    return 0;
}

