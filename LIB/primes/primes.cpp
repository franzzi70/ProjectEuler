#include "stdafx.h"
#include "primes.h"



#include <set>

using namespace std;

primes::primes()
{
	// by default init with 100 primes
	primes(100);
};

primes::primes(int numPrimes)
{
	addPrimes(numPrimes);
}

primes::~primes() {};


bool primes::isPrime(__int64 number)
{
	int hp = getHighStoredPrime();
	if (number > hp)
	{
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

	// check if prime is already stored:
	if (m_primes.count(number) > 0)
		return true;
	else
		return false;
}


bool primes::isPrime(int number)
{
	return isPrime((__int64) number);
}


// private:

void primes::addPrimes(int numPrimes)
{
	if (numPrimes == 0)
		return;

	int insertedPrimes = 0;

	int hp = getHighStoredPrime();

	if (hp == 0 && numPrimes >= 1)
	{
		m_primes.insert(2);
		insertedPrimes += 1;
	}

	if (hp < 3 && numPrimes >= 2)
	{
		m_primes.insert(3);
		insertedPrimes += 1;
	}

	auto hintIt = std::prev(m_primes.end());

	for (int testNum = getHighStoredPrime() + 2 ; insertedPrimes < numPrimes; testNum += 2)
	{
		if (isPrime(testNum))
		{
			hintIt = m_primes.insert(hintIt, testNum);
			insertedPrimes += 1;
		}
	}
}

void primes::addPrimesUpTo(int prime)
{
	int hp = getHighStoredPrime();
	if (hp >= prime)
		return;

	if (hp < 2 && prime >= 2)
	{
		m_primes.insert(2);
	}

	if (hp < 3 && prime >= 3)
		m_primes.insert(3);

	auto hintIt = std::prev(m_primes.end());
	for (int testNum = getHighStoredPrime() + 2 ;; testNum += 2)
	{
		if (isPrime(testNum))
		{
			hintIt = m_primes.insert(hintIt, testNum);
			if (testNum >= prime)
				break;
		}
	}
}


int primes::getHighStoredPrime()
{
	if (m_primes.size() == 0)
		return 0;
	else
	{
		return (*(std::prev(m_primes.end())));
	}
}



