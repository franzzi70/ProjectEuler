
#include <iostream>
#include <chrono>
#include <vector>


__int64 r_configCount(int len, std::vector<__int64>& mem)
{
    if (mem[len] != 0)
        return mem[len];
    if (len == 0)
    {
        mem[len] = 1;
        return 1;
    }
    __int64 result = 0;
    result += r_configCount(len - 1, mem);
    if (len >= 2)
		result += r_configCount(len - 2, mem);
    if (len >= 3)
        result += r_configCount(len - 3, mem);
    if (len >= 4)
        result += r_configCount(len - 4, mem);
    mem[len] = result;
    return result;
}

__int64 configCount(int len)
{
    std::vector<__int64> mem(len + 1, 0);
    return r_configCount(len, mem);
}

__int64 solve()
{
    return configCount(50);
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
