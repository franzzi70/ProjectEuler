// DigitFactorials.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <assert.h>
#include <vector>

std::vector<int> facTable(10,-1);

int fac(int num)
{
	assert(num >= 0);
	if (num == 0)
		return 1;

	int lFac = facTable[num];
	if (lFac != -1)
		return lFac;
	else
	{
		lFac = num * fac(num - 1);
		facTable[num] = lFac;
		return lFac;
	}
}

int checkNum(int n)
{
	int rest = n;
	int facSum = 0;
	while (rest != 0)
	{
		int digit = rest % 10;
		facSum += fac(digit);
		rest /= 10;
	}
	if (facSum == n)
	{
		return n;
	}
	else
		return 0;
}

int main()
{
	//printf("3! = %d\n", fac(3));
	//printf("5! = %d\n", fac(5));
	//printf("9! = %d\n", fac(9));
	//printf("9! *9 = %d\n", fac(9) * 9);
	//printf("checkNum(145)=>%d\n", checkNum(145));
	
	int upperLimit = fac(9) * 9;
	int sum = 0;
	for (int i = 3; i <= upperLimit; i++)
	{
		int result = checkNum(i);
		if (result != 0)
		{
			printf("found one: %d\n",result);
		}

		sum += checkNum(i);
	}
	printf("sum: %d\n", sum);
	return 0;
}

