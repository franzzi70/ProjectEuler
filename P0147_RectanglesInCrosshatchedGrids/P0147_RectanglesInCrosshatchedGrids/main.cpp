
#include <iostream>
#include <chrono>

int getCount(int x, int y)
{
	int count = 0;
	for (int h = 1; h <= y; h++)
	{
		for (int w = 1; w <= x; w++)
		{
			count += (x - w + 1) * (y - h + 1);
		}
	}

	return count;
}

__int64 solve()
{
	int c1 = getCount(3, 2);

	return 0;
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
