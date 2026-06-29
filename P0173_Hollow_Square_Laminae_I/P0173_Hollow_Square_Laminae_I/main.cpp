

#include <iostream>
#include <chrono>
#include <cassert>

const int TILECOUNT = 1000000;

#define DBG_PRINT
#undef DBG_PRINT


int area(int innerwidth, int outerwidth)
{
	assert((innerwidth & 1) == (outerwidth & 1));
	return outerwidth * outerwidth - innerwidth * innerwidth;
}

int64_t solve()
{
	int inner = 1;
	int shapecount = 0;
	
	while (true)
	{
		int outer = inner + 2;
		if (area(inner, outer) > TILECOUNT)
			break;

		while (area(inner, outer) <= TILECOUNT)
		{
			shapecount += 1;
			outer += 2;
		}

		inner += 1;
	}

#ifdef DBG_PRINT
	std::cout << "shapecount: " << shapecount << std::endl;
#endif

	//int a1 = area(2, 6);
	//int a2 = area(7, 9);
	//std::cout << "area(2, 6) = " << a1 << std::endl;
	//std::cout << "area(7, 9) = " << a2 << std::endl;

	return shapecount;
}

int main()
{
	auto t1 = std::chrono::high_resolution_clock::now();
	volatile int64_t solution = solve();
	auto t2 = std::chrono::high_resolution_clock::now();
	auto microSec = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
	std::cout << "solution: " << solution << std::endl << "duration: " << microSec << " micro seconds (" << ms << " ms)" << std::endl;
	if (microSec > 300'000'000)
	{
		std::cout << "(" << ((float)microSec) / 60'000'000 << " minutes )" << std::endl;
	}
}
