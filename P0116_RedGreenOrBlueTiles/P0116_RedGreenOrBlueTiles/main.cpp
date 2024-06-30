
#include <iostream>
#include <chrono>
#include <vector>

const int RED(2);
const int GREEN(3);
const int BLUE(4);

__int64 r_configCount (int blockLen, int len, std::vector<__int64>& mem)
{
    if (len < blockLen)
		return 1;
    if (mem[len] != 0)
        return mem[len];
    __int64 result = 0;
    result += r_configCount(blockLen, len - 1, mem);
    result += r_configCount(blockLen, len - blockLen, mem);
    mem[len] = result;
	return result;
}

__int64 configCount(int blockLen, int len)
{
	std::vector<__int64> mem(len + 1, 0);
	return r_configCount(blockLen, len, mem) - 1;   // subtract 1 to exclude the case of all black tiles
}

__int64 solve()
{
    return configCount(RED,50) + configCount(GREEN,50) + configCount(BLUE, 50);
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
