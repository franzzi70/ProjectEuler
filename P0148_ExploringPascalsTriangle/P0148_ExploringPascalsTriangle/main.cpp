
#include <iostream>
#include <chrono>
#include <vector>

uint32_t nRowCount_vec(uint32_t n, uint32_t factor, bool invert);

const uint32_t MAX_N = 999'999'999;
const uint32_t FACTOR = 7;
std::vector<int> g_factorPow;

constexpr uint32_t ilog(uint32_t n, uint32_t base) {
	return n < base ? 0 : 1 + ilog(n / base, base);
}
constexpr uint32_t LEVELS = ilog(MAX_N, FACTOR) + 1;

void init()
{
	uint32_t f = 1;
	uint32_t fp = f * FACTOR;
	uint32_t ix = 0;
	g_factorPow.push_back(1);
	while (fp < MAX_N)
	{
		g_factorPow.push_back(fp);
		f = fp;
		fp = f * FACTOR;
		ix++;
	}
}


int factorInFactorial(uint32_t fac, uint32_t factor)
{
    uint32_t cmp = factor;
	uint32_t count = 0;
    while (cmp <= fac)
    {
		count += fac / cmp;
		cmp *= factor;
    }
	return count;
}


bool isDivP(uint32_t n, uint32_t k, uint32_t factor)
{
	return factorInFactorial(n, factor) > factorInFactorial(k, factor) + factorInFactorial(n - k, factor);
}

uint32_t testNRowCount(uint32_t n, uint32_t factor, bool inverse = true)
{
	uint32_t count = 0;
	for (uint32_t k = 0; k <= n; ++k)
	{
		if (isDivP(n, k, factor))
		{
			count++;
		}
	}
	return inverse ?
		n + 1 - count :
		count;
}

uint64_t nRowsCount(uint32_t rows, uint32_t factor, bool invert = true)
{
	uint32_t a_factorPow[LEVELS];
	uint32_t a_digits[LEVELS];
	uint32_t a_count[LEVELS];
	uint32_t ix_high = 0;
	int64_t sum = 0;

	uint32_t scalef = 1;
	for (uint32_t i = 0; i < LEVELS; i++)
	{
		a_digits[i] = 0;
		a_count[i] = 0;
		a_factorPow[i] = scalef;
		scalef *= FACTOR;
	}

	for (uint32_t n = 0; n < rows; n++)
	{

		for (uint32_t i = 0; i < LEVELS; i++)
		{
			a_count[i] = 0;
		}
	
		for (uint32_t ix = ix_high; ix > 0; ix--)
		{
			uint32_t f = a_digits[ix] + a_count[ix];
			int32_t pow = a_factorPow[ix];
			int32_t fCount = pow - (n % pow) - 1;
			a_count[0] += f * fCount;
			for (uint32_t il = ix - 1; il > 0; il--)
			{
				a_count[il] += f * a_digits[il];
			}
		}
		int64_t count = a_count[0];

		sum += invert ?
			n + 1 - count :
			count;

		// update a_digit array with next number n
		uint32_t highNum = factor - 1;
		bool carry = false;
		uint32_t inc_ix=0;
		uint32_t d = a_digits[inc_ix] +1;
		a_digits[inc_ix] = d > highNum ? 0 : d;
		while (d == factor)
		{
			inc_ix += 1;
			d = a_digits[inc_ix] + 1;
			a_digits[inc_ix] = d > highNum ? 0 : d;
		}
		if (inc_ix > ix_high)
			ix_high = inc_ix;
	}

	return sum;
}

uint64_t nRowsCount2(uint32_t rows, uint32_t factor, bool invert = true)
{
	uint32_t a_factorPow[LEVELS];
	uint32_t a_digits[LEVELS];
	uint32_t a_count[LEVELS];
	uint32_t ix_high = 0;
	uint64_t sum = 0;

	uint32_t scalef = 1;
	uint64_t fac_acc = 1;
	for (uint32_t i = 0; i < LEVELS; i++)
	{
		a_digits[i] = 0;
		a_count[i] = 0;
		a_factorPow[i] = scalef;
		scalef *= FACTOR;
	}

	for (uint32_t n = 0; n < rows; n++)
	{

		for (uint32_t i = 0; i < LEVELS; i++)
		{
			a_count[i] = 0;
		}

		for (uint32_t ix = ix_high; ix > 0; ix--)
		{
			uint32_t f = a_digits[ix] + a_count[ix];
			uint32_t pow = a_factorPow[ix];
			uint32_t fCount = pow - (n % pow) - 1;
			a_count[0] += f * fCount;
			for (uint32_t il = ix - 1; il > 0; il--)
			{
				a_count[il] += f * a_digits[il];
			}
		}
		int64_t count = a_count[0];

		sum += invert ?
			n + 1 - count :
			count;

		// update a_digit array with next number n
		uint32_t inc_ix = 0;
		uint32_t d = a_digits[inc_ix] + 1;
		a_digits[inc_ix] = d < factor ? d : 0;
		while (d == factor)
		{
			inc_ix += 1;
			d = a_digits[inc_ix] + 1;
			a_digits[inc_ix] = d < factor ? d : 0;
		}
		if (inc_ix > ix_high)
			ix_high = inc_ix;
	}

	return sum;
}

int32_t initVectorDigits(std::vector<uint32_t>& digits, int32_t n, int32_t factor)
{
	int rest = n;
	int ix = 0;
	while (rest > 0)
	{
		int32_t d = rest % factor;
		digits[ix] = d;
		rest /= factor;
		ix += 1;
	}
	return ix - 1;
}

uint32_t nRowCount_vec(uint32_t n, uint32_t factor, bool invert = true)
{
	if (n < factor)
	{
		return invert ?
			n + 1 :
			0;
	}

	std::vector<uint32_t> v_digits(LEVELS, 0);
	std::vector<uint32_t> v_count(LEVELS, 0);
	int32_t ix_high = initVectorDigits(v_digits, n, factor);


	uint32_t upper_count = 0;

	for (uint32_t ix = ix_high ; ix > 0; ix--)
	{
		uint32_t f = v_digits[ix] + v_count[ix];
		int32_t pow = g_factorPow[ix];
		int32_t fCount = pow - (n % pow) - 1;
		v_count[0] += f * fCount;
		for (uint32_t il = ix - 1; il > 0; il--)
		{
			v_count[il] += f * v_digits[il];
		}
	}
	int32_t count = v_count[0];
	return invert ?
		n + 1 - count :
		count;
}

void test()
{
	//std::cout << "factorInFac(10, 2): " << factorInFactorial(10, 2) << std::endl;
 //   std::cout << "factorInFac(7, 7): " << factorInFactorial(7, 7) << std::endl;
 //   std::cout << "factorInFac(8, 7): " << factorInFactorial(8, 7) << std::endl;
 //   std::cout << "factorInFac(48, 7): " << factorInFactorial(48, 7) << std::endl;
	//std::cout << "factorInFac(49, 7): " << factorInFactorial(49, 7) << std::endl;
 //   std::cout << "factorInFac(50, 7): " << factorInFactorial(50, 7) << std::endl;

	//{ int f = 7, n = 6;  std::cout << "testNRowCount(" << n << "," << f << ") = " << testNRowCount(n, f) << " inverse: " << testNRowCount(n, f, false) << std::endl; }
	//{ int f = 7, n = 7;  std::cout << "testNRowCount(" << n << "," << f << ") = " << testNRowCount(n, f) << " inverse: " << testNRowCount(n, f, false) << std::endl; }
	//{ int f = 7, n = 8;  std::cout << "testNRowCount(" << n << "," << f << ") = " << testNRowCount(n, f) << " inverse: " << testNRowCount(n, f, false) << std::endl; }

	uint64_t nCount = 0;

	/*
	
	bool inverse = false;

	for (uint32_t n = 0; n < 10000; ++n)
	{
		int64_t probNCount = nRowCount_vec(n,7, inverse);
		int64_t testCount = testNRowCount(n, 7, inverse);
		nCount += testNRowCount(n, 7, inverse);
		if (probNCount != testCount)
		{
			std::cout << "mismatch for n = " << n << " : expected: " << testCount << " , but value: " << (int)probNCount << std::endl;
		}
	}

	*/

	//const int32_t row_count = 100;
	//const int32_t row_count = 100'000;
	//const int32_t row_count = 10'000'000;
	//const int32_t row_count = 100'000'000;
	const int32_t row_count = 1'000'000'000;

	//for (uint32_t n = 0; n < row_count; ++n)
	//{
	//	int64_t probNCount = nRowCount_vec(n, 7);
	//	nCount += probNCount;
	//}
	//std::cout << "nCount: " << nCount << std::endl;
	
	int64_t countnRows = nRowsCount(row_count, 7);
	std::cout << "nRowsCount: " << countnRows << std::endl;

}

int64_t solve()
{
    return nRowsCount(1'000'000'000, 7);
	//return nRowsCount2(1'000'000'000, 7);
	//return nRowsCount(100'000'000, 7);
	//return nRowsCount2(100'000'000, 7);
	// 2129970655314432
}

int main()
{
	// std::cout << "LEVELS:" << LEVELS << std::endl;

    auto t1 = std::chrono::high_resolution_clock::now();
    volatile int64_t solution = solve();
    auto t2 = std::chrono::high_resolution_clock::now();
    auto microSec = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
    std::cout << "solution: " << solution << std::endl << "duration: " << microSec << " micro seconds (" << ms << " ms)" << std::endl;
    if (microSec > 300'000'000)
    {
        std::cout << "(" << ((float)microSec) / 60'000'000 << " minutes )" << std::endl;
    }
}
