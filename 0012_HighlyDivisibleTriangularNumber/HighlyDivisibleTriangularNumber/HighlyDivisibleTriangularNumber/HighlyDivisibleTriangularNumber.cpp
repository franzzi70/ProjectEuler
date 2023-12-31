// HighlyDivisibleTriangularNumber.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <list>
#include <assert.h>
//#define VERBOSE

class PrimesList
{
public:
	PrimesList(__int64 numberOfPrimes)
	{
		addPrime(2);
		addPrime(3);
		while (true)
		{
			__int64 p = createNextPrime();
			if (primes.size() < numberOfPrimes)
				addPrime(p);
			else
				break;
		}
	}

	__int64 getSumPrimes()
	{
		return sumPrimes;
	}

	std::list<__int64>::iterator getIterator()
	{
		return primes.begin();
	}

	__int64 getNextPrime(std::list<__int64>::iterator &it)
	{
		if (std::next(it) == primes.end())
		{
			addPrimes(1);
		}
		return *(it++);
	}

	void addPrimes(int numPrimes)
	{
		for (int i = 0; i < numPrimes; i++)
		{
			__int64 prime = createNextPrime();
			addPrime(prime);
		}
	}

private:
	__int64 createNextPrime()
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
		assert(false);
		return false;	// should never be reached
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



class DivisorCount
{
public:

	DivisorCount(__int64 n) {}

	DivisorCount(__int64 n, PrimesList *p)
	{
		number = n;
		primes = p;
	}

	int countDivisors()
	{
		std::list<__int64> l = FindPrimeFactors();

		auto it = l.begin();

		__int64 lastNum = 0;
		int groupSize = 1;
		int count = 1;

		for (it; it != l.end(); it++)
		{
			if (it == l.begin())
			{
				groupSize = 1;
			}
			else
			{
				if (*it == lastNum)
				{
					groupSize++;
				}
				else
				{
					count *= (groupSize + 1);
					groupSize = 1;
				}
			}

#ifdef VERBOSE					
			printf("%d\n", (int)*it);
#endif
			lastNum = *it;
		}
		count *= (groupSize + 1);

#ifdef VERBOSE					
		printf("==>%d\n", groupSize + 1);
#endif

		return count;
#ifdef VERBOSE
		printf("=====================\n");
#endif

	}

private:
	std::list<__int64> FindPrimeFactors()
	{
		std::list<__int64>::iterator primes_it = primes->getIterator();
		__int64 reduced = number;

		while (reduced > 1)
		{
			__int64 prime = *primes_it;
			if (reduced % prime == 0)	// found factor
			{
				primeFactors.push_back(prime);

				reduced = reduced / prime;
				if (prime == 1)
					prime = primes->getNextPrime(primes_it);
			}
			else {
				prime = primes->getNextPrime(primes_it);
			}
		}
		return primeFactors;
	}

private:
	std::list<__int64> primeFactors;
	__int64 number;
	PrimesList* primes;

};



__int64 WanderTriangleNumbers()
{

	int initNumPrimes = 100;
	PrimesList* pl = new PrimesList(initNumPrimes);

	__int64 triangleNumber = 0;
	for (int i = 1; /*i<30 */; i++)
	{
		triangleNumber += i;

		DivisorCount* dc = new DivisorCount(triangleNumber, pl);
		int count = dc->countDivisors();
#ifdef VERBOSE
		printf("%d, %d\n", (int)triangleNumber, count);
#endif // VERBOSE

#ifdef VERBOSE
		printf("=====================\n");
		printf("\n");
#endif

		delete dc;

		if (count > 500)
			break;
	}

	return triangleNumber;

}

int main()
{

	printf("result: %d\n", (int)WanderTriangleNumbers());


	return 0;
}

