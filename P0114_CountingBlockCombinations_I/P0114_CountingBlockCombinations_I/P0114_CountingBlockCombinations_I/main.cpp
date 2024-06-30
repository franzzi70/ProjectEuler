
#include <iostream>
#include <chrono>
#include <vector>

static std::vector<__int64> mem (51, 0);

__int64 confCount(int len)
{
    if (mem[len] != 0)
        return mem[len];

    if (len <= 0)
    {
        mem[0] = 1;
        return 1;
    }
    if (len < 3)    // only grey single blocks possible
    {
        mem[len] = 1;
        return 1;
    }
    if (len >= 3)
    {
        __int64 c1 = confCount(len - 1);    // 1st block grey
        __int64 startingReadWithEndCount = 0;
        for (int i = 3; i <= len - 1; i++) // diverse length red blocks with grey block at the end.
        {
            startingReadWithEndCount += confCount(len - i - 1);
        }
        __int64 sum = c1 + startingReadWithEndCount
            + 1;    // allblocksred
        mem[len] = sum;
        return sum;
    }
}

__int64 solve()
{
    return confCount(50);
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
