// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <iostream>
#include <chrono>

//#include <set>
#include <list>

#include <vector>
#include <string>
#include "primes.h"


#define FACTOR_VECSIZE 20

// 1/x + 1/y = 1/n
// x = n + a
// y = n + b
// 1/(n+a) + 1/(n+b) = 1/n

// c*a=b*(a-c)

#define MAXPRIME 10000000
#define PRIMEARRSIZE (MAXPRIME/2)


primes::PrimesSieve g_sieve(MAXPRIME);
int g_primes[PRIMEARRSIZE];
int g_primesCount = 0;

bool isPrime(__int64 num)
{
    return g_sieve.isPrimeExt(num);
}

class PrimeFactors {
public:
    __int64 m_number;
    __int8 m_factorCount;
    std::vector<__int64> m_factors;

    PrimeFactors(__int64 n)
    {
        m_number = n;
        m_factors = std::vector<__int64>(FACTOR_VECSIZE);
        calculateFactors(n);
    }
private:
    void calculateFactors(__int64 n)
    {
        int primeIx = 0;
        int factorCount = 0;
        __int64 testNum = n;

        do
        {
            while (testNum % g_primes[primeIx] != 0) {
                primeIx += 1;
            }
            __int64 _prime = g_primes[primeIx];
            testNum /= _prime;
            m_factors[factorCount] = _prime;
            factorCount += 1;
        }
        while (testNum > 1);
        m_factorCount = factorCount;
    }
};

std::string primeFactorString(PrimeFactors& pf)
{
	//std::cout << "factors for " << pf.m_number << ": ";

    std::string factorString = "(";
    for (int i = 0; i < pf.m_factorCount; i++)
    {
        factorString += std::to_string(pf.m_factors[i]);
        if (i < pf.m_factorCount - 1)
		    factorString += ",";
	}
    factorString += ")";
	return factorString;
}


__int64 maxCountTracker = 0;

int findSolutions_SlowBruteForce(__int64 c, bool shortOutput = false, bool showInput = true)
{
    if (showInput)
    {
        std::cout << "input c = " << c << std::endl;
    }
    std::list<std::pair<__int64, __int64>> solutions;
    int count = 0;
    __int64 a_max = c * 2;
    for (__int64 a = c + 1;
        a <= a_max;
        a++
        )
    {
        __int64 b = a * c / (a - c);
        if ((a + b) * c == a * b)
        {
            if (c * b % (b - c) == 0)
            {
                std::pair<__int64, __int64> testPair = std::make_pair(a, b);

                if (!shortOutput)
                {
                    PrimeFactors pf_a(a);
                    std::string str1 = primeFactorString(pf_a);
                    PrimeFactors pf_b(b);
                    std::string str2 = primeFactorString(pf_b);
                    PrimeFactors pf_c(c);
                    std::string str3 = primeFactorString(pf_c);

                    std::cout << "1/" << a << " + 1/" << b << " = 1/" << c
                        << "   "
                        << str1 << " + "
                        << str2 << " = "
                        << str3
                        << std::endl;
                }
                count++;
                solutions.push_back(testPair);
            }
        }
        if (count > maxCountTracker)
        {
            maxCountTracker = count;
            //std::cout << "c: " << c << " count: " << count << std::endl;
        }
    }

    if (shortOutput)
    {
        PrimeFactors pf_c(c);
        std::string str3 = primeFactorString(pf_c);
        std::cout << "c: " << c << " count: " << count << " " << str3 << std::endl;
    }
    return count;
}

void init()
{
    // init prime array:
    int _maxPrime = g_sieve.m_maxNumber;
    int _primeIx = 0;
    for (int i = 0; i <= _maxPrime; i++)
    {
        if (isPrime(i))
        {
            g_primes[_primeIx] = i;
            _primeIx += 1;
        }
    }
    g_primesCount = _primeIx;
}


__int64 solve_slowBruteForce()
{

    //for (__int64 i = 2; ; i++)
    //{
    //    int solutions = findSolutions_SlowBruteForce(i);
    //    if (i % 10000 == 0)
    //        std::cout << "i: " << i << " solutions: " << solutions << std::endl;
    //    if (solutions > 1000)
    //        return i;
    //}
    // solution: 180180

    //return findSolutions(1260);
    //return findSolutions(1000003);

    std::vector<_int64> testNumbers = { 2, 3, 4, 5, 6, 7, 8, 9, 16, 10, 20, 40, 80, 17, 34, 22, 6, 12, 30, 180, 8, 16, 9, 27, 81, 125, 625 };
    for (auto number : testNumbers)
    {
		__int64 solutions = findSolutions_SlowBruteForce(number);
		std::cout << "solutions for " << number << ":" << solutions << std::endl;
	}

    std::cout << "-----------------" << std::endl;
    std::cout << "-----------------" << std::endl;
    std::cout << "prime patterns" << std::endl;
    std::cout << "-----------------" << std::endl;
    std::cout << "-----------------" << std::endl;


    std::vector<__int64> testNumbers2 = {
            //180,
            //2,
            //3,
            //2 * 3,
            //2 * 2,
            //2 * 3 * 5,
            //2 * 2 * 5,
            //2 * 2 * 2,
            //2 * 3 * 5 * 7,
            //2 * 2 * 5 * 7,
            //2 * 2 * 2 * 7,
            //2 * 2 * 3 * 3,
            //2 * 2 * 2 * 2,
            //2 * 3 * 5 * 7 * 11,
            //2 * 2 * 5 * 7 * 11,
            //2 * 2 * 3 * 3 * 11,
            //2 * 2 * 2 * 7 * 11,
            //2 * 2 * 2 * 3 * 3,
            //2 * 2 * 2 * 2 * 3,
            //2 * 2 * 2 * 2 * 2,


            //2 * 3 * 5 * 7 * 11 * 13,
            //2 * 2 * 5 * 7 * 11 * 13,
            //2 * 2 * 5 * 5 * 11 * 13,
            //2 * 2 * 5 * 5 * 11 * 11,
            //2 * 2 * 5 * 5 * 5 * 13,
            //2 * 2 * 5 * 5 * 5 * 5,
            //2 * 2 * 2 * 5 * 11 * 13,
            //2 * 2 * 2 * 5 * 5 * 13,
            //2 * 2 * 2 * 2 * 5 * 11,
            //2 * 2 * 2 * 2 * 5 * 5,
            //2 * 2 * 2 * 2 * 2 * 5,
            //2 * 2 * 2 * 2 * 2 * 2,


            //2 * 2 * 2 * 2 * 2,
            //2 * 2 * 2 * 2 * 7,
            //2 * 2 * 2 * 5 * 7,
            //2 * 2 * 2 * 3 * 7,
            //2 * 2 * 3 * 5 * 7,
            //2 * 3 * 5 * 7 * 7,
            //2 * 3 * 5 * 5 * 7,
            //2 * 5 * 5 * 7 * 7,
            //2 * 5 * 5 * 5 * 7,
            //2 * 3 * 5 * 7 * 11,
            //2 * 3 * 5 * 7 * 11 * 13,
            //2 * 3 * 5 * 7 * 11 * 13 * 17,
            //2 * 3 * 5 * 7 * 11 * 13 * 13,
            //2 * 3 * 5 * 7 * 11 * 13 * 17 * 19,
            //2 * 3 * 5 * 7 * 11 * 13 * 17 * 19 * 23,
            //// overflow: (__int64)2 * 3 * 5 * 7 * 11 * 13 * 17 * 19 * 23 * 29,
            ////(__int64)2 * 3 * 5 * 7 * 11 * 13 * 17 * 19 * 23 * 29 * 31,
            ////(__int64)2 * 3 * 5 * 7 * 11 * 13 * 17 * 19 * 23 * 29 * 31 * 37,
            //1260,
            //2 * 2 * 2 * 2 * 2 * 2 * 3 * 3 * 3 * 5 * 5,
            //2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2, // 2^20
            //180180
            ////(__int64)2 * 3 * 5 * 7 * 11 * 13 * 17 * 19 * 23 * 29
            //// 29, 31, 37, 41, 43, 47
            ////2 * 3 * 5 * 7 * 11, 13, 17, 19  //, 23, 29, 31, 37, 41, 43, 47
            2 * 3 * 5 * 7 * 11 * 13 * 17,
            2 * 2 * 3 * 3 * 5 * 7 * 11 * 13,
            180180,
            1260,
            2*2*2*3*3*5,
            2*2*2*2*2*3*3
    };


    for (__int64 number : testNumbers2)
    {
        __int64 solutions = findSolutions_SlowBruteForce(number, true, false);
        // std::cout << "solutions for " << number << ":" << solutions << std::endl;
    }

    return 0;
}

// approach: generate (a*b) with maximal number of prime factors
__int64  solve()
{
    init();
    return solve_slowBruteForce();
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
