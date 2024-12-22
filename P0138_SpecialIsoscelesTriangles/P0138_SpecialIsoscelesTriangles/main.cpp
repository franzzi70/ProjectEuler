
#include <iostream>
#include <chrono>
// #include "MyBigInteger.h"
#include <intrin.h>
#include <cstdint>

#pragma intrinsic(_mul128)

// ((b/2) ^ 2 + (b+1)^2 = c^2
// ((b/2) ^ 2 + (b-1)^2 = c^2

 


class Int128
{
public:

	Int128()
	{
		low = 0;
		high = 0;
	}

	Int128(unsigned __int64 low, unsigned __int64 high)
	{
		this->low = low;
		this->high = high;
	}

	
	unsigned __int64 low;
	unsigned __int64 high;

	unsigned __int64 mul128(unsigned __int64 n1, unsigned __int64 n2)
	{
		low = _umul128(n1, n2, &high);
		return low;
	}

	Int128 operator + (const Int128& other)
	{
		Int128 result;
		result.low = low + other.low;
		result.high = high + other.high;
		if (result.low < low)
		{
			result.high += 1;
		}
		return result;
	}

	bool operator == (const Int128& other)
	{
		return low == other.low && high == other.high;
	}

	double sqrt()
	{
		return std::sqrt((double)*this);
	}

	operator double()
	{
		return ((unsigned __int64) low ) + high * ((double)UINT64_C(0x100000000)) * ((double)UINT64_C(0x100000000));
	}
};

unsigned __int64 solve()
{

	unsigned __int64 bhalf = 0;
	unsigned __int64 b = 0;
	bool done = false;
	int foundCount = 0;
	unsigned __int64 lSum = 0;
	Int128 hSqr;
	Int128 bhalfSqr;
	Int128 lSqr;
	Int128 LSqr;
	unsigned __int64 lastB = 1;

	while (!done)
	{
		if (b % 10000000 == unsigned __int64(0))
		{
			std::cout << "b: " << b << std::endl;
		}

		bhalf += 1;
		b += 2;

		bool found = false;
		for (__int32 alt = -1; alt <= 1; alt += 2)
		{
			unsigned __int64 h = b + unsigned __int64(alt);

			if (b == unsigned __int64(16))
				std::cout << "b: " << b << std::endl;
			if (b == unsigned __int64(272))
			{
				std::cout << "b: " << b << std::endl;
			}

			//unsigned __int64 lSqr = h * (unsigned __int64)h + bhalf * (unsigned __int64)bhalf;
			(void)hSqr.mul128(h,h);
			(void)bhalfSqr.mul128(bhalf,bhalf);
			lSqr = hSqr + bhalfSqr;
			//unsigned __int64 lSqr = h * h + bhalf * bhalf;
			unsigned __int64 L = (unsigned __int64)round(lSqr.sqrt());
			LSqr.mul128(L, L);
			if (LSqr == lSqr)
			{
				found = true;
				std::cout << "b: " << b << ", L: " << L << ", h: " << h << std::endl;
				foundCount += 1;
				lSum += L;
				if (foundCount == 12)
				{
					done = true;
					break;
				}
			}
		}
		if (found)
		{
			double f;
			//--double f = ((double)b) / double(lastB) - 1E-6;
			if (b >= 272)
			{
				if (b==4896)
					f = 17.94117;
				else
					if (b==87840)
						f=17.9443;
					else if (b==1576240)
						f = 17.94425;
					else
						f=17.94426;

				bhalf = (unsigned __int64)(bhalf * f);
				b = bhalf * 2;
			}

			lastB = b;
		}
		//b = (unsigned __int64) (b * 1.118034);if 
		//b = (unsigned __int64) (b * 17.944271);
		//break;
	}
	return lSum;
}

void test()
{
	unsigned __int64 i1 = 1000000000000;
	Int128 im1;
	im1.mul128(i1, i1);
	double d1 = (double)i1 * (double)i1;
	double d2 = (double)im1;
	unsigned __int64 sq = (unsigned __int64)round(im1.sqrt());
}

int main()
{
	//test();
	//return 0;

	auto t1 = std::chrono::high_resolution_clock::now();
	unsigned __int64 solution = solve();
	auto t2 = std::chrono::high_resolution_clock::now();
	auto microSec = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
	std::cout << "solution: " << solution << std::endl << "duration: " << microSec << " micro seconds (" << ms << " ms)" << std::endl;
	return 0;
}
