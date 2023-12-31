// TenThousandsAndFirstPrime.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


class primesArray
{
public:
	primesArray()
	{
		primes[0] = 2;
		primes[1] = 3;
		size = 2;
		while (size < 10001)
		{
			findNextPrime();
		}
	}
private:
	void findNextPrime()
	{
		if (size >= 10000)
			return;

		__int64 lastFoundPrime = primes[size - 1];
		for (__int64 probeNum = lastFoundPrime + 2; ; probeNum += 2)
		{
			if (isPrime(probeNum))
			{
				primes[size] = probeNum;
				size += 1;
			}
		}
	}

	bool isPrime(__int64 n)
	{
		for (int i = 0; ; i++)
		{
			__int64 prime_I = primes[i];
			if (prime_I * prime_I > n)
				return true;
			if (n % prime_I == 0)
				return false;
		}
	}

	__int64 primes[20002];
	int size = 0;
};

primesArray foundPrimes;

int main()
{
	primesArray a;

    return 0;
}

