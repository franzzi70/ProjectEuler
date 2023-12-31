// DigitFifthPowers.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <ctime>


bool checkPowers5(int num)
{
	int restNum = num;
	int digitPow5Sum = 0;
	while (restNum > 0)
	{
		int digit = restNum % 10;
		restNum = restNum / 10;
		digitPow5Sum += digit * digit * digit * digit * digit;
	}
	if (digitPow5Sum == num)
	{
		printf("found one: %d\n", num);
		return true;
	}
	else
		return false;
}

void solve()
{
	int sum = 0;
	int num;
	for (num = 2; num < 600000; num++)
	{
		if (checkPowers5(num))
		{
			sum += num;
		}
	}
	printf("i:%d\n", num);

	printf("%d\n", sum);
}

int main()
{

	time_t time_start = clock();
	solve();
	time_t time_end = clock();

	printf("Time: %g seconds.\n", (int)((double)(time_end - time_start) / CLOCKS_PER_SEC));

    return 0;
}

