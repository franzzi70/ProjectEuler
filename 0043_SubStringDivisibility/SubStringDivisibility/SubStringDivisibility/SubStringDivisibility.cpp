// SubStringDivisibility.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <algorithm>
#include <vector>
#include <iostream>

int v[10] = { 0,1,2,3,4,5,6,7,8,9 };

bool check3 (int startIx, int divisor)
{
	int num = v[startIx] * 100 + v[startIx + 1] * 10 + v[startIx + 2];
	if (num % divisor == 0)
		return true;
	else
		return false;
}

__int64 arrToNum()
{
	__int64 num = 0;
	__int64 factor10 = 1;
	for (int i = 9; i >= 0; i--)
	{
		num += v[i] * factor10;
		factor10 *= 10;
	}
	return num;
}

__int64 solve()
{
	__int64 sum = 0;

	do {
		if (check3(1, 2))
			if (check3(2,3))
				if (check3(3,5))
					if (check3(4,7))
						if (check3(5,11))
							if (check3(6,13))
								if (check3(7, 17))
								{
									sum += arrToNum();
								}
	} while (std::next_permutation(v, v + 10));

	return sum;
}

int main()
{
	std::cout << solve() << std::endl;
    return 0;
}

