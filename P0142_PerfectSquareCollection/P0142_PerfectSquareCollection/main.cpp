
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

// for wolfram alpha
// a*a=x+y,b*b=x-y,c*c=x+z,d*d=x-z,e*e=y+z,f*f=y-z,x>y>z>0,x+y+z=k

// wolfram alpha output:
// a > 0, 0 < b < a, sqrt(2)/2 * sqrt(a ^ 2 + b ^ 2) < c < a,
// d = sqrt(a ^ 2 + b ^ 2 - c ^ 2), e = sqrt(c ^ 2 - b ^ 2),
// f = sqrt(a ^ 2 - c ^ 2),
// k=(a ^ 2 - b ^ 2 + 2 c ^ 2) / 2,
// x=(a ^ 2 + b ^ 2) / 2,
// y=(a ^ 2 - b ^ 2) / 2,
// z=(-a ^ 2 - b ^ 2 + 2 c ^ 2) / 2
// 
// 
// k = (a^2 - b^2 + 2 c^2) / 2,
// x = (a^2 + b^2) / 2,
// y = (a^2 - b^2) / 2,
// z = (-a^2 - b^2 + 2 c^2) / 2

// additional considerations:
// check: 2|z, and 2|y, because two different perfect square numbers with same parity have distance divisible by 4

const int QARRSIZE = 10'000'000;
//int64_t sqArr[QARRSIZE];
std::vector<int64_t> sqArr(QARRSIZE);

void init()
{
	//sqArr[0] = 0;
	//int l = 1;
	//int64_t sq = 0;
	//for (int i = 1; i < QARRSIZE; i++)
	//{
	//	sq += l;
	//	sqArr[i] = sq;
	//	l += 2;
	//}
	for (int i = 1; i < QARRSIZE; i++)
	{
		sqArr[i] = i * i;
	}
}

void test()
{

	int sq_a = 400;
	int sq_b = 144;
	int sq_c = 121;

	int x = (sq_a + sq_b) / 2;
	int y = (sq_a - sq_b) / 2;
	int z = (2 * sq_c - sq_b - sq_a) / 2;

	double testC = sqrt(2) * sqrt(sq_a + sq_b) / 2;

	std::cout << "x: " << x << " y: " << y << " x+y: " << x + y << " x-y:" << x - y << std::endl;
	std::cout << "z: " << z << " x+z: " << x + z << " x-z:" << x - z << std::endl;
	std::cout << " y+z: " << y + z << " y-z:" << y - z << std::endl;
	std::cout << "testC: " << testC << " c: " << sqrt(x + z) << std::endl;
}

std::map<int, std::list<std::pair<int, int>>> zMap;

int64_t solve()
{
	test();
	init();

	int testCount = 0;
	int cand_sum = 256 * 256 * 256 * 127;


	for (int threshN = 1; threshN < QARRSIZE; threshN += 1)
	{
		int testSq = threshN * threshN;
		int startZSeed = 2 - (threshN & 1);
		for (int zSeed = startZSeed; zSeed < threshN; zSeed += 1)
		{
			int d = testSq - zSeed * zSeed;
			if (zMap.count(d) == 0)
			{
				std::list<std::pair<int, int>> zList;
				zList.push_back(std::pair<int, int>(threshN, zSeed));
				zMap.insert(std::make_pair(d, zList));
				// zMap.insert(std::pair<int, std::list<std::pair<int, int>>>(zSeed, std::list<std::pair<int, int>>()));
			}
			else
			{
				std::list<std::pair<int, int>> zList = zMap.find(d)->second;
				for (std::pair<int, int> it : zList)
				{

					int z = d / 2;
					int y = it.first * it.first - z;
					int x = testSq - z;

					testCount += 1;
					if (testCount % 10'000'000 == 0)
					{
						std::cout << "testCount: " << testCount << " threshN: " << threshN << " zSeed: " << zSeed << std::endl;
						std::cout << "x: " << x << " y: " << y << " z: " << z << " x+y+z: " << x + y + z << std::endl;
					}

					if (y % 2 == 0)
					{
						// test if x-y is perfect square
						int testSq1 = (int)(sqrt(x - y) + 0.1);
						if (testSq1 * testSq1 == x - y)
						{
							int testSq2 = (int)(sqrt(x + y) + 0.1);
							if (testSq2 * testSq2 == x + y)
							{
								int sum_xyz = x + y + z;
								if (sum_xyz < cand_sum)
								{
									std::cout << "FOUND: " << sum_xyz << " x: " << x << " y: " << y << " z: " << z << " x+y+z: " << x + y + z << std::endl;
									//return sum_xyz;
									cand_sum = sum_xyz;
								}
							}
						}
					}
				}
				zList.push_back(std::pair<int, int>(threshN, zSeed));
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
