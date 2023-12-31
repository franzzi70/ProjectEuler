// CoinSums.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include <set>


const std::vector<int> coinVals({ 1, 2, 5, 10, 20, 50, 100, 200 });
//const std::vector<int> coinVals({ 1, 2, 5, 10 });
std::vector<int> saved(200,0);

int fit(int fitNum, int upperIndex)
{
	int fitSum = 0;
	if (upperIndex < 0)
		return 0;


	for (int i = upperIndex; i >= 0; i--)
	{
		int coinVal = coinVals[i];
		for (int coinSum = coinVal; fitNum - coinSum >= 0; coinSum += coinVal)
		{
			int tryNum = fitNum - coinSum;
			if (tryNum == 0)
			{
				fitSum += 1;
			}
			if (tryNum > 0)
			{
				fitSum += fit(tryNum, i - 1);
			}
		}
	}
	return fitSum;
}


void solve()
{
	//int myints[] = { 1, 2, 5, 10, 20, 50, 100, 200 };
	//std::vector<int> fifth(myints, myints + sizeof(myints) / sizeof(int));

	int count = fit(200, coinVals.size()-1);
	printf("numers of possiblities: %d\n", count);
}

int main()
{
	solve();
	return 0;
}

