
#include <iostream>
#include <chrono>


const uint32_t MAX_N = 999'999'999;
const uint32_t FACTOR = 7;


const uint32_t LEVELS = 11;

uint64_t nRowsCount(uint32_t rows, uint32_t factor, bool invert = true)
{
	uint32_t a_factorPow[LEVELS];
	uint32_t a_digits[LEVELS];
	uint32_t a_count[LEVELS];
	uint32_t ix_high = 0;

	uint32_t scalef = 1;
	for (uint32_t i = 0; i < LEVELS; i++)
	{
		a_digits[i] = 0;
		a_count[i] = 0;
		a_factorPow[i] = scalef;
		scalef *= FACTOR;
	}


	int64_t sum = 0;

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
		bool carry = false;
		uint32_t inc_ix = 0;
		uint32_t d = a_digits[inc_ix] + 1;
		a_digits[inc_ix] = d > 6 ? 0 : d;
		while (d == factor)
		{
			inc_ix += 1;
			d = a_digits[inc_ix] + 1;
			a_digits[inc_ix] = d > 6 ? 0 : d;
		}
		if (inc_ix > ix_high)
			ix_high = inc_ix;
	}

	return sum;
}


int64_t solve()
{
	return nRowsCount(1'000'000'000, 7);
	// 2129970655314432
}

int main()
{

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
