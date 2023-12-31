// PandigitalProducts.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include <algorithm>
#include <iostream>


bool checkNum(int num, std::vector<int>&comb, std::vector<int> reducedDigits)
{

	int combSize = comb.size();
	std::vector<int> checkDigits = reducedDigits;
	for (int i = 0; i < combSize; i++)
	{
		for (int j = 0; j < reducedDigits.size(); j++)
			if (checkDigits[j] == comb[i])
			{
				checkDigits[j] = -1;
				break;
			}
	}
	std::vector<int> save = checkDigits;

	if (combSize)
	{
		do {
			int checkNum = 0;
			for (int i = 0; i < combSize; i++)
			{
				checkNum *= 10;
				checkNum += comb[i];
			}
	
			checkDigits = save;
			if (num % checkNum == 0)
			{
				int quotient = num / checkNum;
				// check for Pandigitality:
				int rest = quotient;
				while (rest > 0) {
					int digit = rest % 10;
					bool found = false;
					rest /= 10;
					for (int find_i = 0; find_i < checkDigits.size(); find_i++)
					{
						if (checkDigits[find_i] == digit)
						{
							checkDigits[find_i] = -1;
							found = true;
							break;
						}
					}
					if (!found)
						return false;
				}

				// if all -1 then found:
				bool allCovered = true;
				for (int i = 0; i < checkDigits.size(); i++)
				{
					if (checkDigits[i] != -1)
					{
						allCovered = false;
					}
				}
				if (allCovered)
					printf("foundOne: %d * %d = %d", checkNum, quotient, num);
				return allCovered;
			}
		} while (std::next_permutation(comb.begin(), comb.end()));
	}
	return false;
}


//int consumeDigit(std::vector<int> &digits, int index)
void getCombinations(std::vector<int> &digits, int depth, std::vector<std::vector<int>> &output, int level = 0, int higherThan = -1, std::vector<int>*workVec = NULL)
{
	int vSize = digits.size();
	int skipped = 0;

	if (depth == level)
	{
		//std::vector<int> newVec(depth, num);
		output.push_back(*workVec);
		return;
	}

	std::vector<int> *v;
	if (level == 0)
		v = new std::vector<int>(depth);
	else
		v = workVec;

	std::vector<int> saveDigits = digits;
	for (int i = 0; i < vSize; i++)
	{
		int num = digits[i];
		if (num != -1)
		{
			if (num > higherThan)
			{
				(*v)[level] = num;
				digits[i] = -1;
				getCombinations(digits, depth, output, level + 1, num, v);
				digits = saveDigits;
			}
		}
	}
}

bool isPandagial(int num)
{
	int rest0 = num;
	//std::vector<int> usedDigits(10, -1);
	std::vector<int> digits = { 1,2,3,4,5,6,7,8,9 };
	int digitCount = 0;
	for (int i = 0; rest0 > 0 ; i++ )
	{
		int digit = rest0 % 10;
		if (digit == 0)
			return 0;
		if (digits[digit-1] == -1)
			return false;		// duplicate digit, cannit be pandagial
		digitCount += 1;
		digits[digit-1] = -1;	// mark unusable digit.
		rest0 /= 10;
	}

	std::vector<int> reducedDigits(9 - digitCount);

	int targetPos = 0;
	for (int i = 0; i < 9; i++)
	{
		if (digits[i] != -1)
		{
			reducedDigits[targetPos] = digits[i];
			targetPos += 1;
		}
	}

	std::vector<int> save = reducedDigits;
	int reducedCount = reducedDigits.size();

	// build factors with rest of digits:
	for (int numSize = 1; numSize < (9 - digitCount) / 2 + 1; numSize++) // n digit combinations
	{
		int testNumber = 0;
		std::vector<int> testDigits(numSize);
		reducedDigits = save;
		std::vector<std::vector<int>> combinations;
		getCombinations(reducedDigits, numSize, combinations);
		
		reducedDigits = save;

		// test all permutations of combinations for multiplicationPandigital
		for (int combIx = 0; combIx < combinations.size(); combIx++)
		{
			if (checkNum(num, combinations[combIx], reducedDigits))
			{
				printf("found one: %d\n", num);
				return true;
			}
		}

		// got test digits now, now try all permutations for division:
	}
	return false;
	
}


int solve()
{
	int sumPandagials = 0;
	for (int i = 1000; i < 100000; i++)
	{
		if (isPandagial(i))
		{
			sumPandagials += i;
		}

	}
	return sumPandagials;
}

int main()
{
	int sumPandagials = solve();
	printf("sumPandagials:%d\n", sumPandagials);

    return 0;
}

