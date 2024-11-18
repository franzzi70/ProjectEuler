
#include <iostream>
#include <chrono>
#include <map>


const __int64 TRHESH_PERIMETER = 100000000;
const __int64 QARR_SIZE = TRHESH_PERIMETER / 2;

__int64 qArr[QARR_SIZE];

void init()
{
	qArr[0] = 0;
	__int64 qNum = 0;
	__int64 diff = 1;
	for (__int64 i = 1; i < QARR_SIZE; i++)
	{
		qNum += diff;
		diff += 2;
		qArr[i] = qNum;
	}
}

// a^2 + (b)^2 = c^2
// (b-a) | c

// (a+b)^2 - 2ab = c^2
// d= b - a
// (a+a+d)^2 - 2a(a+d) = c^2
// (2a+d)^2 - 2a^2 + 2ad = c^2

// 
// 4*a^2 + 4*a*d + d^2 - 2a^2 - 2ad = c^2
// 2a^2 + 2ad + d^2 = c^2

// d^2*(2a^2/d^2 + 2a/d + 1) = c^2

// c^2/d^2 = 2a^2/d^2 + 2a/d + 1

// 2a^2 + 2ad + d^2 = c^2
// d * (2a^2/d) + 2ad + d^2 = c^2
// d * (2a^2/d + 2a + d) = c^2
// 
// d^2 * (2a^2/d^2 + 2a/d + 1) = c^2
// =>
//	1.) d | 2a
//	2.) d <= 2a

__int64 solve()
{
	init();
	__int64 foundCount = 0;
	__int64 checkCount = 0;

	// first all of the from b = 2a
	// no triangle with sqrt(3*n) = natural number
	// as well no triangle with (2*n) = natural number (no rect with a==b)

	for (int a = 1; a < QARR_SIZE - 1; a++)
	{
		if (a % 1000 == 0)
			std::cout << "a: " << a << std::endl;

		__int64 qa = qArr[a];
		for (int b = a + 1; b < QARR_SIZE; b++)
		{
			int d = b - a;
			if (d != 1)
			{
				if (d >= a)
					break;
			}

			checkCount += 1;

			__int64 qb = qArr[b];
			__int64 qc = qa + qb;
			int c = round(sqrt(qc));
			if (c * c == qc)
			{
				//std::cout << "found triangle: a: " << a << " b: " << b << " c: " << c
				//	<< " (check count : )" << checkCount << std::endl;
				if (c % (b - a) != 0)
				{
					// std::cout << "does not divide." << std::endl;
					continue;
				}
				foundCount += 1;
			}
			if (a + b + c >= TRHESH_PERIMETER)
			{
				std::cout << "break at a: " << a << " b: " << b << " c: " << c << std::endl;
				break;
			}
		}
	}
	return foundCount;
}

int main()
{

	auto t1 = std::chrono::high_resolution_clock::now();
	__int64 solution = solve();
	auto t2 = std::chrono::high_resolution_clock::now();
	auto microSec = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
	std::cout << "solution: " << solution << std::endl << "duration: " << microSec << " micro seconds (" << ms << " ms)" << std::endl;
	if (microSec > 300'000'000)
	{
		std::cout << "(" << ((float)microSec) / 60'000'000 << " minutes )" << std::endl;
	}
}
