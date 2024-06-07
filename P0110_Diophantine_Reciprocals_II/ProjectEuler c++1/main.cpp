
#include <iostream>
#include <time.h>
#include "primes.h"

#define FACTOR_VECSIZE 20
#define MAXPRIME 10000000
#define PRIMEARRSIZE (MAXPRIME/2)

primes::PrimesSieve g_sieve(MAXPRIME);
int* g_primes;
int g_primesCount = 0;


class PrimeFactors {
public:
    __int64 m_number;
    __int8 m_factorCount;
    std::vector<__int64> m_factors;
    std::vector<std::pair<int, int>>
    m_factorGroups;

    PrimeFactors(__int64 n)
    {
        m_number = n;
        calculateFactors(n);
    }
private:
    void calculateFactors(__int64 n)
    {
        int primeIx = 0;
        int groupIx = 0;
        int groupCount = 0;
        __int64 lastPrime = 0;
        int factorCount = 0;
        __int64 testNum = n;

        do
        {
            while (testNum % g_primes[primeIx] != 0) {
                primeIx += 1;
            }
            __int64 _prime = g_primes[primeIx];
            testNum /= _prime;
            m_factors.push_back(_prime);
            groupCount += 1;

			if (_prime == lastPrime && lastPrime != 0)
			{
				m_factorGroups[groupIx].second += 1;
			}
			else
			{
				m_factorGroups.push_back(std::pair<int, int>(_prime, 1));
				if (lastPrime != 0)
                    groupIx += 1;
			}

            lastPrime = _prime;
            factorCount += 1;
        } while (testNum > 1);
        m_factorCount = factorCount;
    }

};


bool isPrime(__int64 num)
{
    return g_sieve.isPrimeExt(num);
}



void init()
{
    // init prime array:
    int _maxPrime = g_sieve.m_maxNumber;
    int _primeIx = 0;
    int *g_primes_tmp = new int[_maxPrime / 2];

    for (int i = 0; i <= _maxPrime; i++)
    {
        if (isPrime(i))
        {
            if (_primeIx >= PRIMEARRSIZE)
			{
				std::cout << "prime array too small" << std::endl;
				break;
			}
            else
                // #pragma warning(suppress: C6386)
                g_primes_tmp[_primeIx] = i;
            _primeIx += 1;
        }
    }

    g_primes = new int[_primeIx];

 //   for (int i=0; i < _primeIx; i++)
	//{
	//	//g_primes[i] = g_primes_tmp[i];
	//}

    // #pragma warning(suppress: C6386)
    memcpy(g_primes, g_primes_tmp, _primeIx * sizeof(int));

    delete[] g_primes_tmp;
    g_primesCount = _primeIx;
}



__int64 factorCount(std::vector<int>& v)
{
    __int64 result = 1;
    for (auto it = v.begin(); it != v.end(); it++)
    {
        result *= (*it) * 2 + 1;
    }
    return result;
}

__int64 factorCount(std::vector<std::pair<int, int>> &fGroups)
{
    __int64 result = 1;
    for (auto it = fGroups.begin(); it != fGroups.end(); it++)
	{
		result *= (it->first * 2 + 1);
	}
    return result;
}

__int64 solutionCount(__int64 factorCount)
{
	return (factorCount + 1) / 2;
}

__int64 solutionCount(std::vector<int>& v)
{
    return (factorCount(v) + 1) / 2;
}

__int64 solutionCount(std::vector<std::pair<int, int>>& fGroups)
{
    return (factorCount(fGroups) + 1) / 2;
}

__int64 calcNumber(std::vector<int>& v)
{
    __int64 result = 1;
    int prime = 2;
    for (auto it = v.begin(); it != v.end(); it++)
    {
        result *= (*it) * prime;
        prime = g_sieve.nextPrime(prime);
    }
    return result;
}
__int64 calcNumber(std::vector<std::pair<int,int>>& gv)
{
    __int64 result = 1;
    for (auto it = gv.begin(); it != gv.end(); it++)
    {
        for (int i=0 ; i < it->first ; i++)
            result *= it->second;
    }
    return result;
}

class Node {
public:
    Node(int n)
	{
        int initialCount = 0;
        __int64 initialTerm = 1;
        __int64 factorMinCount = n * 2 - 1;
        __int64 f = 1;
        while (initialTerm < factorMinCount)
		{
            __int64 prime = g_primes[initialCount];
            f *= prime;
            initialTerm *= 3;
            m_primegroups.push_back(std::pair<int, int>(0, prime));
            initialCount += 1;
		}
        m_currentMinNumber = f;
        m_currentMinGroup = m_primegroups;
        m_sequenceLen = initialCount;
        m_goalSolutionCount = n;
	}
    
    void distr()
    {
        for (int i = 0; i < 100; i++)
        {
            int parNumbers = m_sequenceLen + i;
            if (distr(0, 1, parNumbers, parNumbers))
            {
                std::cout << "parNumbers: " << parNumbers << std::endl;
                std::cout << "currentMinNumber: " << m_currentMinNumber << " , solutionCount:" << solutionCount(m_currentMinGroup) << std::endl;
            }
        }
    }

    bool distr(int position, __int64 factor, int numberCount, int maxHeigth)
    {
        if (position >= m_sequenceLen)
        {
            return false;
        }

        if (numberCount == 0)
        {
            __int64 _solutionCount = solutionCount(m_primegroups);
            if (_solutionCount > m_goalSolutionCount)
            {
                if (factor < m_currentMinNumber)
				{
					m_currentMinNumber = factor;
					m_currentMinGroup = m_primegroups;
                    return true;
				}
            }
            return false;
        }

        bool found = false;
        __int64 f = 1;
        for (int i = 1 ; i <= maxHeigth; i++)
        {
            m_primegroups[position].first = i;
            f *= m_primegroups[position].second;

            __int64 parFactor = factor * f;
            if (parFactor > m_currentMinNumber)
				break;

            if (
                distr(
                    position + 1,
                    factor * f,
                    numberCount - i,
                    i
                    )
                )
            {
                found = true;
            }
            
        }
        m_primegroups[position].first = 0;
        return found;
    }


    int m_sequenceLen;
    int m_goalSolutionCount;
    std::vector<std::pair<int,int>> m_primegroups;
    __int64 m_currentMinNumber;
    std::vector<std::pair<int, int>> m_currentMinGroup;
};

std::vector<std::pair<int, int>> findPrimeGroups(int solutionCount)
{
    Node n(solutionCount);
    n.distr();

    return n.m_currentMinGroup;
}

void test()
{
    primes::PrimesSieve g_sieve(1000000);

    int a = 957337;

    std::cout << a << " is prime: " << g_sieve.isPrime(a) << std::endl;

    a += 1;
    std::cout << a << " is prime: " << g_sieve.isPrime(a) << std::endl;

    a += 1;
    std::cout << a << " is prime: " << g_sieve.isPrime(a) << std::endl;
    a += 1;
    std::cout << a << " is prime: " << g_sieve.isPrime(a) << std::endl;
    a += 1;
    std::cout << a << " is prime: " << g_sieve.isPrime(a) << std::endl;
    a += 1;
    std::cout << a << " is prime: " << g_sieve.isPrime(a) << std::endl;
    a += 1;
    std::cout << a << " is prime: " << g_sieve.isPrime(a) << std::endl;
    a += 1;
    std::cout << a << " is prime: " << g_sieve.isPrime(a) << std::endl;
    a += 1;
    std::cout << a << " is prime: " << g_sieve.isPrime(a) << std::endl;
    a += 1;
    std::cout << a << " is prime: " << g_sieve.isPrime(a) << std::endl;
    a += 1;
    std::cout << a << " is prime: " << g_sieve.isPrime(a) << std::endl;
    a += 1;
    std::cout << a << " is prime: " << g_sieve.isPrime(a) << std::endl;
    a += 1;
    std::cout << a << " is prime: " << g_sieve.isPrime(a) << std::endl;
    a += 1;
    std::cout << a << " is prime: " << g_sieve.isPrime(a) << std::endl;

    a = 1000007;
    std::cout << a << " is prime: " << g_sieve.isPrimeExt(a) << std::endl;

    a += 1;
    std::cout << a << " is prime: " << g_sieve.isPrimeExt(a) << std::endl;
    a += 1;
    std::cout << a << " is prime: " << g_sieve.isPrimeExt(a) << std::endl;
    a += 1;
    std::cout << a << " is prime: " << g_sieve.isPrimeExt(a) << std::endl;
    a += 1;
    std::cout << a << " is prime: " << g_sieve.isPrimeExt(a) << std::endl;
    a += 1;
    std::cout << a << " is prime: " << g_sieve.isPrimeExt(a) << std::endl;
    a += 1;
    std::cout << a << " is prime: " << g_sieve.isPrimeExt(a) << std::endl;
    a += 1;
    std::cout << a << " is prime: " << g_sieve.isPrimeExt(a) << std::endl;
    a += 1;
    std::cout << a << " is prime: " << g_sieve.isPrimeExt(a) << std::endl;
    a += 1;
    std::cout << a << " is prime: " << g_sieve.isPrimeExt(a) << std::endl;
    a += 1;
    std::cout << a << " is prime: " << g_sieve.isPrimeExt(a) << std::endl;
    a += 1;
    std::cout << a << " is prime: " << g_sieve.isPrimeExt(a) << std::endl;
    a += 1;
    std::cout << a << " is prime: " << g_sieve.isPrimeExt(a) << std::endl;
    a += 1;
    std::cout << a << " is prime: " << g_sieve.isPrimeExt(a) << std::endl;
    a += 1;
    std::cout << a << " is prime: " << g_sieve.isPrimeExt(a) << std::endl;
    a += 1;
    std::cout << a << " is prime: " << g_sieve.isPrimeExt(a) << std::endl;
    a += 1;
    std::cout << a << " is prime: " << g_sieve.isPrimeExt(a) << std::endl;
    a += 1;
    std::cout << a << " is prime: " << g_sieve.isPrimeExt(a) << std::endl;
    a += 1;
    std::cout << a << " is prime: " << g_sieve.isPrimeExt(a) << std::endl;
    a += 1;
    std::cout << a << " is prime: " << g_sieve.isPrimeExt(a) << std::endl;
    a += 1;
    std::cout << a << " is prime: " << g_sieve.isPrimeExt(a) << std::endl;
    a += 1;
    std::cout << a << " is prime: " << g_sieve.isPrimeExt(a) << std::endl;
    a += 1;
    std::cout << a << " is prime: " << g_sieve.isPrimeExt(a) << std::endl;
    a += 1;
    std::cout << a << " is prime: " << g_sieve.isPrimeExt(a) << std::endl;
    a += 1;
    std::cout << a << " is prime: " << g_sieve.isPrimeExt(a) << std::endl;
}

void testCalc1(int num)
{
    PrimeFactors pf(num);
    std::cout << "number: " << num << " solutions:" << solutionCount(pf.m_factorGroups) << std::endl;
}
__int64 solve()
{
    init();

    // test();

    //testCalc1(4 * 9 * 3);

    //testCalc1(2);
    //testCalc1(3);
    //testCalc1(4);
    //testCalc1(5);
    //testCalc1(6);
    //testCalc1(7);
    //testCalc1(1260);


    // 1/a + 1/b = 1/n
    // number of divisors of n
    // smallest n with number of divisors > 1E6
    //
    // if n = p1^a1 * p2^a2 * ... * pn^an
    // then number of divisors = (2a1 + 1) * (2a2 + 1) * ... * (2an + 1)
    // number of distinct solutions for a and b = (number of divisors + 1) / 2
    // or number of divisors = 2 * number of distinct solutions - 1
	
    // find smallest n with number of divisors > 1999999
    // number of divisors = 2 * number of distinct solutions - 1
    // number of distinct solutions = (number of divisors + 1) / 2
    // number of divisors = (2a1 + 1) * (2a2 + 1) * ... * (2an + 1)
    // number of divisors = 2 * number of distinct solutions - 1
    // (2a1 + 1) * (2a2 + 1) * ... * (2an + 1) = 2 * ((number of divisors + 1) / 2) - 1
    // (2a1 + 1) * (2a2 + 1) * ... * (2an + 1) = number of divisors
    // (2a1 + 1) * (2a2 + 1) * ... * (2an + 1) = 1999999

    // 1. find a distribution pattern with smallest primes with smalles number above 1999999

    int minSolutionCount = 4000000;
    std::vector<std::pair<int,int>> ttemp = findPrimeGroups(minSolutionCount);
    __int64 _solutionCount = solutionCount(ttemp);
    __int64 _solution = calcNumber(ttemp);

    //std::cout << "number: " << _solution << std::endl;
    std::cout << "number: " << _solution << " solutions: " << solutionCount(ttemp) << std::endl;


    // 2. find the smallest number with that distribution pattern

	
    return _solution;
}


int main()
{
    time_t t1 = clock();
    __int64 solution = solve();
    time_t t2 = clock();
    int ms = (int)(t2 - t1) * 1000 / CLOCKS_PER_SEC;

    std::cout << "solution: " << solution << std::endl << "duration: " << ms << " ms" << std::endl;
}
