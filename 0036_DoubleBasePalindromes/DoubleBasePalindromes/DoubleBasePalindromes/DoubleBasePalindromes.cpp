// DoubleBasePalindromes.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include <algorithm>


std::vector<__int8> decBuffer(6);
std::vector<__int8> binBuffer(20);

bool check(int num)
{

	//for (int i = 0; i < 10; i++)
	if (num % 10 == 0)	// numbers with trailing zeroes are no candidates
		return false;
	if (num % 2 == 0)	// numbers with trailing zeroes are no candidates
		return false;
	int digitCount = 0;
	int rest = num;
	while (rest != 0)
	{
		int digit = rest % 10;
		decBuffer[digitCount] = digit;
		rest = rest / 10;
		digitCount += 1;
	}
	bool isPalindrome = true;
	for (int chk = 0; chk < digitCount / 2; chk++)
	{
		if (decBuffer[chk] != decBuffer[digitCount - 1 - chk])
		{
			isPalindrome = false;
			break;
		}
	}

	if (!isPalindrome)
		return false;

	digitCount = 0;
	rest = num;
	while (rest != 0)
	{
		int digit = rest % 2;
		binBuffer[digitCount] = digit;
		rest = rest / 2;
		digitCount += 1;
	}
	isPalindrome = true;
	for (int chk = 0; chk < digitCount / 2; chk++)
	{
		if (binBuffer[chk] != binBuffer[digitCount - 1 - chk])
		{
			isPalindrome = false;
			break;
		}
	}

	//if (isPalindrome)
	//{
	//	printf("%d isPalindrome\n", num);
	//}
	//else
	//{
	//	printf("%d is no Palindrome\n", num);
	//}

	return isPalindrome;
}

int solve()
{
	int sum = 0;
	for (int i = 1; i < 1000000; i++)
	{
		if (check(i))
			sum += i;
	}
	return sum;
}

int main()
{
	int sum = solve();
	printf("sumOfDoublePalindromes: %d\n", sum);

    return 0;
}

