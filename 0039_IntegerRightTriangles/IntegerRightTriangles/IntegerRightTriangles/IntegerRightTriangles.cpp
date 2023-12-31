// IntegerRightTriangles.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

int findMaxTriangles()
{
	int circfMax = 0;
	int maxTriangles = 0;

	for (int circf = 3; circf <= 1000 ; circf++)
	{
		int numTriangles = 0;
		for (int c = 3; c <= circf - 2; c++)
		{
			for (int a = 1;
				a < c && a <= (circf-c)/2;
				a++)
			{
				int b = circf - (c + a);
				if (b != 0 && c > a && c > b)
				{
					if (a * a + b * b == c * c)
					{
						numTriangles += 1;
					}
				}
			}
		}
		if (numTriangles > maxTriangles)
		{
			maxTriangles = numTriangles;
			circfMax = circf;
		}
	}
	return circfMax;
}

int main()
{
	int n = findMaxTriangles();

	std::cout << n << std::endl;

    return 0;
}

