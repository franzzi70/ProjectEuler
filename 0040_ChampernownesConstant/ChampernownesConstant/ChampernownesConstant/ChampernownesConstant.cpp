// ChampernownesConstant.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

int getDigitCount(int n)
{
	int tmpN = n;
	int digitCount = 0;
	while (tmpN != 0)
	{
		tmpN /= 10;
		digitCount +=1;
	}
	return digitCount;
}

int getNthDigit(int num, int pos)
{
	int digitBuf[20];
	int tmpN = num;
	int digitCount = 0;
	while (tmpN != 0)
	{
		int digit = tmpN % 10;
		tmpN /= 10;
		digitBuf[digitCount] = digit;
		digitCount += 1;
	}
	return digitBuf[digitCount - pos];
}

int solve()
{
	int resultProd = 1;

	int digitsCount = 0;
	int ixNextToFind = 0;
	int findDigits[7] = { 1, 10, 100, 1000, 10000, 100000, 1000000 };
	int findIx = 0;
	int posToFind = findDigits[findIx];
	for (int num = 1 ; findIx < 7 ; num++)
	{
		int testDigitCount = getDigitCount(num);
		int newDigitsCount = digitsCount + testDigitCount;
		if (newDigitsCount >= posToFind)
		{
			int foundDigit = getNthDigit(num, posToFind - digitsCount);
			resultProd *= foundDigit;
			findIx += 1;
			posToFind = findDigits[findIx];
		}

		digitsCount = newDigitsCount;
	}
	return resultProd;
}

int main()
{

	int solution = solve();

	std::cout << solution << std::endl;

    return 0;
}

