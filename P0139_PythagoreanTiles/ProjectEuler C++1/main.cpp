
#include <iostream>
#include <chrono>
#include <map>
#include <cmath>

const bool VERBOSE = false;
const int64_t TRHESH_PERIMETER = 100000000;



//int64_t gcd(uint64_t a, uint64_t b)
//{
//	uint64_t _a = a;
//	uint64_t _b = b;
//
//	while (_a != 0)
//	{
//		uint64_t _tmp = _a;
//		_a = _b % _a;
//		_b = _tmp;
//	}
//	return _b;
//}

// a^2 + (b)^2 = c^2
// (b-a) | c

// (a+b)^2 - 2ab = c^2
// d= b - a
// (a+a+d)^2 - 2a(a+d) = c^2
// (2a+d)^2 - 2a^2 + 2ad = c^2

// (a+b)^2 - 2ab = c^2
// d = b - a
// (b+b-d)^2 - 2b(b-d) = c^2
// (2b-d)^2 - (2b^2 - 2bd) = c^2
// 
// 4b^2 - 4bd + d^2 - 2b^2 -2bd = c^2
// 2b^2 - 2bd + d^2 = c^2
// d^2 * (2b^2/d^2 - 2b/d + 1) = c^2
// 
// => 1.) d | b
//	=> 2.) d <= b
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

int64_t solve()
{
	// init();
	int64_t foundCount = 0;
	int64_t checkCount = 0;

	// first all of the from b = 2a
	// no triangle with sqrt(3*n) = natural number
	// as well no triangle with (2*n) = natural number (no rect with a==b)

	int64_t diff = 3;
	int64_t a = 1;
	int64_t qa = 1;
	int64_t b = 2;
	int64_t qb = 4;

	while (true)
	{
		if (VERBOSE)
			if (a % 1000000 == 0)
				std::cout << "a: " << a << std::endl;

		checkCount += 1;

		int64_t qc = qa + qb;
		int64_t c = (int64_t)(sqrt(qc) + 0.5);
		int64_t per = a + b + c;
		if (per >= TRHESH_PERIMETER)
			break;

		if (c * c == qc)
		{
			if (VERBOSE)
				std::cout << "a: " << a << ", b: " << b << ", c: " << c << std::endl;
			foundCount += (TRHESH_PERIMETER-1) / per;
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

	if (VERBOSE)
	{
		std::cout << "last a: " << a << ", b: " << b << std::endl;
	}
		
	return foundCount;
}

int main()
{

	auto t1 = std::chrono::high_resolution_clock::now();
	int64_t solution = solve();
	auto t2 = std::chrono::high_resolution_clock::now();
	auto microSec = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
	std::cout << "solution: " << solution << std::endl << "duration: " << microSec << " micro seconds (" << ms << " ms)" << std::endl;
	if (microSec > 300'000'000)
	{
		std::cout << "(" << ((float)microSec) / 60'000'000 << " minutes )" << std::endl;
	}
}
