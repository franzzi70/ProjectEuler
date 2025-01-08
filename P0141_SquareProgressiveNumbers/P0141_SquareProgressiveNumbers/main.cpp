
#include <iostream>
#include <chrono>
#include <set>
#include <vector>
#include "primes.h"

//const int64_t MAX = 1'000'000'000'000;
//const int64_t MAX = 100'000'000;
const int64_t MAX = 100'000'000;
const int THRESHOLD_N = MAX + 1; // sqrt(MAX) + 1;

typedef uint64_t type64;

primes::PrimesSieve g_primes(THRESHOLD_N + 100);

const int PRIMEARRSIZE = THRESHOLD_N / 5 + (int)round(THRESHOLD_N / log(THRESHOLD_N));
std::vector<int> g_primeArray(PRIMEARRSIZE);

class PrimeFactorization
{
public:
	PrimeFactorization()
	{
		ms_factorCount = 0;
		ms_divisorCount = 0;
	}
	static int ms_factorCount;
	static int ms_divisorCount;
	//static std::pair<int, int> m_primeFactors[];
	static std::vector<std::pair<int, int>> ms_primeFactors;
	static std::vector<int> ms_divisors;

	static void calculateFactors(int n)
	{
		if (g_primes.isPrime(n))
		{
			ms_primeFactors[0].first = n;
			ms_primeFactors[0].second = 1;
			ms_factorCount = 1;
			return;
		}
		int rest = n;
		int primeIx = 0;
		int prime = g_primeArray[primeIx];
		int factorCount = 0;
		while (rest > 1)
		{
			if (rest % prime == 0)
			{
				int primeCount = 0;
				do {
					rest /= prime;
					primeCount += 1;
				} while (rest % prime == 0);
				ms_primeFactors[factorCount].first = prime;
				ms_primeFactors[factorCount].second = primeCount;
				factorCount++;
			}
			primeIx += 1;
			prime = g_primeArray[primeIx];
			//prime = g_primes.nextPrime(prime);
		}
		ms_factorCount = factorCount;
	}

	static int calculateDivisors(int n)
	{
		calculateFactors(n);
		ms_divisorCount = 0;
		_calulateDivisors(1, 0);
		return ms_divisorCount;
	}

private:
	static void _calulateDivisors(int prod, int index)
	{
		if (index == ms_factorCount)
		{
			ms_divisors[ms_divisorCount] = prod;
			ms_divisorCount += 1;
			return;
		}
		int prime = ms_primeFactors[index].first;
		int primeCount = ms_primeFactors[index].second;
		for (int i = 0; i <= primeCount; i++)
		{
			_calulateDivisors(prod, index + 1);
			prod *= prime;
		}
	}
};

//const int FACTORARRAYSIZE = 2 + (int)round(log(THRESHOLD_N + 1));
// 2 + (int)round(log(THRESHOLD_N + 1) / log(2));
//std::pair<int, int> PrimeFactorization::m_primeFactors[100 + THRESHOLD_N / 1000];

std::vector<std::pair<int, int>> PrimeFactorization::ms_primeFactors(2 + (int)round(log(THRESHOLD_N + 1) / log(2)));
std::vector<int> PrimeFactorization::ms_divisors(2 + (int)round(sqrt(THRESHOLD_N + 1)));
int PrimeFactorization::ms_factorCount = 0;
int PrimeFactorization::ms_divisorCount = 0;

void init()
{
	int prime = 2;
	int primeIx = 0;
	while (prime <= THRESHOLD_N)
	{
		g_primeArray[primeIx] = prime;
		prime = g_primes.nextPrime(prime);
		primeIx += 1;
	}
}

int64_t solve()
{
	init();

	//double testD = 3.99;
	//type64 testI = (type64)(0.1 + sqrt(testD));
	//std::cout << "testI: " << testI << std::endl;

	int64_t sum = 0;
	int64_t step_count = 0;

	std::set <type64> squares;

	std::cout << "n" << "\t" << "q" << "\t" << "r" << "\t" << "d" << "\t" << "sqrt(n)" << "\t" << "d/sqrt(n)" << "\t" << "q/sqrt(n)" << "\t" << "r/sqrt(n)" << "\t" << "step count" << std::endl;
	int l = 3;
	for (int n = 1; n < MAX; n += l, l += 2)
	{
		//type64 d_max = (type64)sqrt(n + 1);
		type64 d_max = n / 2;
		type64 d;
		type64 q;
		type64 r;
		//for (d = 4; d <= d_max; d += 1)

		for (d = 1; d < n; d += 1)
		{
			step_count += 1;

			q = _udiv128(0, n, d, &r);
			if (r == 0)
				continue;

			// now check for geometric progression
			int64_t q_sqr = q * q;
			if ((q_sqr) / r == d)
			{
				if (q_sqr % r != 0)
					continue;

				sum += n;


				// m^2 = d * q + r;

				// m = (d * q + r) / m;
				// m = (d*q)/m + r/m;
				// r * f = q
				// q * f = d

				// m^2 = (r*f*f) * (r*f) + r
				// r^2 * f^3 + r - m^2 = 0
				// solve for r
				// r = (sqrt(4*f^3*m^2 +1)-1)/2*f^3

				// m^2 = (q*f) * q + q/f
				// f * m^2 = q^2 + q
				// q^2 + q - f * m^2 = 0
				// solve for q
				// q = ( -1 + sqrt(1+4*f*m^2) ) / 2


				//std::cout << "n: " << n << " q: " << q << " r: " << r << " d: " << d << " sqrt(n): " << sqrt(n) << std::endl;
				std::cout << n << "\t" << q << "\t" << r << "\t" << d << "\t" << sqrt(n) << "\t" << d / sqrt(n) << "\t" << q / sqrt(n) << "\t" << r / sqrt(n) << "\t" << (double)step_count << std::endl;
				PrimeFactorization::calculateDivisors(d);
				// list	all divisors of d
				//std::cout << "divisors of d: ";
				//for (int i = 0; i < PrimeFactorization::ms_divisorCount; i++)
				//{
				//	std::cout << PrimeFactorization::ms_divisors[i] << " ";
				//}
				// list all prime factors of d
				std::cout << std::endl << "prime factors of d: ";
				for (int i = 0; i < PrimeFactorization::ms_factorCount; i++)
				{
					std::cout << PrimeFactorization::ms_primeFactors[i].first << "^" << PrimeFactorization::ms_primeFactors[i].second << " ";
				}

				PrimeFactorization::calculateDivisors(r);
				// list	all divisors of r
				//std::cout << std::endl << "prime factors of r: ";
				//std::cout << std::endl << "divisors of r: ";
				//for (int i = 0; i < PrimeFactorization::ms_divisorCount; i++)
				//{
				//	std::cout << PrimeFactorization::ms_divisors[i] << " ";
				//}
				// list all prime factors of r
				std::cout << std::endl << "prime factors of r: ";
				for (int i = 0; i < PrimeFactorization::ms_factorCount; i++)
				{
					std::cout << PrimeFactorization::ms_primeFactors[i].first << "^" << PrimeFactorization::ms_primeFactors[i].second << " ";
				}

				PrimeFactorization::calculateDivisors(q);
				// list	all divisors of q
				//std::cout << std::endl << "prime factors of q: ";
				//std::cout << std::endl << "divisors of q: ";
				//for (int i = 0; i < PrimeFactorization::ms_divisorCount; i++)
				//{
				//	std::cout << PrimeFactorization::ms_divisors[i] << " ";
				//}
				// list all prime factors of q
				std::cout << std::endl << "prime factors of q: ";
				for (int i = 0; i < PrimeFactorization::ms_factorCount; i++)
				{
					std::cout << PrimeFactorization::ms_primeFactors[i].first << "^" << PrimeFactorization::ms_primeFactors[i].second << " ";
				}
				std::cout << std::endl;
			}
			//return 0;
			// hier weiter
		}
	}
	return sum;
}


//int64_t _div128(
//	int64_t highDividend,
//	int64_t lowDividend,
//	int64_t divisor,
//	int64_t* remainder
//);

int main()
{

	auto t1 = std::chrono::high_resolution_clock::now();
	int64_t solution = solve();
	auto t2 = std::chrono::high_resolution_clock::now();
	auto microSec = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
	std::cout << "solution: " << solution << std::endl << "duration: " << microSec << " micro seconds (" << ms << " ms)" << std::endl;
	if (microSec > 300'000'000)
	{
		std::cout << "(" << ((float)microSec) / 60'000'000 << " minutes )" << std::endl;
	}
}
