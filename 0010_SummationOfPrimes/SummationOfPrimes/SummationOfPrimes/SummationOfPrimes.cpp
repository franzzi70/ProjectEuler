// SummationOfPrimes.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <list>


// TenThousandsAndFirstPrime.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


class primesList
{
public:
	primesList(__int64 primesBelowN)
	{
		addPrime(2);
		addPrime(3);
		while (true)
		{
			__int64 p = getNextPrime();
			if (p < primesBelowN)
				addPrime(p);
			else
				break;
		}
	}

	__int64 getSumPrimes()
	{
		return sumPrimes;
	}

private:
	__int64 getNextPrime()
	{
		for (__int64 probeNum = highestPrime + 2; ; probeNum += 2)
		{
			if (isPrime(probeNum))
			{
				return probeNum;
			}
		}
	}

	bool isPrime(__int64 n)
	{
		for (std::list<__int64>::iterator it = primes.begin();
			it != primes.end();
			it++
			)
		{
			__int64 prime_It = *it;

			if (prime_It * prime_It > n)
				return true;
			if (n % prime_It == 0)
				return false;
		}
	}

	void addPrime(__int64 prime)
	{
		primes.push_back(prime);
		highestPrime = prime;
		sumPrimes += prime;
	}

	__int64 highestPrime = 0;
	__int64 sumPrimes = 0;
	std::list<__int64> primes;
};



int main()
{
	primesList p(2000000);
	__int64 sumPrimes = p.getSumPrimes();

    return 0;
}

