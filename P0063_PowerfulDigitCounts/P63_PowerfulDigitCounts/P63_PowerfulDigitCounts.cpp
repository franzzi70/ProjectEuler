// P63_PowerfulDigitCounts.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <math.h>
#include <time.h>


int digitCount(double num)
{
	return (int)(log(num) / log(10.0)) + 1;
}

void solve()
{
	int count = 0;
	for (double n = 1; n <= 9; n++)
	{
		double num = n;
		int pow = 1;
		while (digitCount(num) == pow)
		{
			count += 1;
			num *= n;
			pow += 1;
		}
	}
	std::cout << "numberCount: " << count << std::endl;
}

int main()
{
	clock_t t = clock();
	solve();
	printf("\nTime taken: %.4fs\n", (float)(clock() - t) / CLOCKS_PER_SEC);
}
