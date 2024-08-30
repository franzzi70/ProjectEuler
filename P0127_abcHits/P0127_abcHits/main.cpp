#include <iostream>
#include <chrono>
#include <vector>
#include <algorithm>
#include "primes.h"
#include "mem.h"


//const int MAXNUM = 999;
const int MAXNUM = 119'999;
const int MAXPRIME = MAXNUM+1000;
const int PRIMEARRSIZE = (MAXPRIME / 2);

const int FACTOR_VECSIZE = (int)(log(MAXNUM) / log(2)) + 1;
StreamBlocks b;

primes::PrimesSieve g_sieve(MAXPRIME);
int g_primes[PRIMEARRSIZE];
int g_primesCount = 0;


class PrimeRadicals {
public:
    __int64 m_number;
    __int8 m_radicalCount;
	int* m_radicals;
    int m_radicalProd = 0;

    PrimeRadicals()
    {
        m_number = 0;
        m_radicalCount = 0;
        m_radicals = nullptr;
        //m_radicals = new std::vector<int>(FACTOR_VECSIZE, 0);
    }
    PrimeRadicals(const PrimeRadicals& other)
    {
        m_number = other.m_number;
        m_radicalCount = other.m_radicalCount;
        _memccpy(m_radicals, other.m_radicals, FACTOR_VECSIZE, sizeof(int));
        //m_radicals = other.m_radicals;
    }

    PrimeRadicals(int n, std::vector<int>* tmpfactors)
    {
        calculateRadicals(n, tmpfactors);
    }

    int getRadical()
    {
        if (m_radicalProd != 0)
			return m_radicalProd;

        int radical = 1;
        int lastFactor = 0;
        for (int i = 0; i < m_radicalCount; i++)
        {
            int f = m_radicals[i];
            if (f != lastFactor)
            {
                radical *= f;
                lastFactor = f;
            }
        }
        m_radicalProd = radical;
        return radical;
    }

    void calculateRadicals(int n, std::vector<int>* tmpfactors)
    {
        m_number = n;
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
            (*tmpfactors)[radicalCount] = _prime;
            radicalCount += 1;
            while (testNum % _prime == 0)
            {
                testNum /= _prime;
            }
            primeIx += 1;
        } while (testNum > 1);
        m_radicalCount = radicalCount;
        m_radicals = (int*) b.alloc(m_radicalCount * sizeof(int));
        for (int i = 0; i < m_radicalCount; i++)
		{
			m_radicals[i] = (*tmpfactors)[i];
		}
    }
    
    bool sharesFactor(int n2)
    {
        int primeIx = 0;
        while (primeIx < m_radicalCount)
        {
            if (n2 % m_radicals[primeIx] == 0)
			{
				return true;
			}
            primeIx += 1;
        }
        return false;
    }

    int getTestFactor()
    {
        // product of first two unused primes

        int prod = 1;
        int primeIx = 0;
        int radicalIx = 0;
        int r = m_radicals[radicalIx];
        int primeCount = 0;
        while (primeCount < 2)
		{
            int _prime = g_primes[primeIx];
            if (_prime != r)
            {
                primeCount += 1;
                prod *= _prime;
            }
            else {
                radicalIx += 1;
                if (radicalIx >= m_radicalCount)
                {
                    r = 1;
                }
                else
                {
                    r = m_radicals[radicalIx];
                }
            }
            primeIx += 1;
        }
        return prod;
    }
};


std::vector<PrimeRadicals*> g_radicals(MAXNUM+1);

void init()
{
    // init prime array:
    int _maxPrime = g_sieve.m_maxNumber;
    int _primeIx = 0;
    std::vector<int> *tmp_factors = new std::vector<int>(FACTOR_VECSIZE, 0);

    for (int i = 0; i <= _maxPrime; i++)
    {
        if (g_sieve.isPrime(i))
        {
            g_primes[_primeIx] = i;
            _primeIx += 1;
        }
    }
    g_primesCount = _primeIx;


    for (int i = 2; i <= MAXNUM; i++)
	{
		g_radicals[i] = new PrimeRadicals(i, tmp_factors);
	}
}


void testMem()
{
    for (int i = 0; i < 4; i++)
	{
        char* ptr = (char*)	b.alloc(300);
	}
}

__int64 solve()
{
    init();


    __int64 sum = 0;
    int count = 0;

    //std::vector<int>* a_radials = new std::vector<int>(FACTOR_VECSIZE, 0);
    //std::vector<int>* b_radials = new std::vector<int>(FACTOR_VECSIZE, 0);
    //std::vector<int> *c_radials = new std::vector<int>(FACTOR_VECSIZE, 0);

    int cLow = 3;

    //PrimeRadicals ra(2, a_radials);
    //PrimeRadicals rb(2, b_radials);
    //PrimeRadicals rc(2, c_radials);

    for (int c = cLow; c <= MAXNUM; c++)
    {
        if (c % 1000 == 0)
            std::cout << "c:" << c << std::endl;

        int aHigh = (c-1) / 2;

        //rc.calculateRadicals(c);

        PrimeRadicals* rc = g_radicals[c];
        int c_radical = rc->getRadical();;
        if (c_radical * 2 > c)
			continue;

        for (int a = 1; a <= aHigh; a++)
		{
			int b = c - a;

            if (a != 1)
            {
                if (rc->sharesFactor(a))
                    continue;

                if (a == 2)
                {
                    if (c_radical * rc->getTestFactor() >= c)
                    {
                        break;
                    }
                }
            }

            if (rc->sharesFactor(b))
                continue;

            PrimeRadicals* ra = g_radicals[a];
            if (a != 1)
            {
                if (ra->sharesFactor(b))
                    continue;
            }

            PrimeRadicals* rb = g_radicals[b];

            int a_radical = a == 1 ? 1 : ra->getRadical();

            if (a_radical * rb->getRadical() * c_radical < c)
			{
                //std::cout << "a:" << a << " b:" << b << " c:" << c << std::endl;
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
    if (microSec > 300'000'000)
    {
        std::cout << "(" << ((float)microSec) / 60'000'000 << " minutes )" << std::endl;
    }
}
