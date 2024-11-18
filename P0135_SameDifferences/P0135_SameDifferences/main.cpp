
#include <iostream>
#include <chrono>
#include "primes.h"

// x^2 - y^2 - z^2 = n
// x - y = y - z = d
// d > 0
// n > 0

// y = z + d
// x = z + 2d
// x^2 - y^2 - z^2 = n
// (z + 2d)^2 - (z + d)^2 - z^2 = n
// z^2 + 4dz + 4d^2 - z^2 - 2dz - d^2 - z^2 = n
// 2dz + 3d^2 - z^2 = n

// z = y - d
// x = y + d
// x^2 - y^2 - (y - d)^2 = n
// (y + d)^2 - y^2 - (y - d)^2 = n
// y^2 + 2dy + d^2 - y^2 - y^2 + 2dy - d^2 = n
// 4dy - y^2 = n    => use for upper boundarary of y
// 4dy - y^2 - n = 0
// solutions for y:
// y = 2*d + sqrt(4d^2-n) => find y for n and d, y must be integer

// 4dy - y^2 = n
// d = (n+y^2)/4*y

//const int MAX_N = 1'000'000;
const int THRESHOLD_N = 1000000;

__int64 solve()
{
	__int64 stepCount = 0;
	int solutionCount = 0;
	std::vector<int> countArray(THRESHOLD_N + 1);
	int foundCount = 0;
	for (__int64 y = 2; y < THRESHOLD_N; y++)
	{
		// y(4a - y) = n
		// 4ay - y^2 = n
		// 4*a > y
		__int64 minA = y / 4 + 1;
		for (__int64 a = minA; a < y; a++)
		{
			stepCount += 1;
			__int64 n = y * (4 * a - y);
			if (n >= THRESHOLD_N)
			{
				break;
			}
			if (n > 0)
			{
				foundCount += 1;
				countArray[n] += 1;
			}
		}
	}
	for (int i = 0; i < THRESHOLD_N; i++)
	{
		if (countArray[i] == 10)
		{
			solutionCount += 1;
		}
	}
	//std::cout << "stepCount: " << stepCount << std::endl;
	return solutionCount;
}

int main()
{

	auto t1 = std::chrono::high_resolution_clock::now();
	__int64 solution = solve();
	//__int64 solution = solve_notworking();
	auto t2 = std::chrono::high_resolution_clock::now();
	auto microSec = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
	std::cout << "solution: " << solution << std::endl << "duration: " << microSec << " micro seconds (" << ms << " ms)" << std::endl;
	if (microSec > 300'000'000)
	{
		std::cout << "(" << ((float)microSec) / 60'000'000 << " minutes )" << std::endl;
	}
}
