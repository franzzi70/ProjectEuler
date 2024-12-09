
#include <iostream>
#include <chrono>
#include "MyBigInteger.h"

// ((b/2) ^ 2 + (b+1)^2 = c^2
// ((b/2) ^ 2 + (b-1)^2 = c^2

 
 typedef MyBigInteger BigIntType;

__int64 solve()
{

	BigIntType bhalf = 0;
	BigIntType b = 0;
	bool done = false;
	int foundCount = 0;
	__int64 lSum = 0;
	while (!done)
	{
		if (b % 10000000 == BigIntType(0))
		{
			std::cout << "b: " << b << std::endl;
		}

		bhalf += BigIntType(1);
		b += 2;
		for (__int32 alt = -1; alt <= 1; alt += 2)
		{
			BigIntType h = b + BigIntType(alt);

			if (b == BigIntType(16))
				std::cout << "b: " << b << std::endl;
			if (b == BigIntType(272))
			{
				std::cout << "b: " << b << std::endl;
			}

			//__int64 lSqr = h * (__int64)h + bhalf * (__int64)bhalf;
			BigIntType lSqr = h * h + bhalf * bhalf;
			__int64 L = (int)round(sqrt(lSqr));
			if (L * L == lSqr)
			{
				std::cout << "b: " << (std::string)b << ", L: " << L << ", h: " << (std::string)h << std::endl;
				foundCount += 1;
				lSum += L;
				if (foundCount == 6)
				{
					done = true;
					break;
				}
			}
		}
	}
	return lSum;
}


int main()
{
	auto t1 = std::chrono::high_resolution_clock::now();
	__int64 solution = solve();
	auto t2 = std::chrono::high_resolution_clock::now();
	auto microSec = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
	std::cout << "solution: " << solution << std::endl << "duration: " << microSec << " micro seconds (" << ms << " ms)" << std::endl;
	return 0;
}
