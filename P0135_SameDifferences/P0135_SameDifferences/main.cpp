
#include <iostream>
#include <chrono>
#include <cmath>

// z^2 - y^2 - x^2 = n
// z^2 - y^2 = n + x^2
// (z + y)(z - y) = n + x^2
// x^2 + y^2 = z^2 - n

// x^2 + y^2 = k^2, k=z^2 - n

// m*m = n
// k^2-m^2 = (k+m)(k-m)
// x^2 - y^2 = (x + y)(x - y)

// (x+k)^2 - x^2 - d = 0
// x^2 + 2*x*k + k^2 -x^2 - n = 0
// 2*x*k + k^2 - n = 0
// 2*x*k + k^2 = n
// x = (n - k ^ 2) / 2*k

//const int LIMIT = 1'000'000;
const int LIMIT = 2000;


__int64 solve()
{
    __int64 sum = 0;
    for (int n = 1; n < LIMIT; n++)
    {
        int sequenceCount = 0;
        std::cout << "n: " << n << std::endl;
  //      if (n==27)
		//{
		//	std::cout << "n == 27" << std::endl;
		//}
        for (int z = 1; z < LIMIT;z++)
        {
			int d = z * z + n;

            //if (z == 6 && n == 27)
            //{
            //    std::cout << "n: " << n << " z: " << z << std::endl;
            //}

            for (int k = 1; k < LIMIT; k++)
            {
                int num = d - k * k;
                int denom = 2 * k;
                if (num <= 0)
                    break;
                if (num % denom != 0)
                    continue;

                int y = num / denom;
				int x = y + k;
                if (y <= z)
                    continue;

                if (x - y != y - z)
                    continue;
                sequenceCount += 1;
				
                if (sequenceCount == 10)
				{
					std::cout << "n: " << n << " z: " << z << " x: " << x << " y: " << y << std::endl;
				}
                // std::cout << "( x: " << x << " y: " << y << " z: " << z << " )" << std::endl;
            }

            //for (int n = 1; n < LIMIT; n++)
            //{
            //    int k = 1;
            //    int d = d - z * z;
            //    int num = d - k * k;
            //    int denom = 2 * k;
            //    if (num <= 0)
            //        break;
            //    if (num % denom != 0)
            //        continue;
            //    int y = num / denom;
            //    int x = y + k;

            //    if (z == 6 && i == 27)
            //    {
            //        std::cout << "i: " << i << " z: " << z << std::endl;
            //    }
            //}
        }
        if (sequenceCount == 10)
        {
            // std::cout << "first with 10: " << n << std::endl;
            std::cout << "has 10 sequences: " << n << std::endl;
            sum += n;
        }
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
    if (microSec > 300'000'000)
    {
        std::cout << "(" << ((float)microSec) / 60'000'000 << " minutes )" << std::endl;
    }
}
