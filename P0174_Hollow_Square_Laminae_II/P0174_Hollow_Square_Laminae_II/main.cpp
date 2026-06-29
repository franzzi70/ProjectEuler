

#include <iostream>
#include <chrono>
#include <cassert>
#include <vector>

const int TILECOUNT = 1000000;

std::vector<uint16_t> typeArr(TILECOUNT + 1, 0);


#define DBG_PRINT
#undef DBG_PRINT


uint64_t area(uint32_t innerwidth, uint32_t outerwidth)
{
	assert((innerwidth & 1) == (outerwidth & 1));
	return (uint64_t) outerwidth * outerwidth - (uint64_t)innerwidth * innerwidth;
}

int64_t solve()
{
	uint64_t sum = 0;
	uint32_t inner = 1;

	while (true)
	{
		uint32_t outer = inner + 2;
		if (area(inner, outer) > TILECOUNT)
			break;

		int64_t a = 0;
		while (true)
		{
			a = area(inner, outer);
			if (a > TILECOUNT)
				break;
			typeArr[a] += 1;
			outer += 2;
		}

		inner += 1;
	}

	for (int32_t i = 1; i <= TILECOUNT; i++)
	{
		if ((typeArr[i] > 0) && (typeArr[i] <= 10))
		{
			sum += 1;
		}
	}

	return sum;
}

int main()
{
	// solution of P173: 1572729

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
