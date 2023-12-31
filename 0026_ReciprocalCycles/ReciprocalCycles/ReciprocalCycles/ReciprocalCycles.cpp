// ReciprocalCycles.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <iostream>
#include <ctime>

int getReciprocalCycleLength(int divisor)
{
	int dividend = 1;
	std::basic_string<int> a;
	std::basic_string<int> digits;
	a.push_back(1);
	int cycleLen = 0;
	
	while (cycleLen == 0)
	{
		if (0 == (dividend % divisor))
			return 0;	// no cycle

		int aLen = a.length();
		// check for cycle
		if (aLen > 1)
		{
			for (int i = 0; i < aLen-1; i++)
			{
				if (a[aLen - i - 2]==dividend)
				{
					// found cycle
					cycleLen = i + 1;
					break;
				}
			}
		}
		digits.push_back(dividend / divisor);
		dividend = (dividend % divisor) * 10;
		a.push_back(dividend);
	}

	//for (auto it_dig = digits.begin(); it_dig != digits.end(); it_dig++)
	//	printf("%d", *it_dig);
	//printf("\n");

	return cycleLen;
}

	
int main()
{
	int start = clock();

	int MaxLen = 0;
	int numWithMaxLen = 0;

	for (int i = 2; i < 999; i++)
	{
		int cycLen = getReciprocalCycleLength(i);
		if (cycLen > MaxLen)
		{
			numWithMaxLen = i;
			MaxLen = cycLen;
		}
	}

	printf("%d: %d\n", numWithMaxLen, MaxLen);

	int ende = clock();
	std::cout << (ende - start) / double(CLOCKS_PER_SEC) << " s\n";

    return 0;
}

