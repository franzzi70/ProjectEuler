// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <iostream>
#include <chrono>

const int pow2Num = 7830457;
const __int64 dig11 = 10000000000;

__int64 solve()
{
    __int64 f = 28433;
    
    __int64 powRest = pow2Num;
    const __int64 bit16 = 0x10000;

    while (powRest > 16)
    {
        powRest -= 16;
        f = (f * bit16) % dig11;
    }
    while (powRest > 0)
    {
        powRest -= 1;
        f = (f * 2) % dig11;
    }
    f += 1;

    return f % dig11;
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
