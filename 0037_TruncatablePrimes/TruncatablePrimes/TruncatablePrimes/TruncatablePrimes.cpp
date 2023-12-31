// TruncatablePrimes.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "primes.h"

#include <iostream>

void somePrimeTest()
{
	primes* p = new primes(100);

	bool b1 = p->isPrime(41);
	bool b2 = p->isPrime(42);
	std::cout << p->isPrime(3797) << std::endl;

	std::cout << b1 << "," << b2 << std::endl;

	int hp = p->getHighStoredPrime();
	std::cout << hp << std::endl;

	p->addPrimes(1000);
	hp = p->getHighStoredPrime();
	std::cout << hp << std::endl;

	p->addPrimesUpTo(50000);
	hp = p->getHighStoredPrime();
	std::cout << hp << std::endl;

	p->addPrimesUpTo(300000);
	hp = p->getHighStoredPrime();
	std::cout << hp << std::endl;

	p->addPrimesUpTo(600000);
	hp = p->getHighStoredPrime();
	std::cout << hp << std::endl;

	p->addPrimesUpTo(1000000);
	hp = p->getHighStoredPrime();
	std::cout << hp << std::endl;

	std::cout << p->isPrime(3797) << std::endl;
}

// check if prime meets condition: whether it is a truncatable prime.
bool checkPrimesFromLeft(primes& p, int prime)
{
	int testNum = 0;
	int tmpNum = prime;
	
	for (int base = 1; tmpNum > 0 ; base *= 10)
	{
		int digit = tmpNum % 10;
		testNum += base * digit;
		if (!p.isPrime(testNum))
			return false;
		tmpNum /= 10;
	}
	return true;
}

void constructPrimesRigt(std::set<int> &foundPrimes, primes& p, int basePrime)
{
	for (int i = 1; i <= 9; i+=2)
	{
		int testNum = basePrime * 10 + i;
		if (p.isPrime(testNum))
		{
			if (checkPrimesFromLeft(p, testNum))
			{
				foundPrimes.insert(testNum);
			}
			constructPrimesRigt(foundPrimes, p, testNum);
		}
	}
}

void findPrimes(std::set<int> &foundPrimes, primes& p)
{
	int startPrimes[4] = { 2,3,5,7 };
	for (int i = 0;i < 4;i++)
	{
		constructPrimesRigt(foundPrimes, p, startPrimes[i]);
	}
}

int main()
{
	// somePrimeTest();

	primes p(100);
	std::cout << p.isPrime(3797) << std::endl;

	std::set<int> foundPrimes;
	findPrimes(foundPrimes, p);

	int sum = 0;
	for (auto it = foundPrimes.begin(); it != foundPrimes.end(); it++)
		sum += *it;

	std::cout << sum << std::endl;
	return 0;
}

