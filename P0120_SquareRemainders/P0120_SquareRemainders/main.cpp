
#include <iostream>
#include <chrono>
#include <set>

int getHighR(int num)
{
    int z1 = num - 1;
    int z2 = num + 1;
    int mod = num * num;
    int currentHighest = 0;
	std::set<int> results;
    int z1Pow = z1;
    int z2Pow = z2;
    int keyValue = z1Pow * mod + z2Pow;

    while (results.find(keyValue) == results.end())
    {
        results.insert(keyValue);
        int testValue = (z1Pow + z2Pow) % mod;
        if (testValue > currentHighest)
		{
			currentHighest = testValue;
		}
        z1Pow = (z1Pow * z1) % mod;
        z2Pow = (z2Pow * z2) % mod;
        keyValue = z1Pow * mod + z2Pow;
    }
    return currentHighest;
}

int solve()
{
    __int64 sum = 0;
    for (int i = 3; i <= 1000; i++)
    {
        sum +=  getHighR(i);
        // std::cout << "High R for " << i << " is " << h << std::endl;
    }
    return sum;
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
