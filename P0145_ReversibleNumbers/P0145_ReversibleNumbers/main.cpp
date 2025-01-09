
#include <iostream>
#include <chrono>
#include <string>

int reverseNum(int n)
{
	int reversedNum = 0;
	int r = n;
	while (r > 0)
	{
		int digit = r % 10;
		reversedNum = reversedNum * 10 + digit;
		r /= 10;
	}
	return reversedNum;
}


void test() // check requriement
{
	int reversableCount = 0;

	for (int i = 1; i < 1'000'000'000; i++)
	{
		if (i % 1'000'000 == 0)
			std::cout << i << std::endl;
		if (i % 10 == 0)
			continue;
		int i_reverse = reverseNum(i);
		int n = i + i_reverse;;
		bool reversible = true;
		while (n > 0)
		{
			if ((n % 10) % 2 == 0)
			{
				reversible = false;
				break;
			}
			n /= 10;
		}
		if (reversible)
		{
			reversableCount += 1;
			//std::cout << i << std::endl;
		}
	}

	std::cout << "reversableCount: " << reversableCount << std::endl;
}

int64_t solve()
{
	test();
	return 0;
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
