// NonAbundantSums.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <list>
#include <assert.h>
#include <bitset>
#include <iostream>


int getDivisorSum(int number)
{
	assert(number > 1);
	int sum = 1;	// init with first factor, find others and add them.

	for (int i = 2; i * i <= number; i++)
	{
		if (number %i == 0)
		{
			sum += i;
			if (number / i != i)
				sum += number / i;
		}
	}
	return sum;
}



using namespace std;

int main()
{
	const unsigned long CHECKUPTO = 28123;
	std::bitset<(unsigned long)(CHECKUPTO + 1)> isAbundand;
	std::list<int> abundantNumbers;

	for (int i = 2 ; i <= CHECKUPTO ; i++)
	{
		int divisorSum = getDivisorSum(i);
		if (divisorSum > i)
		{
			isAbundand[i] = true;
			abundantNumbers.push_back(i);
		}
		
	}

	int sum = 0;

	for (int i = 1; i <= CHECKUPTO; i++)
	{
		bool foundAbPair = false;
		for (auto ab_it = abundantNumbers.begin(); ab_it != abundantNumbers.end(); ab_it++)
		{
			int abNum = *ab_it;
			if (abNum >= i)
				break;

			int diff = i - abNum;
			if (diff > 0)	// actually >= 12 could be used, because 12 is first abundant number
			{
				if (isAbundand[diff])
				{
					foundAbPair = true;
					// cout << i << '\t' << *ab_it << '\t' << diff << endl;
					break;
				}
			}
		}
		if (!foundAbPair)
		{
			// cout << i << endl;
			sum += i;
		}
	}

	cout << sum << endl;

    return 0;
}

