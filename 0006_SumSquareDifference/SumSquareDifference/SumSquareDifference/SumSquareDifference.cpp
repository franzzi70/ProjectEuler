// SumSquareDifference.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int SumDiffFirstN(int n)
{
	int sum = (1 + n) * n / 2;
	int sumSqr = 0;
	for (int i = 1; i <= n; i++)
	{
		sumSqr += i * i;
	}
	return sum * sum - sumSqr;
}

int main()
{
	printf("diff20: %d\n", SumDiffFirstN(10));
	printf("diff20: %d\n", SumDiffFirstN(100));
	return 0;
}

