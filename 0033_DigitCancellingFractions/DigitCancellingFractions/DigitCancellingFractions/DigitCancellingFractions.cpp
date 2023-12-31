// DigitCancellingFractions.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <math.h>

int ggt(int a, int b)
{
	if (a == 0)
		return b;
	if (b == 0)
		return a;
	if (a > b)
		return ggt(a-b, b);
	if (b > a)
		return ggt(a, b-a);
}


int kgv(int a, int b)
{
	return (a*b / ggt(a, b));
}

bool check(int i, int j, int ix, int jx)
{
	if (ix != 0 && jx != 0)
	{
		if (i * jx == j *ix && (!(ix * 10 == i && jx * 10 == j)))
		{
			printf("got one: %d / %d => %d / %d\n", i, j, ix, jx);
			return true;
		}
	}
	return false;
}

bool check(int i, int j)
{
	int i10 = i / 10;
	int i01 = i % 10;
	int j10 = j / 10;
	int j01 = j % 10;

	if (i10 == j10)
		if (check(i, j, i01, j01))
			return true;
	if (i01 == j01)
		if (check(i, j, i10, j10))
			return true;
	if (i01 == j10)
		if (check(i, j, i10, j01))
			return true;
	if (i10 == j01)
		if (check(i, j, i01, j10))
			return true;
	
	return false;
}

int main()
{
	//printf("kgv(12,4) = %d\n", kgv(12, 4));
	//printf("kgv(18,6) = %d\n", kgv(18, 6));
	//printf("kgv(18,12) = %d\n", kgv(18, 12));

	int allNumerator = 1;
	int allDenominator = 1;

	for (int i = 10 ; i<100 ; i++)
		for (int j = i + 1; j < 100; j++)
		{
			if (check(i, j))
			{
				allNumerator *= i;
				allDenominator *= j;

				int allGgt = ggt(allNumerator, allDenominator);
				allNumerator /= allGgt;
				allDenominator /= allGgt;
			}
		}
	printf("%d\n", allDenominator);
    return 0;
}

