// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <iostream>
#include <chrono>
#include <set>

// 1/x + 1/y = 1/n
// x = n + a
// y = n + b
// 1/(n+a) + 1/(n+b) = 1/n

// c*a=b*(a-c)

__int64 maxCountTracker = 0;

int findSolutions_SlowBruteForce(__int64 c)
{
    std::set<std::pair<__int64, __int64>> solutions;
    int count = 0;
    for (__int64 a = c + 1;
        (a - c) * (a - c) <= (a*c) ;
        a++
        )
    {
        if (c * a % (a - c) == 0)
        {
            __int64 b = c * a / (a - c);
            if (c * b % (b - c) == 0)
            {
                __int64 test_a;
                __int64 test_b;
                if (a > b)
                {
                    test_a = a;
                    test_b = b;
                }
                else
                {
					test_a = b;
					test_b = a;

                }
                std::pair<__int64, __int64> testPair = std::make_pair(test_a, test_b);
                if (solutions.find(testPair) != solutions.end())
					continue;
                //std::cout << "1/" << a << " + 1/" << b << " = 1/" << c << std::endl;
                count++;
                solutions.insert(testPair);
			}
		}
        if (count > maxCountTracker)
        {
			maxCountTracker = count;
			//std::cout << "c: " << c << " count: " << count << std::endl;
        }
    }
	return count;
}

__int64 solve_slowBruteForce()
{
    for (__int64 i = 2; ; i++)
    {
        int solutions = findSolutions_SlowBruteForce(i);
        if (i%10000==0)
            std::cout << "i: " << i << " solutions: " << solutions << std::endl;
        if (solutions > 1000)
            return (int)i;
    }
    // solution: 180180

    //return findSolutions(1260);
    //return findSolutions(1000003);
    //return findSolutions(6);
}

// approach: generate (a*b) with maximal number of prime factors
int solve()
{
    return solve_slowBruteForce();
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
