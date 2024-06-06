
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
    std::vector<std::pair<__int64, int>>
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
				m_factorGroups.push_back(std::pair<__int64, int>(_prime, 1));
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

__int64 factorCount(std::vector<std::pair<__int64, int>> &fGroups)
{
    __int64 result = 1;
    for (auto it = fGroups.begin(); it != fGroups.end(); it++)
	{
		result *= (it->second * 2 + 1);
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

__int64 solutionCount(std::vector<std::pair<__int64, int>>& fGroups)
{
    return (factorCount(fGroups) + 1) / 2;
}

class Node {
public:
    Node(int n)
	{
        int initialCount = 0;
        __int64 initialTerm = 1;
        //__int64 nTn = (__int64)n* (__int64)n;
        __int64 factorMinCount = n * 2 - 1;
        while (initialTerm < factorMinCount)
		{
			initialTerm *= 3;
			initialCount += 1;
		}
        m_primegroups = std::vector<int>(initialCount, 1);
        m_goalSolutionCount = n;
        m_sequenceLen = initialCount;
	}
    
    bool move()
    {
        return move(m_sequenceLen-1);
    }

    bool move(int position)
    {
        int _position = position;   // take position from parameter
        int _value = m_primegroups[_position];
        int ix = 0;
        // try to find a position to move to from left to right:
        std::pair <int, int> start_bak = std::pair<int, int>(_position, _value);
        while (true)
        {
            std::pair <int, int> goal_bak = std::pair<int, int>(ix, m_primegroups[ix]);
            int len_bak = m_sequenceLen;
            if (_position == m_sequenceLen-1 && _value == 1)
            {
                m_sequenceLen -= 1;
            }
            m_primegroups[_position] -= 1;
            m_primegroups[ix] += 1;
            __int64 _solutionCount = solutionCount(m_primegroups);

            if (_solutionCount > m_goalSolutionCount)
            {
                return true;    // moved.
            }
            else
            {
                // restore values:
                m_primegroups[start_bak.first] = start_bak.second;
                m_primegroups[goal_bak.first] = goal_bak.second;
                m_sequenceLen = len_bak;

                int thisGroupValue = goal_bak.second;
                while (ix < m_sequenceLen && m_primegroups[ix] == thisGroupValue)
				{
					ix += 1;
				}
                if (ix == m_sequenceLen || ix >= _position)
                {
                    return false;
                }
            }
        }
    }

    int m_sequenceLen;
    int m_position;
    int m_goalSolutionCount;
    std::vector<int> m_primegroups;
};

std::vector<int> findPattern(int solutionCount)
{

    Node n(solutionCount);
    while (n.move())
    {
        std::cout << "moved" << std::endl;
    }


    return n.m_primegroups;
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

    int solutionCount = 4000000;
    std::vector<int> ttemp = findPattern(solutionCount);



    // 2. find the smallest number with that distribution pattern


    return 0;
}


int main()
{
    time_t t1 = clock();
    __int64 solution = solve();
    time_t t2 = clock();
    int ms = (int)(t2 - t1) * 1000 / CLOCKS_PER_SEC;

    std::cout << "solution: " << solution << std::endl << "duration: " << ms << " ms" << std::endl;
}
