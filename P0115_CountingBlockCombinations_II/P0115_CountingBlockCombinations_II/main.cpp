
#include <iostream>
#include <chrono>
#include <vector>


__int64 r_confCount(int minBlockLen, int len, std::vector<__int64>& mem)
{
    if (mem[len] != 0)
        return mem[len];

    if (len <= 0)
    {
        mem[0] = 1;
        return 1;
    }
    if (len < minBlockLen)    // only grey single blocks possible
    {
        mem[len] = 1;
        return 1;
    }
    if (len >= minBlockLen)
    {
        __int64 c1 = r_confCount(minBlockLen, len - 1, mem);    // 1st block grey
        __int64 startingReadWithEndCount = 0;
        for (int i = minBlockLen; i <= len - 1; i++) // diverse length red blocks with grey block at the end.
        {
            startingReadWithEndCount += r_confCount(minBlockLen, len - i - 1, mem);
        }
        __int64 sum = c1 + startingReadWithEndCount
            + 1;    // allblocksred
        mem[len] = sum;
        return sum;
    }
}

__int64 confCount(int minBlockLen, int len)
{
    std::vector<__int64> mem(len+1, 0);
    return r_confCount(minBlockLen, len, mem);
}

int solve()
{
    __int64 result = 0;
    int blockCount = 50;
    do {
        result = confCount(50, blockCount);
        blockCount++;
	} while(result < (__int64)1000000);
    return blockCount-1;
}

int main()
{
    auto t1 = std::chrono::high_resolution_clock::now();
    int solution = solve();
    auto t2 = std::chrono::high_resolution_clock::now();
    auto microSec = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
    std::cout << "solution: " << solution << std::endl << "duration: " << microSec << " micro seconds (" << ms << " ms)" << std::endl;
}
