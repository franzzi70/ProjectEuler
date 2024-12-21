
#include <iostream>
#include <chrono>
#include <map>
#include <cmath>


const __int64 TRHESH_PERIMETER = 100000000;
const __int64 QARR_SIZE = TRHESH_PERIMETER / 2;

__int64 qArr[QARR_SIZE];

void init()
{
	qArr[0] = 0;
	unsigned __int64 qNum = 0;
	unsigned __int64 diff = 1;
	for (unsigned __int64 i = 1; i < QARR_SIZE; i++)
	{
		qNum += diff;
		diff += 2;
		qArr[i] = qNum;
	}
}

__int64 gcd(unsigned __int64 a, unsigned __int64 b)
{
	unsigned __int64 _a = a;
	unsigned __int64 _b = b;

	while (_a != 0)
	{
		unsigned __int64 _tmp = _a;
		_a = _b % _a;
		_b = _tmp;
	}
	return _b;
}

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
//	1.) d | a
//		=> 2.) d <= a

__int64 solve()
{
	auto t1 = std::chrono::high_resolution_clock::now();
	init();
	auto t2 = std::chrono::high_resolution_clock::now();
	auto microSec = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
	std::cout << "duration of init() : " << ms << " milliseconds." << std::endl;

	std::cout << "gcd(12,15) " << gcd(12, 15) << std::endl;
	std::cout << "gcd(12,16) " << gcd(12, 16) << std::endl;
	std::cout << "gcd(12,17) " << gcd(12, 17) << std::endl;
	std::cout << "gcd(12,18) " << gcd(12, 18) << std::endl;

	__int64 foundCount = 0;
	__int64 checkCount = 0;

	// first all of the from b = 2a
	// no triangle with sqrt(3*n) = natural number
	// as well no triangle with (2*n) = natural number (no rect with a==b)

	//for (int a = 1; a < QARR_SIZE - 1; a++)
	//{
	//	if (a % 1000 == 0)
	//		std::cout << "a: " << a << std::endl;

	//	__int64 qa = qArr[a];
	//	for (int b = a + 1; b < QARR_SIZE; b++)
	//	{
	//		int d = b - a;
	//		if (d != 1)
	//		{
	//			if (d >= a)
	//				break;
	//		}

	//		checkCount += 1;

	//		__int64 qb = qArr[b];
	//		__int64 qc = qa + qb;
	//		int c = round(sqrt(qc));
	//		if (c * c == qc)
	//		{
	//			//std::cout << "found triangle: a: " << a << " b: " << b << " c: " << c
	//			//	<< " (check count : )" << checkCount << std::endl;
	//			if (c % (b - a) != 0)
	//			{
	//				// std::cout << "does not divide." << std::endl;
	//				continue;
	//			}
	//			foundCount += 1;
	//		}
	//		if (a + b + c >= TRHESH_PERIMETER)
	//		{
	//			std::cout << "break at a: " << a << " b: " << b << " c: " << c << std::endl;
	//			break;
	//		}
	//	}
	//}

	// much too slow suggestion from github copilot:
	__int64 stepCount = 0;
	for (__int64 a = 2; a * 3 < TRHESH_PERIMETER; ++a)
	{
		for (__int64 b = 1; b < a; ++b)
		{
			stepCount += 1;
			if (stepCount % 1000000 == 0)
			{
				std::cout << "step count: " << stepCount << " a: " << a << std::endl;
			}
			if ((a - b) % 2 == 1 && gcd(a, b) == 1) // m and n must be coprime and one must be even, the other odd
			{
				__int64 d = a - b;
				//__int64 t = 2 * a * b;
				__int64 qS = a * a + b * b;
				__int64 c = round(sqrt(qS));
				if (c * c != qS)
					continue;

				__int64 per = a + b + c;
				if (per >= TRHESH_PERIMETER)
					break;

				if (c % d == 0)
				{

					std::cout << "found triangle: a: " << a << " b: " << b << " c: " << c << " per: " << per << std::endl;
					foundCount += (TRHESH_PERIMETER / per);
				}
			}
		}
	}

	std::cout << "step count: " << stepCount << std::endl;
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
