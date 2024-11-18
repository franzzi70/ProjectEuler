
#include <iostream>
#include <chrono>
#include <vector>
#include "primes.h"


//  (1) x ^ 2 - y ^ 2 - z ^ 2 = n
//  If x, y, and z are consecutive terms of an arithmetic progression, then
//  (2) x ^ 2 - (x - a) ^ 2 - (x - 2a) ^ 2 = n
//      x ^ 2 - (x ^ 2 - 2ax + a ^ 2) - (x ^ 2 - 4ax + 4a ^ 2) = n
//      x ^ 2 - x ^ 2 + 2ax - a ^ 2 - x ^ 2 + 4ax - 4a ^ 2 = n
//      6ax - 5a^2 - x^2 = n
//      x^2 - 6ax + 5a^2 = -n
//  Expanding, simplifying and rearranging the latter gives :
//  (3) (x - 5a) * (x - a) = -n
//      x ^ 2 - 10ax + 25a ^ 2 = n

//  If x, y, and z are consecutive terms of an arithmetic progression, then
//  (2) (y+a)^2 - y^2 - (y-a)^2 = n
//      y^2 + 2ay + a^2 - y^2 - y^2 + 2ay - a^2 = n
//      -y^2 +4ay = n
//          -y^2 + 4ay -2a = n - 2a
//          (y - 2a) * (y + 2a) = n - 2a
//      y(4a - y) = n

//  If x, y, and z are consecutive terms of an arithmetic progression, then
//  (2) (z+2a)^2 - (z+a)^2 - z^2 = n
//      z^2 + 4az + 4a^2 - z^2 - 2az - a^2 - z^2 = n
//      -z^2 + 2az + 3a^2 = n
//  Expanding, simplifying and rearranging the latter gives :
//  (3)  -z^2 + 2az + 3a^2 = n
//      (z - 3a) * (z + a) = -n

const int THRESHOLD_N = 50'000'000;

//primes::PrimesSieve g_primes(THRESHOLD_N + 100);
//
//const int PRIMEARRSIZE = THRESHOLD_N / 5 + (int)round(THRESHOLD_N / log(THRESHOLD_N));
//std::vector<int> g_primeArray(PRIMEARRSIZE);
//
//
//class PrimeFactorization
//{
//public:
//	PrimeFactorization()
//	{
//		m_factorCount = 0;
//		m_divisorCount = 0;
//	}
//	static int m_factorCount;
//	static int m_divisorCount;
//	//static std::pair<int, int> m_primeFactors[];
//	static std::vector<std::pair<int, int>> m_primeFactors;
//	static std::vector<int> m_divisors;
//
//	static void calculateFactors(int n)
//	{
//		if (g_primes.isPrime(n))
//		{
//			m_primeFactors[0].first = n;
//			m_primeFactors[0].second = 1;
//			m_factorCount = 1;
//			return;
//		}
//		int rest = n;
//		int primeIx = 0;
//		int prime = g_primeArray[primeIx];
//		int factorCount = 0;
//		while (rest > 1)
//		{
//			if (rest % prime == 0)
//			{
//				int primeCount = 0;
//				do {
//					rest /= prime;
//					primeCount += 1;
//				} while (rest % prime == 0);
//				m_primeFactors[factorCount].first = prime;
//				m_primeFactors[factorCount].second = primeCount;
//				factorCount++;
//			}
//			primeIx += 1;
//			prime = g_primeArray[primeIx];
//			//prime = g_primes.nextPrime(prime);
//		}
//		m_factorCount = factorCount;
//	}
//
//	static int calculateDivisors(int n)
//	{
//		calculateFactors(n);
//		m_divisorCount = 0;
//		_calulateDivisors(1, 0);
//		return m_divisorCount;
//	}
//
//private:
//	static void _calulateDivisors(int prod, int index)
//	{
//		if (index == m_factorCount)
//		{
//			m_divisors[m_divisorCount] = prod;
//			m_divisorCount += 1;
//			return;
//		}
//		int prime = m_primeFactors[index].first;
//		int primeCount = m_primeFactors[index].second;
//		for (int i = 0; i <= primeCount; i++)
//		{
//			_calulateDivisors(prod, index + 1);
//			prod *= prime;
//		}
//	}
//};

//const int FACTORARRAYSIZE = 2 + (int)round(log(THRESHOLD_N + 1));
// 2 + (int)round(log(THRESHOLD_N + 1) / log(2));
//std::pair<int, int> PrimeFactorization::m_primeFactors[100 + THRESHOLD_N / 1000];

//std::vector<std::pair<int, int>> PrimeFactorization::m_primeFactors(2 + (int)round(log(THRESHOLD_N + 1) / log(2)));
//std::vector<int> PrimeFactorization::m_divisors(2 + (int)round(sqrt(THRESHOLD_N + 1)));
//int PrimeFactorization::m_factorCount = 0;
//int PrimeFactorization::m_divisorCount = 0;

//void init()
//{
//	int prime = 2;
//	int primeIx = 0;
//	while (prime <= THRESHOLD_N)
//	{
//		g_primeArray[primeIx] = prime;
//		prime = g_primes.nextPrime(prime);
//		primeIx += 1;
//	}
//}
//
//__int64 solve_notworking()
//{
//	init();
//
//	int foundMatchCount = 0;
//
//	__int64 divCount = 0;
//	for (int n = 10000000; n < THRESHOLD_N; n++)
//	{
//		if (n % 10'000 == 0)
//		{
//			std::cout << "n: " << n;
//			std::cout << " divisorCount: " << divCount << std::endl;
//		}
//		divCount += PrimeFactorization::calculateDivisors(n);
//
//		//std::cout << "n: " << n << std::endl;
//		//for (int divIx = 0; divIx < PrimeFactorization::m_divisorCount; divIx++)
//		//{
//		//	std::cout << PrimeFactorization::m_divisors[divIx] << " ";
//		//}
//		//std::cout << std::endl;
//
//
//		int nMatchCount = 0;
//
//		for (int dix = 0; dix < PrimeFactorization::m_divisorCount; dix++)
//		{
//			int y = PrimeFactorization::m_divisors[dix];
//			// y(4a - y) = n
//			// 4ay - y^2 = n
//			// 4a = (n+ y^2) / (y*4)
//			int d = (n + y * y) / (4 * y);
//			//std::cout << "\td: " << d << std::endl;
//			if (y > d)
//			{
//				if (d > 0 && y * (4 * d - y) == n)
//				{
//					//std::cout << "n: " << n
//					//	<< " x: " << y + d
//					//	<< " y: " << y
//					//	<< " z: " << y - d
//					//	<< " a: " << d << std::endl;
//
//					nMatchCount += 1;
//					if (nMatchCount > 1)
//					{
//						break;
//					}
//				}
//			}
//		}
//		if (nMatchCount == 1)
//		{
//			foundMatchCount += 1;
//
//			//std::cout << "n: " << n << " is prime: " << g_primes.isPrime(n) << std::endl;
//			//std::cout << "prime factors: ";
//			//for (int fix = 0; fix < PrimeFactorization::m_factorCount; fix++)
//			//{
//			//	std::cout << PrimeFactorization::m_primeFactors[fix].first << "^" << PrimeFactorization::m_primeFactors[fix].second << " ";
//			//}
//			//std::cout << std::endl;
//			//std::cout << "divisors: ";
//			//for (int dix = 0; dix < PrimeFactorization::m_divisorCount; dix++)
//			//{
//			//	std::cout << PrimeFactorization::m_divisors[dix] << " ";
//			//}
//			//std::cout << std::endl;
//
//			if (foundMatchCount == 100000)
//			{
//				return 0;
//			}
//		}
//		//     if (i == 41333451)
//		//     {
//		//         for (int j = 0; j < PrimeFactorization::m_divisorCount; j++)
//		//         {
//		//             std::cout << PrimeFactorization::m_divisors[j] << " ";
//		//         }
//				 //std::cout << std::endl;
//		//     }
//	}
//
//	//	PrimeFactorization::calculateDivisors(100);
//
//	std::cout << "divisorCount: " << divCount << std::endl;
//
//	return 0;
//}

__int64 solve()
{
	__int64 stepCount = 0;
	int solutionCount = 0;
	std::vector<int> countArray(THRESHOLD_N + 1);
	int foundCount = 0;
	for (__int64 y = 2; y < THRESHOLD_N; y++)
	{
		// y(4a - y) = n
		// 4ay - y^2 = n
		// 4*a > y
		__int64 minA = y / 4 + 1;
		for (__int64 a = minA; a < y; a++)
		{
			stepCount += 1;
			__int64 n = y * (4 * a - y);
			if (n >= THRESHOLD_N)
			{
				break;
			}
			if (n > 0)
			{
				foundCount += 1;
				countArray[n] += 1;
			}
		}
	}
	for (int i = 0; i < THRESHOLD_N; i++)
	{
		if (countArray[i] == 1)
		{
			solutionCount += 1;
		}
	}
	//std::cout << "stepCount: " << stepCount << std::endl;
	return solutionCount;
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
