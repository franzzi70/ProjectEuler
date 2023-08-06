// SpecialPythagoreanTriplet.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int getProductOfPyhtTripletWithSum1000()
{
	for (int c = 999; c > 0; c--)
	{
		for (int b = 1000 - c; b > 0; b--)
		{
			int a = 1000 - b - c;
			if (a*a + b*b - c*c == 0
				&& a!= 0 && b!= 0 && c!= 0)
			{
				if (a + b + c == 1000)
				{
					printf("%d, %d, %d\n", a, b, c);
					return (a*b*c);
				}
			}
		}
	}
	return 0;
}

int main()
{
	printf("%d\n", getProductOfPyhtTripletWithSum1000());

    return 0;
}

