// LargestPalindromeProduct.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string.h>

bool isPalinDrome(int n)
{
	char decString[100];
	sprintf_s(decString, 100, "%d", n);
	
	int len = strlen(decString);

	for (int i = len / 2; i--; i >= 0)
	{
		if (decString[i] != decString[len - 1 - i])
			return false;
	}
	return true;
}

int findLargestPalindromeProduct3DigitNumbers()
{
	int lastFound = 0;

	for (int i = 999 ; i > 99 ; i--)
	{
		for (int j = 999; j > 99; j--)
		{
			int testNumber = i * j;
			if (testNumber > lastFound)
			{
				if (isPalinDrome(testNumber))
					lastFound = testNumber;
			}
		}
	}
	return lastFound;
}

int main()
{
	printf("highestPalindrome: %d", findLargestPalindromeProduct3DigitNumbers());

    return 0;
}

