
#include <iostream>
#include <chrono>
#include "primes.h"

// x^2 - y^2 - z^2 = n
// x - y = y - z = d
// d > 0
// n > 0

// y = z + d
// x = z + 2d
// x^2 - y^2 - z^2 = n
// (z + 2d)^2 - (z + d)^2 - z^2 = n
// z^2 + 4dz + 4d^2 - z^2 - 2dz - d^2 - z^2 = n
// 2dz + 3d^2 - z^2 = n

// z = y - d
// x = y + d
// x^2 - y^2 - (y - d)^2 = n
// (y + d)^2 - y^2 - (y - d)^2 = n
// y^2 + 2dy + d^2 - y^2 - y^2 + 2dy - d^2 = n
// 4dy - y^2 = n    => use for upper boundarary of y
// 4dy - y^2 - n = 0
// solutions for y:
// y = 2*d + sqrt(4d^2-n) => find y for n and d, y must be integer

// 4dy - y^2 = n
// d = (n+y^2)/4*y

//const int MAX_N = 1'000'000;
const int THRESHOLD_N = 1000000;
const int MAX_PRIME = 1'000'100;

std::vector<int> solutionCounts(THRESHOLD_N +1);

__int64 solve()
{
    __int64 stepCount = 0;
    __int64 RangeCount = 0;
    __int64 prevRangeCount = 0;

    __int64 z = 1;
    //int maxZ = 1 + (int)round(sqrt(THRESHOLD_N + 1));
    __int64 maxZ = 1 + THRESHOLD_N;
    while (z <= maxZ)
    {
        __int64 d = 1;
        __int64 t3 = z * z;
        while (true)
		{
			stepCount += 1;

			//if (z == 20 && d == 7)
			//{
			//	std::cout << 20 << std::endl;
			//}
			//int n = (z+2*d)*(z+2*d) - (z+d) * (z+d) - z * z;
            //int n = z*z + 4*d*z + 4*d*d - (z*z + 2*d*z + d*d) - z*z;
			//int n = 2*d*z - 3*d*d - z*z;
            
			// n= d*d + 2*d*z + z*z - 4*d*d - 2*z*z
			// n = (d+z)^2 - 4*d*d - 2*z*z

            __int64 t1 = z * 2 * d;
            __int64 t2 = 3 * d * d;

			//__int64 n = 2 * d * z + 3 * d * d - z * z;
            __int64 n = t1 + t2 - t3;

            // int n = 2 * z + 3 - z * z;
			if (n > THRESHOLD_N)
				break;
            if (n > 0)
            {
                solutionCounts[n] += 1;
                RangeCount += 1;
            }

			d += 1;
		}

        if (z % 1000 == 0)
        {
            if (z % 10000 == 0)
            std::cout << "z: " << z << ", RangeCount: "
                << RangeCount << ", diff: "
                << RangeCount - prevRangeCount << std::endl;

            if (RangeCount - prevRangeCount == 0)
                break;

            prevRangeCount = RangeCount;
        }

        z += 1;
    }
	int solutionCount = 0;
    for (int i=0;i<THRESHOLD_N;i++)
	{
		if (solutionCounts[i] == 10)
		{
            solutionCount += 1;
		}
	}
	std::cout << "stepCount: " << stepCount << std::endl;
    return solutionCount;
}

int main()
{

    auto t1 = std::chrono::high_resolution_clock::now();
    __int64 solution = solve();
    //__int64 solution = solve_notworking();
    auto t2 = std::chrono::high_resolution_clock::now();
    auto microSec = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
    std::cout << "solution: " << solution << std::endl << "duration: " << microSec << " micro seconds (" << ms << " ms)" << std::endl;
    if (microSec > 300'000'000)
    {
        std::cout << "(" << ((float)microSec) / 60'000'000 << " minutes )" << std::endl;
    }
}
