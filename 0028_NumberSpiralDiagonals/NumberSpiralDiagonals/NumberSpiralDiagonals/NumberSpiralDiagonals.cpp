// NumberSpiralDiagonals.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int main()
{
	int num = 1;
	int diagSum1 = num;
	int diagSum2 = num;
	num = 2;	// start position for next round

	for (int n = 1; n <= 500; n++)
	{
		// step 1: go down (2*n -1) numbers
		num += 2 * n - 1;
		diagSum1 += num;

		printf("%d\n", num);

		// step 2: go left 2*n numbers
		num += 2 * n;
		diagSum2 += num;

		printf("%d\n", num);

		// step 3: go up 2*n numbers
		num += 2 * n;
		diagSum1 += num;

		printf("%d\n", num);

		// step 4: go right (2*n) numbers
		num += 2 * n;
		diagSum2 += num;

		printf("%d\n", num);

		// step 4: go right 1 number (start position for next round)
		num += 1;
	}

	printf("diag1sum:%d, diag2sum:%d, diagsum:%d\n", diagSum1, diagSum2, diagSum1 + diagSum2 - 1);
    return 0;
}

