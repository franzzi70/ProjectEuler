
#include <iostream>
#include <chrono>
#include <map>

const bool VERBOSE = false;
const __int64 TRHESH_PERIMETER = 100000000;



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
//	1.) d | 2a => d | a
//	2.) d <= 2a
//	3.) d | b
//	4.) d == 1	-- for principal solutions

__int64 solve()
{
	// init();
	__int64 foundCount = 0;
	__int64 checkCount = 0;

	// first all of the from b = 2a
	// no triangle with sqrt(3*n) = natural number
	// as well no triangle with (2*n) = natural number (no rect with a==b)

	// try to get better than 364 ms.


	__int64 diff = 3;
	__int64 a = 1;
	__int64 qa = 1;
	__int64 b = 2;
	__int64 qb = 4;

	while (true)
	{
		if (VERBOSE)
			if (a % 1000000 == 0)
				std::cout << "a: " << a << std::endl;

		checkCount += 1;

		__int64 qc = qa + qb;
		__int64 c = (__int64)(sqrt(qc) + 0.5);
		if (c * c == qc)
		{
			if (VERBOSE)
				std::cout << "a: " << a << ", b: " << b << ", c: " << c << std::endl;

			__int64 per = a + b + c;
			if (per <= TRHESH_PERIMETER)
				foundCount += (TRHESH_PERIMETER-1) / per;
			else
				break;
		}

		if (VERBOSE)
			if (checkCount % 1000000 == 0)
				std::cout << "checkCount: " << checkCount << ", foundCount:" << foundCount << std::endl;
		
		a += 1;
		b += 1;
		qa = qb;
		diff += 2;
		qb += diff;

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
