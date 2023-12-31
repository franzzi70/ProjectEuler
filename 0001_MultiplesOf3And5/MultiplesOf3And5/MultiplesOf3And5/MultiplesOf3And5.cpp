// MultiplesOf3And5.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int sumNfoldsLeM(int n, int M)
{
	int count = M / n;
	printf("number of %d folds in %d: %d\n", n, M, count);
	int highest = n * count;
	return ((n + highest) * count) / 2;
}

int sumSum3and5FoldsLtN(int N)
{
	int UpperBoundary = N - 1;
	int threeFoldSum = sumNfoldsLeM(3, UpperBoundary);
	int fiveFoldSum = sumNfoldsLeM(5, UpperBoundary);
	int fifteenFoldSum = sumNfoldsLeM(15, UpperBoundary);
	int result = threeFoldSum + fiveFoldSum - fifteenFoldSum;
	return result;
}
	
int main()
{

	int LtNum = 10;
	printf("3and5FoldSums less than %d = %d\n", LtNum, sumSum3and5FoldsLtN(LtNum));

	LtNum = 1000;
	printf("3and5FoldSums less than %d = %d\n", LtNum, sumSum3and5FoldsLtN(LtNum));
	

	return 0;
}

