// LargestPrimeFactor.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


void getFactors(_int64 n)
{
	__int64 probe = 2;
	__int64 product = n;

	while (true)
	{

		if (probe * probe > n)
		{
			printf("something went wrong. terminating ... \n");
			break;
		}

		if (product % probe == 0)	// found one
		{
			printf("%d\n", (int)probe);
			product = product / probe;
			if (product == 1)
			{
				break; // done
			}
		}
		else
		{
			if (probe == 2)
				probe += 1;
			else
				probe += 2;
		}

	}
}


int main()
{
	__int64 n = 600851475143;
	n = 600851475143;

	getFactors(n);

    return 0;
}

