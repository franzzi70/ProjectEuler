// AmicableNumbers.cpp : Defines the entry point for the console application.
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



class DivisorHelper
{
public:

	DivisorHelper(__int64 n) {}

	DivisorHelper(__int64 n, PrimesList *p)
	{
		number = n;
		primes = p;
	}

	class factorInfo {
	public:
		factorInfo(int f, int c)
		{
			factor = f;
			count = c;
		}
		int factor;
		int count;
	};

	int SumOfDivisors()
	{
		std::list<__int64> l = FindPrimeFactors();
		std::list<factorInfo> factorsInfo;
		std::list<int> divisors;

		// std::list<__int64>::iterator it;

		__int64 lastNum = 0;
		int groupSize = 1;

		for (auto it = l.begin(); it != l.end(); it++)
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
					factorsInfo.push_back(factorInfo((int)lastNum, groupSize));
					groupSize = 1;
				}
			}
			lastNum = *it;
		}
		factorsInfo.push_back(factorInfo((int)lastNum, groupSize));

		// now expand all divisors:
		divisors.push_back(1);
		for (auto inf_it = factorsInfo.begin(); inf_it != factorsInfo.end(); inf_it++)
		{
			int cnt = inf_it->count;
			int fac = inf_it->factor;
			for (auto div_it = divisors.begin(); div_it != divisors.end(); div_it++)
			{
				int mult = fac;
				for (int i = 0; i < cnt; i++, mult *= fac)
				{
					int newDivisor = mult * (*div_it);
					if (newDivisor != number)	// do not treat the number itself as divisor.
						divisors.push_front(newDivisor);
				}
			}
		}

		int divisorSum = 0;
		for (auto it = divisors.begin(); it != divisors.end(); it++)
		{
			divisorSum += *it;
		}

		return divisorSum;
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



void Solve()
{
#define BOUNDARY 10000

	PrimesList p(BOUNDARY);
	int amicables[BOUNDARY];
	for (int i = 0; i < BOUNDARY; i++)
	{
		amicables[i] = 0;
	}


	int AmicableSum = 0;
	for (int i = 1; i < 10000; i++)
	{

		if (amicables[i] == 0)
		{
			DivisorHelper dh(i, &p);
			int divisorSum = dh.SumOfDivisors();
			if (divisorSum < BOUNDARY && divisorSum != i)
			{
				DivisorHelper dh2(divisorSum, &p);
				int sum2 = dh2.SumOfDivisors();
				if (sum2 == i)
				{
					amicables[i] = divisorSum;
					amicables[divisorSum] = i;
					printf("Found pair of amicabls: %d <=> %d\n", i, divisorSum);
					AmicableSum += i;
					AmicableSum += divisorSum;
				}
			}
		}
	}
	printf("sum of found amicables: %d\n", AmicableSum);
}

void test()
{
	PrimesList p(10000);

	DivisorHelper dh(220, &p);
	int divisorSum = dh.SumOfDivisors();
	printf("DivisorSum of %d = %d\n", 220, divisorSum);

	DivisorHelper dh2(284, &p);
	divisorSum = dh2.SumOfDivisors();
	printf("DivisorSum of %d = %d\n", 284, divisorSum);

}

int main()
{
	// test();

	Solve();


	
	return 0;
}

