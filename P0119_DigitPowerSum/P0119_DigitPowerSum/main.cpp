
#include <iostream>
#include <chrono>
#include <map>


class PowInfo
{
public:
		PowInfo(__int64 num, __int64 sum, __int64 power)
		: m_num(num), m_sum(sum), m_power(power)
	{
	}
	__int64 m_num;
	__int64 m_sum;
	__int64 m_power;

};

std::map<__int64,PowInfo> g_powTable;

int digitSum(__int64 num)
{
	int sum = 0;
	__int64 rest = num;
	while (rest > 0)
	{
		sum += rest % 10;
		rest /= 10;
	}
	return sum;
}


void init()
{
	const __int64 maxNum = LLONG_MAX / 100;

	for (int j = 2; j < 100; j++)
	{
		__int64 temp = j;
		for (int pow = 2;pow<20;pow++)
		{
			temp *= j;
			if (pow <= maxNum)
			{
				if (digitSum(temp) == j)
					g_powTable.insert(std::make_pair(temp, PowInfo(temp, j, pow)));
			}
		}
	}
}

__int64 solve()
{
	init();
	int i = 1;
	__int64 num = 0;
	for (auto it = g_powTable.begin(); it != g_powTable.end(); it++)
	{
		num = it->first;
		std::cout << "num: " << it->second.m_num << " sum: " << it->second.m_sum << " power: " << it->second.m_power << std::endl;
//		std::cout << i << " : " << num << std::endl;
		i += 1;
		if (i > 30)
			break;
	}
	return num;
}


int main()
{
    auto t1 = std::chrono::high_resolution_clock::now();
    __int64 solution = solve();
    auto t2 = std::chrono::high_resolution_clock::now();
    auto microSec = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
    std::cout << "solution: " << solution << std::endl << "duration: " << microSec << " micro seconds (" << ms << " ms)" << std::endl;
}
