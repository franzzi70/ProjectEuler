
#include <iostream>
#include <chrono>

const double C = 60;

int64_t solve()
{

    return 0;
}


int main()
{
    auto t1 = std::chrono::high_resolution_clock::now();
    volatile int64_t solution = solve();
    auto t2 = std::chrono::high_resolution_clock::now();
    auto microSec = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
    std::cout << "solution: " << solution << std::endl << "duration: " << microSec << " micro seconds (" << ms << " ms)" << std::endl;
}
