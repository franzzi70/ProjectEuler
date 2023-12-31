// QuadraticPrimes.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <set>
#include <iostream>
#include <ctime>

class primes {
public:
	primes()
	{ 
		// by default init with 100 primes
		primes(100);
	};

	primes(int numPrimes)
	{
		addPrimes(numPrimes);
	}


	bool isPrime(int number)
	{
		// check if prime is already stored:
		if (m_primes.count(number) > 0)
			return true;

		if (getHighStoredPrime() > number)
			return false;

		// then try to divide by all stored primes.
		int primCount = m_primes.size();
		int p = 0;

		for (auto p_it = m_primes.begin(); p_it != m_primes.end(); p_it++)
		{
			p = *p_it;
			if (number % p == 0)
			{
				return false;
			}
			if (p*p > number)
				return true;
		}
		
		// now continue with checking
		int testDiv = p;
		while (testDiv * testDiv < number)
		{
			// get next test divisor:
			testDiv =
				testDiv == 0 ?
				2 :
				(testDiv == 2 ?
					3 :
					testDiv + 2
				);

			if (number % testDiv == 0)
				return false;
		}

		return true;

	}

private:

	void addPrimes(int numPrimes)
	{
		if (numPrimes == 0)
			return;

		int hp = getHighStoredPrime();

		if (hp==0 && numPrimes >= 1)
		{
			m_primes.insert(2);
		}

		if (hp < 3 && numPrimes >= 2)
			m_primes.insert(3);

		int insertedPrimes = 2;
		for (int testNum = 5; insertedPrimes < numPrimes; testNum += 2)
		{
			if (isPrime(testNum))
			{
				m_primes.insert(testNum);
				insertedPrimes += 1;
			}
		}
	}


	int getHighStoredPrime()
	{
		if (m_primes.size() == 0)
			return 0;
		else
		{
			return (*(std::prev(m_primes.end())));
		}
	}

	std::set<int> m_primes;

};


int PrimeSeqLen(int num1, int num2, primes *p)
{
	int ix = 0;
	for (ix = 0; p->isPrime(ix * ix + ix * num1 + num2); ix++) {}

	return ix;
}

void test()
{
	clock_t c1 = clock();

	primes *p = new primes(80000);

	clock_t c2 = clock();

	printf("secs:%g, clocksPerSec:%d\n", (double)(c2 - c1) / (double)CLOCKS_PER_SEC, (int)CLOCKS_PER_SEC);
	
	//	bool chk = p->isPrime(337);

	std::cout << "len:" << PrimeSeqLen(1, 41, p) << std::endl;
	std::cout << "len:" << PrimeSeqLen(999, 997, p) << std::endl;

	int maxlen = 0;
	int maxA = 0, maxB = 0;
	int loopcount = 0;

	int i, j;
	for (i = 1000; i >= 0; i--)
	{
		std::cout << i << std::endl;

		for (int sgn = -1; sgn <= 1; sgn += 2)
		{
			for (j = -1000; j <= 1000; j++)
			{
				int a = i * sgn;
				int b = j;

				int currentLen = PrimeSeqLen(a, b, p);
				if (currentLen > maxlen)
				{
					maxlen = currentLen;
					maxA = a;
					maxB = b;
				}
				//std::cout << "len:" << PrimeSeqLen(i, j, p) << std::endl;

				loopcount += 1;
				//if (loopcount >= 1000000)
				//	return;
			}
		}
	}

	std::cout << "A,B,len, A*B: " << maxA << ',' << maxB << ',' << maxlen << ", " << maxA * maxB << std::endl;

/*
	for (int i = 0; i < 81; i++)
	{
		int n = i;
		std::cout << n << " isPrime:" << p->isPrime(n*n - 79 * n + 1601) << std::endl;
	}
*/

}

int main()
{
	test();

    return 0;
}

