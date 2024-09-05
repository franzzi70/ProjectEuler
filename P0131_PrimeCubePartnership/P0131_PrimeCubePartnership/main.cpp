
#include <iostream>
#include <chrono>
#include "primes.h"
#include "MyBigInteger.h"

typedef MyBigInteger _IntType;

const int MAXPRIME = 1'000'000;
//const int MAXPRIME = 100;
primes::PrimesSieve g_primes(MAXPRIME+100);
const int SQUAREARRSIZE = (int)(sqrt(MAXPRIME)) + 2;
std::vector<_IntType> g_squareNumbers(SQUAREARRSIZE);

int getHighNIxforP(int p)
{
    return (int)floor(sqrt(
        (p - 1) / 3
    ));
}

_IntType getStartXforNP(_IntType n, _IntType p)
{

    // hier weiter
    // (x+n)^3 = x^3 + p * x^2
    // p = ((x+n)^3 - x^3) / x^2
    // p = (x^3 + 3nx^2 + 3n^2x + n^3 - x^3) / x^2
    // p*x^2 = 3n*x^2 + 3n^2*x + n^3
    // x^2 * (3n-p) + 3n^2*x + n^3 = 0
    // 
    _IntType n2 = n * n;
    //_IntType x1 = (_IntType)(
    //    ((-3) * n2 + sqrt(9 * n2 * n2 - 4 * n2 * n * (3 * n - p))) / (2 * (3 * n - p))
    //    );
    _IntType x2 = (_IntType)(
        ((double)(_IntType(-3) * n2) - sqrt((double)(n2 * n2 * 9 - n2 * 4 * n * (n * 3 - p)))) / (double)((n * 3 - p) * 2)
        );

    return x2;
}

__int64 solve()
{
    // (x+n)^3 = x^3 + p * x^2
	// p = ((x+n)^3 - x^3) / x^2
	// p = (x^3 + 3nx^2 + 3n^2x + n^3 - x^3) / x^2
	// p = (3n*(x^2+n*x)+n^3)/x^2
    // p = 3n + 3n^2/x + n^3/x^2
	
    // 1.a) => p > 3n +1
    // 1.b) => n < (p-1)/3
    // 2.) => x|n
	// 3.) => n^3 >= x^2
    // 4.) n^3/x^2 => n is square number

    _IntType sq = 1;
    int diff = 1;
	for (int i = 1; i < SQUAREARRSIZE; i++)
	{
		g_squareNumbers[i] = sq;
        diff += 2;
        sq += diff;
	}

	int foundCount = 0;
    _IntType xPrev = 1;
    for (int _p = 3; _p < MAXPRIME; _p = g_primes.nextPrime(_p))
    {
		int p = _p;

		_IntType nThresh = (p - 1) / 3;
        if (p == 2791)
        {
			std::cout << 2791 << std::endl;
        }

        int nIx = getHighNIxforP(p);;
		_IntType n = g_squareNumbers[nIx];

        _IntType xPrev = 1;
        _IntType diff = 0;
		bool found = false;
        do {

            _IntType n2 = n * n;
            for (_IntType x = getStartXforNP(n, p); x >= _IntType(1) ; x -= 1)
            {
                diff = (x + n) * (x + n) * (x + n);
                diff -= x * x * x;
                diff -= (x * x) * _IntType(p);

				if (diff != _IntType(0))
				{
					break;
				}
                if (diff==_IntType(0))
                {
//                    std::cout << "p: " << p << " n: " << n << " x: " << x << ", x-ratio: " << ((float)x) / xPrev << ", p/n: " << ((float)p) / n << ", x/p: " << ((float)x)/p << std::endl;
                    std::cout << "p: " << p;
                    std::cout << " n: " << (std::string)n << " x: " << (std::string)x;
                    //std::cout << ", x-ratio: " << ((float)x) / xPrev << ", p/n: " << ((float)p) / n << ", x/p: " << ((float)x) / p << std::endl;
					std::cout << std::endl; 

                    xPrev = x;
                    found = true;
					foundCount += 1;

                    break;
                }
            }

            nIx -= 1;
            if (nIx < 1)
                break;
            n = g_squareNumbers[nIx];

        } while (diff > _IntType(0) && !found);
    }

    return foundCount;
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
