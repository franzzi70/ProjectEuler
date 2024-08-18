#include <iostream>
#include <chrono>
#include <vector>
#include <algorithm>
#include "primes.h"


//const int MAXNUM = 19'999;
const int MAXNUM = 999;
const int MAXPRIME = MAXNUM+1000;
const int PRIMEARRSIZE = (MAXPRIME / 2);

const int FACTOR_VECSIZE = (int)(log(MAXNUM) / log(2)) + 1;


primes::PrimesSieve g_sieve(MAXPRIME);
int g_primes[PRIMEARRSIZE];
int g_primesCount = 0;


class PrimeRadicals {
public:
    __int64 m_number;
    __int8 m_radicalCount;
	std::vector<int> *m_radicals;

    PrimeRadicals()
    {
        m_number = 0;
        m_radicalCount = 0;
        m_radicals = new std::vector<int>(FACTOR_VECSIZE, 0);
    }
    PrimeRadicals(const PrimeRadicals& other)
    {
        m_number = other.m_number;
        m_radicalCount = other.m_radicalCount;
        m_radicals = other.m_radicals;
    }

    PrimeRadicals(int n, std::vector<int>* factors)
    {
        m_number = n;
        m_radicals = factors;
        calculateRadicals(n);
    }
    int getRadical()
    {
        int radical = 1;
        int lastFactor = 0;
        for (int i = 0; i < m_radicalCount; i++)
        {
            int f = (*m_radicals)[i];
            if (f != lastFactor)
            {
                radical *= f;
                lastFactor = f;
            }
        }
        return radical;
    }
    void calculateRadicals(int n)
    {
        int primeIx = 0;
        int radicalCount = 0;
        int testNum = n;

        do
        {
            int _prime = g_primes[primeIx];
            while (testNum % _prime != 0) {
                primeIx += 1;
                _prime = g_primes[primeIx];
                }
            testNum /= _prime;
            (*m_radicals)[radicalCount] = _prime;
            radicalCount += 1;
            while (testNum % _prime == 0)
            {
                testNum /= _prime;
            }
            primeIx += 1;
        } while (testNum > 1);
        m_radicalCount = radicalCount;
    }
    
    bool sharesFactor(int n2)
    {
        int primeIx = 0;
        while (primeIx < m_radicalCount)
        {
            if (n2 % (*m_radicals)[primeIx] == 0)
			{
				return true;
			}
            primeIx += 1;
        }
        return false;
    }
};

void init()
{
    // init prime array:
    int _maxPrime = g_sieve.m_maxNumber;
    int _primeIx = 0;
    std::vector<int> *m_factors = new std::vector<int>(FACTOR_VECSIZE, 0);

    for (int i = 0; i <= _maxPrime; i++)
    {
        if (g_sieve.isPrime(i))
        {
            g_primes[_primeIx] = i;
            _primeIx += 1;
        }
    }
    g_primesCount = _primeIx;
}


__int64 solve()
{
    init();

    __int64 sum = 0;
    int count = 0;

    std::vector<int>* a_radials = new std::vector<int>(FACTOR_VECSIZE, 0);
    std::vector<int>* b_radials = new std::vector<int>(FACTOR_VECSIZE, 0);
    std::vector<int> *c_radials = new std::vector<int>(FACTOR_VECSIZE, 0);

    int cLow = 3;

    PrimeRadicals ra(2, a_radials);
    PrimeRadicals rb(2, b_radials);
    PrimeRadicals rc(2, c_radials);

    for (int c = cLow; c <= MAXNUM; c++)
    {
        if (c % 1000 == 0)
            std::cout << "c:" << c << std::endl;

        int aHigh = (c-1) / 2;

        rc.calculateRadicals(c);

        for (int a = 1; a <= aHigh; a++)
		{
			int b = c - a;

            if (a!=1)
                if (rc.sharesFactor(a))
                    continue;

            if (rc.sharesFactor(b))
                continue;

            if (a != 1)
            {
                ra.calculateRadicals(a);
                if (ra.sharesFactor(b))
                    continue;
            }

            rb.calculateRadicals(b);

            int a_radical = a == 1 ? 1 : ra.getRadical();

            if (a_radical * rb.getRadical() * rc.getRadical() < c)
			{
                // std::cout << "a:" << a << " b:" << b << " c:" << c << std::endl;
				count += 1;
                sum += c;
			}

            //PrimeRadicals f3(b, c_radials);

			//if (f1.getRadical() * f2.getRadical() * f3.getRadical() < c)
			//{
			//	if (g_sieve.isCoprime(a, b) && g_sieve.isCoprime(a, c) && g_sieve.isCoprime(b, c))
			//	{
			//		return c;
			//	}
			//}
		}
        //if (i%1000==0)
        //    std::cout << f1.getRadical() << std::endl;
    }

    std::cout << "count: " << count << std::endl;

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
