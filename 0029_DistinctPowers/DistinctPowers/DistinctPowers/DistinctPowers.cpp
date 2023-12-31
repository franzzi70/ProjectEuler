// DistinctPowers.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <set>
#include <iostream>
#include <vector>
#include <string>
#include <exception>
#include "primes.h"


std::set<std::string> patterns;


const int SIEVESIZE = 1000;
const int PRIMEARRSIZE = 1000;
primes::PrimesSieve sieve(SIEVESIZE);

int primeArray[PRIMEARRSIZE];

void getPrimeFactors(int n, std::vector<int>& factors, int& factorCount, bool checkResult = false)
{
	int rest = n;
	int fCount = 0;
	int primeArrIx = 0;

	while (rest > 1)
	{
		int currentPrime = primeArray[primeArrIx];
		if (rest % currentPrime == 0)
		{
			rest /= currentPrime;
			factors[fCount] = currentPrime;
			fCount += 1;
		}
		else
		{
			primeArrIx += 1;
		}
	}
	factorCount = fCount;

	if (checkResult)
	{
		// -- check result
		int prod = 1;
		for (int i = 0; i < factorCount; i++)
		{
			int f = factors[i];
			prod *= f;
			if (!(sieve.isPrime(f)))
			{
				throw std::exception(std::string("factor is not prime ").append(std::to_string(f)).c_str());
			}
		}
		if (prod != n)
		{
			throw std::exception(std::string("factors do not match number ").append(std::to_string(n)).c_str());
		}
	}

}


void init()
{
	int primeArrIx = 0;
	for (int i = 2; i < SIEVESIZE; i++)
	{
		if (primeArrIx >= PRIMEARRSIZE)
			break;

		if (sieve.isPrime(i))
		{
			primeArray[primeArrIx] = i;
			primeArrIx += 1;
		}
	}
}

std::string getStringForNum(int a, int exp)
{
	static std::vector<int> primefactors(10);
	std::string pattern = "";

	int factorCount;

	getPrimeFactors(a, primefactors, factorCount);
	int nextIx = 0;

	while (nextIx < factorCount)
	{
		int f = primefactors[nextIx];
		int groupSize = 1;
		while (nextIx < factorCount - 1)
		{
			if (primefactors[nextIx + 1] == f)
			{
				groupSize += 1;
				nextIx += 1;
			}
			else
				break;
		}
		pattern.append(std::to_string(f))
			.append(1, '^')
			.append(std::to_string(exp * groupSize))
			.append(1, ';');
		nextIx += 1;
	}
	return pattern;
}

int solve(int X, int Y)
{
	init();
	patterns.clear();
	int termCount = 0;

	for (int a = 2; a <= X; a++)
	{
		for (int b = 2; b <= Y; b++)
		{
			std::string str = getStringForNum(a, b);
			if (patterns.count(str) == 0)
			{
				//std::cout << str << std::endl;

				patterns.insert(str);
				termCount += 1;
			}
		}
	}

	return termCount;
}


int main()
{
	time_t t1 = clock();
	int solution = solve(100, 100);
	//int solution = solve(5, 5);
	time_t t2 = clock();
	int ms = (int)(t2 - t1) * 1000 / CLOCKS_PER_SEC;

	std::cout << "solution: " << solution << std::endl << "duration: " << ms << " ms" << std::endl;
}

