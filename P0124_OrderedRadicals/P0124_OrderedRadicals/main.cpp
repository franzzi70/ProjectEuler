// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <iostream>
#include <chrono>
#include <vector>
#include <algorithm>
#include "primes.h"

#define MAXNUM 100'000
#define MAXPRIME 1000'000
#define PRIMEARRSIZE (MAXPRIME/2)


primes::PrimesSieve g_sieve(MAXPRIME);
int g_primes[PRIMEARRSIZE];
int g_primesCount = 0;

#define FACTOR_VECSIZE 30
class PrimeFactors {
public:
    __int64 m_number;
    __int8 m_factorCount;
    std::vector<int> m_factors;

    PrimeFactors()
	{
		m_number = 0;
		m_factorCount = 0;
		m_factors = std::vector<int>(FACTOR_VECSIZE);
	}
    PrimeFactors(const PrimeFactors& other)
	{
		m_number = other.m_number;
		m_factorCount = other.m_factorCount;
		m_factors = other.m_factors;
	}

    PrimeFactors(int n)
    {
        m_number = n;
        m_factors = std::vector<int>(FACTOR_VECSIZE);
        calculateFactors(n);
    }
    int getRadical()
	{
		int radical = 1;
        int lastFactor = 0;
		for (int i = 0; i < m_factorCount; i++)
		{
            int f = m_factors[i];
            if (f != lastFactor)
			{
				radical *= f;
				lastFactor = f;
			}
		}
		return radical;
	}
private:
    void calculateFactors(int n)
    {
        int primeIx = 0;
        int factorCount = 0;
        int testNum = n;

        do
        {
            while (testNum % g_primes[primeIx] != 0) {
                primeIx += 1;
            }
            int _prime = g_primes[primeIx];
            testNum /= _prime;
            m_factors[factorCount] = _prime;
            factorCount += 1;
        } while (testNum > 1);
        m_factorCount = factorCount;
    }
};

void init()
{
    // init prime array:
    int _maxPrime = g_sieve.m_maxNumber;
    int _primeIx = 0;
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

class RadicalRec
{
public:
	int m_number;
	int m_radical;
    PrimeFactors m_pf;
	RadicalRec(int n, int radical, PrimeFactors* pf)
	{
		m_number = n;
        m_radical = radical;
        if (pf != nullptr)
            m_pf = *pf;
	}

    bool operator < (const RadicalRec& other)
	{
        if (m_radical == other.m_radical)
        {
            return m_number < other.m_number;
		}
        return m_radical < other.m_radical;
	}

};

std::vector<RadicalRec*> g_radicals (MAXNUM, nullptr);

int solve()
{
    init();

    g_radicals[0] = new RadicalRec(1, 1, nullptr);
    for (int i=2;i<=MAXNUM; i++)
	{
		PrimeFactors pf(i);
		int radical = pf.getRadical();
		g_radicals[i-1] = new RadicalRec(i, radical, &pf);
	}
    std::sort(g_radicals.begin(), g_radicals.end(), [](RadicalRec* a, RadicalRec* b) { return *a < *b; });
    return g_radicals[9999]->m_number;
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
