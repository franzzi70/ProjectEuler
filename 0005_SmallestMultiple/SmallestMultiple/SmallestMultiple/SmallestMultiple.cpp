// SmallestMultiple.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

bool Divisible1to20(int n)
{
	for (int i = 2; i <= 20; i++)
	{
		if (n % i != 0)
			return false;
	}
	return true;
}

int FindLowestDivisible1to20()
{
	// Loesung ist brute force aber schnell zu implementieren :-)
	// finding KGV (kleinstes gemeinsames vielfaches mit primfaktoren finden und so waere laufzeiteffizienter aber aufwendinger zu implementieren)
	// zahlt sich aber nicht aus bei diesen kleinen Zahlen.
	// sorry for that :-)

	int probeNum = 20;

	while (true)
	{
		if (Divisible1to20(probeNum))
			return probeNum;
		else
		{
			probeNum += 20;
		}
		if (probeNum > 2000000000)
		{
			printf("something went wrong.\n");
			return 0;
		}
	}
}

int main()
{
	printf("LowestNumber divisible by all numer from 1 to 20 is: %d\n", FindLowestDivisible1to20());
    return 0;
}

