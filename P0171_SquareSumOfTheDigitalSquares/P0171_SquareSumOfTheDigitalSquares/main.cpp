

#include <iostream>
#include <chrono>
#include <cmath>
#include <cassert>

#define DIGITCOUNT 20
const int64_t MODLIMIT = 1000000000;

std::vector<int8_t> digitArr(DIGITCOUNT + 1, -1);
int assignCounter = 0;
int sq_sum = 0;
int square_count = 0;
std::vector<int64_t>mem_comb((DIGITCOUNT + 1 ) * (DIGITCOUNT + 1), 0);
int64_t modwrap_count = 0;
int64_t modwrap_count_z = 0;
int64_t modwrap_count_s = 0;
int64_t modwrap_count_dc = 0;
int64_t modwrap_count_dc2 = 0;


void printDigitArr(int  len)
{
	for (int i = 0; i < len; i++)
		std::cout << (int)(digitArr[i]);
}

bool is_square(int num)
{
	int root_test = (int)floor(sqrt(num) + 0.1);
	if (root_test * root_test == num)
		return true;
	else
		return false;
}

inline int64_t mem_comb_index(int64_t n, int64_t k)
{
	return n * (DIGITCOUNT + 1) + k;
}

// calculates n! / (k! * (n-k)!)
int64_t comb(int64_t n, int64_t k)
{
	assert(n <= DIGITCOUNT && k <= DIGITCOUNT);

	// we do not call with k > n, so we do not need to check for that.
	//if (k > n)
	//	return 0;
	if (k == 0 || k == n)
		return 1;
	int64_t mem = mem_comb[mem_comb_index(n, k)];
	if (mem != 0)
		return mem;
	else
	{
		int64_t result = 1;
		for (int i = 1; i <= k; i++)
		{
			result *= n - (k - i);
			result /= i;
		}
		mem_comb[mem_comb_index(n, k)] = result;
		return result;
	}
}

//// calculates n! / (k! * (n-k)!)
//int64_t comb(int64_t n, int64_t k)
//{
//	assert(n <= DIGITCOUNT && k <= DIGITCOUNT);
//
//	// we do not call with k > n, so we do not need to check for that.
//	//if (k > n)
//	//	return 0;
//	if (k == 0 || k == n)
//		return 1;
//	{
//		int64_t result = 1;
//		for (int i = 1; i <= k; i++)
//		{
//			result *= n - (k - i);
//			result /= i;
//		}
//		return result;
//	}
//}

int64_t	zdistr(int len)
{
	// given a number with non-zero digits, with len digits.
	// calculation successively numbers with 1,2,...,max_zero_count zeroes added, and sum up the possibilities.
	// maximum number of digits is DIGITCOUNT, so max number of zeroes is DIGITCOUNT- len.

	//if (len == DIGITCOUNT)
	//	return 0;

	int max_zero_count = DIGITCOUNT - len;
	int64_t result = 0;
	int k = len;

	for (int n = 1; n <= max_zero_count; n++)
	{
		result += comb(n + k - 1, k - 1);
	}

	if (result >= MODLIMIT)
		result %= MODLIMIT;

	return result;
}

int64_t digitcombinations(int startix, int len)
{
	// no sequences with 0 will be input, instead calculate the remaining possibilites with
	// zeroes, but not leading zeroes. Each input is unique, so all different counts of
	// addes 0 are calculated in one call per call of digitcombinations.

	int64_t result = 1;
	// find out all possible 10 based numbers which can be built with
	// first len digits in digitArr.
	//if (len <= 1)
	//	result = 1;

	//if (digitArr[startix] == 0)
	//	std::cout << "he!" << std::endl;

	//assert(digitArr[startix] != 0);

	int8_t first_digit = digitArr[startix];

	assert(first_digit != 0);

	int ix = 1;
	while (ix < len && digitArr[ix+startix] == first_digit)
		ix++;
	if (ix == len)
	{
		result = 1;
		// all digits are the same, so we have only one combination.
	}
	else
	{
		// we have at least two different digits, so we need to calculate the combinations.
		while (ix < len)
		{
			int ix_marker = ix;
			int8_t current_digit = digitArr[ix + startix];
			while (ix < len && digitArr[ix + startix] == current_digit)
				ix++;
			int current_count = ix - ix_marker;
			result *= comb(ix, current_count);
			if (result >= MODLIMIT)
			{
				result %= MODLIMIT;
				modwrap_count += 1;
				modwrap_count_dc += 1;
			}
		}
	}

	if (len==DIGITCOUNT)
		return result;

	// handle zeroes:
	result += result * zdistr(len);

	if (result >= MODLIMIT)
	{
		result %= MODLIMIT;
		modwrap_count += 1;
		modwrap_count_dc2 += 1;
	}

	return result;
}

int64_t countVariations(int digitLimit, int pos, int sq_sum)
{
	assert(pos < DIGITCOUNT);
	
	int64_t sum = 0;
	for (int i = digitLimit; i <= 9; i++)
	{
		digitArr[pos] = i;
		assignCounter += 1;

		int sq_new_sum = sq_sum + i * i;

		//if (sq_new_sum == 100)
		//	std::cout << "hier!" << std::endl;
		//if (lastPos == 17 && i==7)
		//	std::cout << "hier!" << std::endl;

		if (is_square(sq_new_sum))
		{
			square_count += 1;
			sum += digitcombinations(0, pos+1);
			if (sum >= MODLIMIT)
			{
				sum %= MODLIMIT;
				modwrap_count += 1;
				modwrap_count_s += 1;
			}
			//std::cout << "SQUARE FOUND: " << sq_new_sum << "(";
			//printDigitArr(pos+1);
			//std::cout << ")" << std::endl;
		}

		if ((assignCounter % 100000) == 0)
		{
			std::cout << assignCounter << ": ";
			printDigitArr(21);
			std::cout << std::endl;
		}

		if (pos < (DIGITCOUNT -1))
		{
			sum += countVariations(i, pos+1, sq_new_sum);
			if (sum >= MODLIMIT)
			{
				sum %= MODLIMIT;
				modwrap_count += 1;
				modwrap_count_s += 1;
			}
		}
	}
	if (sum >= MODLIMIT)
	{
		sum %= MODLIMIT;
		modwrap_count += 1;
		modwrap_count_s += 1;
	}
	return sum;
}


void test_setarr(int8_t arr[], int len)
{
	for (int i = 0; i < len; i++)
		digitArr[i] = arr[i];
}

void test()
{

	std::cout << zdistr(2) << std::endl, std::cout << zdistr(3) << std::endl;

	int8_t test_arr[]{ 1,2 };
	test_setarr(test_arr, 2);
	int64_t test_comb = digitcombinations(0,2);
	std::cout << "test_comb: " << test_comb << std::endl;


	//int64_t count = countVariations(1, 0, 0);	// start with 1, treat leading 0 separately for every found solution in digitcombinations function.
	//std::cout << "variations(0,"<< DIGITCOUNT << ") = " << count % MODLIMIT << std::endl;
	//std::cout << "squares found:" << square_count << std::endl;

	//int8_t test_arr[]{ 1,1,1,2,2,3 };
	//test_setarr(test_arr, 6);
	//int64_t test_comb = digitcombinations(0,6);
	//std::cout << "test_comb: " << test_comb << std::endl;
	//
	//int8_t test_arr2[]{ 1,1,1,2,2,3,5 };
	//test_setarr(test_arr2, 7);
	//int64_t test_comb2 = digitcombinations(0, 7);
	//std::cout << "test_comb: " << test_comb2 << std::endl;

	//int8_t test_arr3[]{ 1,1,1,2,2,2,2 };
	//test_setarr(test_arr3, 7);
	//int64_t test_comb3 = digitcombinations(0, 7);
	//std::cout << "test_comb: " << test_comb3 << std::endl;

	//int8_t test_arr4[]{ 1,1,1,1,2,2,3 };
	//test_setarr(test_arr4, 7);
	//int64_t test_comb4 = digitcombinations(1, 6);
	//std::cout << "test_comb: " << test_comb4 << std::endl;

	//std::cout << "{ 0,1,1,1,2,2,3 }" << std::endl;
	//int8_t test_arr5[]{ 0,1,1,1,2,2,3 };
	//test_setarr(test_arr5, 7);
	//int64_t test_comb5 = digitcombinations(0, 7);
	//std::cout << "test_comb: " << test_comb5 << std::endl;

	//std::cout << "{ 0,0,1,1,1,2,2,3 }" << std::endl;
	//int8_t test_arr6[]{ 0,0,1,1,1,2,2,3 };
	//test_setarr(test_arr6, 8);
	//int64_t test_comb6 = digitcombinations(0, 8);
	//std::cout << "test_comb: " << test_comb6 << std::endl;

	//std::cout << "{ 0,1,2,2,2,3,3,4 }" << std::endl;
	//int8_t test_arr7[]{ 0,1,2,2,2,3,3,4 };
	//test_setarr(test_arr7, 8);
	//int64_t test_comb7 = digitcombinations(0, 8);
	//std::cout << "test_comb: " << test_comb7 << std::endl;

	//int8_t test_arr8[]{ 1,1,1,2,2,3 };
	//test_setarr(test_arr8, 6);
	//int64_t test_comb8 = digitcombinations(0, 6);
	//std::cout << "test_comb: " << test_comb8 << std::endl;

	//std::cout << "{ 1,2 }" << std::endl;
	//int8_t test_arr9[]{ 1,2 };
	//test_setarr(test_arr9, 2);
	//int64_t test_comb9 = digitcombinations(0, 2);

	//std::cout << "test_comb: " << test_comb9 << std::endl;

	//std::cout << "{ 1,1 }" << std::endl;
	//int8_t test_arr10[]{ 1,1 };
	//test_setarr(test_arr10, 2);
	//int64_t test_comb10 = digitcombinations(0, 2);
	//std::cout << "test_comb: " << test_comb10 << std::endl;

	//std::cout << "{ 1 }" << std::endl;
	//int8_t test_arr11[]{ 1 };
	//test_setarr(test_arr11, 1);
	//int64_t test_comb11 = digitcombinations(0, 1);
	//std::cout << "test_comb: " << test_comb11 << std::endl;


}

int64_t solve()
{
	test();

	int64_t count = (countVariations(1, 0, 0)) % MODLIMIT;	// start with 1, treat leading 0 separately for every found solution in digitcombinations function.
	std::cout << "variations(0," << DIGITCOUNT << ") = " << count << std::endl;
	std::cout << "squares found:" << square_count << std::endl;

	std::cout << "modwrap_count: " << modwrap_count << std::endl;
	std::cout << "modwrap_count_z: " << modwrap_count_z << std::endl;
	std::cout << "modwrap_count_s: " << modwrap_count_s << std::endl;
	std::cout << "modwrap_count_dc: " << modwrap_count_dc << std::endl;
	std::cout << "modwrap_count_dc2: " << modwrap_count_dc2 << std::endl;

	return count;
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
