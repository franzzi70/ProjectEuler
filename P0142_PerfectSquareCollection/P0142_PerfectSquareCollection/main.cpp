
#include <iostream>
#include <chrono>
#include <vector>
#include <map>
#include <list>

// Problem: https://projecteuler.net/problem=142
// Perfect Square Collection

// x+y+z = min
// x > y > z > 0
// x+y, x-y, x+z, x-z, y+z, y-z are all perfect squares

// additional considerations:
// check: 2|z, and 2|y, because two different perfect square numbers with same parity have distance divisible by 4

const int MAXN = 1'000'000;

std::map<int, std::list<std::pair<int, int>>> matchMap;

int64_t solve()
// different approach: find x first.
{
	for (int threshN = 2; threshN < MAXN; threshN += 1)
	{
		int testSq = threshN * threshN;
		int startMSeed = 2 - (threshN & 1);
		//int startMSeed = 1;
		for (int mSeed = startMSeed; mSeed < threshN; mSeed += 2)
		{
			int mSeedSq = mSeed * mSeed;
			int d = testSq - mSeed * mSeed;
			int m = (testSq + mSeedSq) / 2;
			if (matchMap.count(m) == 0)
			{
				std::list<std::pair<int, int>> zList;
				zList.push_back(std::pair<int, int>(threshN, mSeed));
				matchMap.insert(std::make_pair(m, zList));
			}
			else
			{
				std::list<std::pair<int, int>> zList = matchMap.find(m)->second;
				for (std::pair<int, int> it : zList)
				{

					int y = d / 2;
					int z = (it.first * it.first - it.second * it.second) / 2;
					int x = testSq - y;

					// test if x-z is perfect square
					int testSq1 = (int)(sqrt(y - z) + 0.1);
					if (testSq1 * testSq1 == y - z)
					{
							// test if y+z is perfect square
						int testSq2 = (int)(sqrt(y + z) + 0.1);
						if (testSq2 * testSq2 == y + z)
						{
							int sum_xyz = x + y + z;
							std::cout << "FOUND: " << sum_xyz << " x: " << x << " y: " << y << " z: " << z << " x+y+z: " << x + y + z << std::endl;
							return sum_xyz;
						}
					}
				}
				zList.push_back(std::pair<int, int>(threshN, mSeed));
			}
		}
	}
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
