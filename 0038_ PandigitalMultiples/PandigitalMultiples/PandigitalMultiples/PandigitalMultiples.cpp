// PandigitalMultiples.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <bitset>
#include <iostream>


bool check9Pand(__int64 num)
{
	std::bitset<10> digits;

	__int64 tmpNum = num;
	int numDigits = 0;
	while (tmpNum != 0)
	{
		int digit = tmpNum % 10;

		if (digit == 0)	// 0 no allowed:
			return false;

		if (digits[digit])	// digit already occured
			return false;
		digits.set(digit);

		tmpNum /= 10;
		numDigits += 1;

	}
	if (numDigits != 9)
		return false;

	return true;
}

int getDigitCount(int n)
{
	int tmpNum = n;
	int digitCount = 0;
	while (tmpNum != 0)
	{
		tmpNum /= 10;
		digitCount++;
	}
	return digitCount;
}

__int64 tryPand(int num)
{
	__int64 testNum = num;
	int digitCount = getDigitCount(num);

	for (int n = 2; digitCount < 9; n++)
	{
		int numTimesN = num * n;
		int numTimesNdigitCount = getDigitCount(numTimesN);

		for (int i = 0; i < numTimesNdigitCount; i++)
		{
			testNum *= 10;
		}
		testNum += numTimesN;
		digitCount += numTimesNdigitCount;

		if (digitCount == 9)
		{
			if (check9Pand(testNum))
				return testNum;
		}
	}
	return 0;
}

__int64 findHighestPandigital()
{
	__int64 currentHighest = 0;
	for (int i = 1; i <= 9999; i++)
	{
		__int64 pand = tryPand(i);
		if (pand > currentHighest)
			currentHighest = pand;
	}
	return currentHighest;
}

int main()
{
	__int64 highestPand = findHighestPandigital();

	//std::cout << check9Pand(123654789) << std::endl;
	//std::cout << check9Pand(123654289) << std::endl;
	//std::cout << check9Pand(12365478) << std::endl;

	std::cout << highestPand << std::endl;

    return 0;
}

