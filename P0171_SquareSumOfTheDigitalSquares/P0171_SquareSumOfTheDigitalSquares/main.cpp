

#include <iostream>
#include <chrono>
#include <vector>
#include <cmath>
#include <cassert>
#include <unordered_map>
#include <algorithm>

#define DIGITCOUNT 20
const int64_t MODLIMIT = 1'000'000'000;
const int MODSIGLIMIT = (int) floor(0.1 + log(MODLIMIT) / log(10));

std::vector<int8_t> digit_grouped_arr(DIGITCOUNT + 1, -1);
int assignCounter = 0;
int sq_sum = 0;
int square_count = 0;
std::vector<int64_t>mem_comb((DIGITCOUNT + 1 ) * (DIGITCOUNT + 1), 0);
int64_t modwrap_count = 0;
int64_t modwrap_count_z = 0;
int64_t modwrap_count_s = 0;
int64_t modwrap_count_dc = 0;
int64_t modwrap_count_dc2 = 0;
int64_t dbg_sequence_count = 0;
int64_t dbg_sum_groups_count = 0;
int64_t dbg_code_high_used = 0;
int64_t dbg_code_high_stored = 0;
int64_t dbg_code_low_used = 0;
int64_t dbg_code_low_stored = 0;
int64_t dbg_swap_count = 0;
int64_t dbg_unswap_count = 0;

class DigitGroup {
public:
	DigitGroup()
	{
		digit = 0;
		count = 0;
		remaining = 0;
	}
	DigitGroup(int8_t digit, int count)
	{
		this->digit = digit;
		this->count = count;
		this->remaining = count;
	}
	DigitGroup(const DigitGroup& other)
	{
		this->digit = other.digit;
		this->count = other.count;
		this->remaining = other.remaining;
	}

	bool operator < (const DigitGroup& other) const
	{
		if (digit == 0)
			return false;
		if (other.digit == 0)
			return true;
		return remaining > other.remaining;
	}

	int8_t digit;
	int count;
	int remaining;
};

class ModNumbers {
public:
	ModNumbers()
	{
		init_static();

	}

	int64_t  arrToNum(std::vector<int8_t>& v)
	{
		int64_t sum = 0;
		for (int i = 0; i <= MODSIGLIMIT; i++)
		{
			sum += v[i] * digitPow10[i];
		}
		return sum;
	}

private:
	void init_static()
	{
		if (initialized)
			return;

		digitPow10 = std::vector<int64_t>(MODSIGLIMIT+1, 0);
		int64_t f = 1;
		for (int i = 0; i <= MODSIGLIMIT; i++)
		{
			digitPow10[i] = f;
			f *= 10;
		}

		initialized = true;
	}

public:
	static bool initialized;
	static std::vector<int64_t> digitPow10;
};
bool ModNumbers::initialized = false;
std::vector<int64_t> ModNumbers::digitPow10;

int64_t encode_groupscounts(int pos, std::vector<DigitGroup>& groups, int grouplen)
{
	int ix = 0;
	int i = 0;
	static int8_t buf[10];
	//std::array<int8_t, 10> buf;
	while (i < grouplen)
	{
		if (groups[i].remaining != 0)
		{
			if (groups[i].digit != 0)
			{
				buf[ix] = groups[i].remaining;
				ix += 1;
			}
		}
		i++;
	}
	std::sort(buf, buf+ix);
	int64_t val = 0;
	int64_t f = 1;
	for (int i = 0; i < ix; i++)
	{
		val += buf[i] * f;
		f *= 10;
	}
	val += 1'000'000'000 * (int64_t) pos;
	return val;
}


int64_t eval_groups_highdigits(
	int pos,
	int nzcount,
	std::vector<DigitGroup>& groups,
	int8_t groupcount
	)
{
	static std::unordered_map<int64_t, int64_t> mem_highgroups;

	int64_t groups_code = encode_groupscounts(pos, groups, groupcount);
	auto mem_it = mem_highgroups.find(groups_code);
	if (mem_it != mem_highgroups.end())
	{
		dbg_code_high_used += 1;
		int64_t val = mem_it->second;
		return val;
	}

	int64_t sum = 0;
	if (pos < DIGITCOUNT-1)
	{
		assert(nzcount > 0);

		for (int8_t i = 0; i < groupcount; i++)
		{
			if (groups[i].remaining > 0)
			{
				int8_t digit = groups[i].digit;
				groups[i].remaining -= 1;
				int new_nzcount = digit == 0 ? nzcount : nzcount - 1;
				int64_t val = 1;
				if (new_nzcount != 0)
				{
					val = eval_groups_highdigits(pos + 1, new_nzcount, groups, groupcount);
				}
				sum += val;
				groups[i].remaining += 1;
			}
		}
	}
	else
	{
		// filled first MODSIGLIMIT digits, so we can calculate the number of combinations for this distribution of digits.

		dbg_sequence_count += 1;
		mem_highgroups[groups_code] = 1;
		dbg_code_high_stored += 1;
		return 1;
	}

	mem_highgroups[groups_code] = sum;
	dbg_code_high_stored += 1;
	return sum;
}

void set_acc(
	int digitcount,
	std::vector<DigitGroup>& groups,
	int8_t groupcount,
	std::vector<int8_t>& vseq,
	std::vector<int64_t>& vacc,
	int64_t rcount
)
{
	int64_t f10 = 1;
	for (int i = 0; i < digitcount; i++)
	{
		int8_t digit = vseq[i];
		if (digit != 0)
		{
			for (int j = 0; j < groupcount; j++)
			{
				if (groups[j].digit == digit)
				{
					int64_t acc = rcount * f10;
					if (acc >= MODLIMIT)
					{
						acc %= MODLIMIT;
						modwrap_count += 1;
						modwrap_count_s += 1;
					}
					vacc[j] += acc;
					break;
				}
			}
		}
		f10 *= 10;
	}
}

void adjust_groups(
	int decreased_pos,
	std::vector<DigitGroup>& groups,
	int& swapcount)
{
	int _swapcount = 0;
	int remaining = groups[decreased_pos].remaining;
	for (int i = decreased_pos; i < groups.size()-1; i++)
	{
		if (groups[i + 1].digit != 0)
		{
			if (groups[i+1].remaining > remaining)
			{
				std::swap(groups[i], groups[i + 1]);
				_swapcount += 1;
				dbg_swap_count += 1;
			}
		}
	}
	swapcount = _swapcount;
}

void restore_groups(
	int decreased_pos,
	std::vector<DigitGroup>& groups,
	int swapcount)
{
	for (int i = decreased_pos + swapcount -1; i >= decreased_pos; i--)
	{
		std::swap(groups[i], groups[i + 1]);
		dbg_unswap_count += 1;
	}
}

int64_t eval_groups(
	int pos,
	int64_t digitfactor,
	int nzcount,
	std::vector<DigitGroup>& groups,
	int8_t groupcount,
	std::vector<int64_t>& arr_distr,
	std::vector<int8_t>& vseq,
	std::vector<int64_t>& vacc,
	int64_t& sum,
	int8_t lastgroupix
	)
{
	static std::unordered_map<int64_t, std::pair<int64_t,int64_t>> mem_lowgroups;
	int64_t groups_code = 0;

	if (pos == 0)
	{
		int  offset = 0;
		if (groups[groupcount - 1].digit == 0)
		{
			offset = 1;
		}
		std::sort(groups.begin(), groups.end() - offset);
		groups_code = encode_groupscounts(pos, groups, groupcount);
	}


	if (pos == 0)
	{
		auto mem_it = mem_lowgroups.find(groups_code);
		if (mem_it != mem_lowgroups.end())
		{
			dbg_code_low_used += 1;
			int64_t val = mem_it->second.first;
			sum = mem_it->second.second;
			return val;
		}
	}

	int64_t count = 0;
	if (pos < MODSIGLIMIT)
	{
		assert(nzcount > 0);

		int swapcount = 0;
		if (pos > 0)
		{
			adjust_groups(lastgroupix, groups, swapcount);
			groups_code = encode_groupscounts(pos, groups, groupcount);

			int  offset = 0;
			if (groups[groupcount - 1].digit == 0)
			{
				offset = 1;
			}
			// std::sort(groups.begin(), groups.end() - offset);
			groups_code = encode_groupscounts(pos, groups, groupcount);

			auto mem_it = mem_lowgroups.find(groups_code);
			if (mem_it != mem_lowgroups.end())
			{
				dbg_code_low_used += 1;
				int64_t val = mem_it->second.first;
				sum = mem_it->second.second;
				if (swapcount > 0)
					restore_groups(lastgroupix, groups, swapcount);
				return val;
			}
		}


		for (int8_t i = 0; i < groupcount; i++)
		{
			if (groups[i].remaining > 0)
			{
				int8_t digit = groups[i].digit;
				groups[i].remaining -= 1;
				vseq[pos] = digit;
				int new_nzcount = digit == 0 ? nzcount : nzcount - 1;
				int64_t val = 1;
				if (new_nzcount != 0)
				{
					val = eval_groups(pos + 1, digitfactor * 10, new_nzcount, groups, groupcount, arr_distr, vseq, vacc, sum, i);
				}
				else
				{
					if (digit != 0)
					{
						set_acc(pos + 1, groups, groupcount, vseq, vacc, 1);
					}
				}
				if (digit != 0)
					arr_distr[i * MODSIGLIMIT + pos] += val;

				count += val;
				sum += val * digit * digitfactor;
				if (sum >= MODLIMIT)
				{
					sum %= MODLIMIT;
					modwrap_count += 1;
					modwrap_count_z += 1;
				}
				groups[i].remaining += 1;
			}
		}
		mem_lowgroups[groups_code] = std::pair<int64_t, int64_t>(count, sum);
		if (swapcount > 0)
			restore_groups(lastgroupix, groups, swapcount);
	}
	else
	{
		// filled first MODSIGLIMIT digits, so we can calculate the number of combinations for this distribution of digits.

		int64_t rcount = 1;
		if (nzcount != 0)
			rcount = eval_groups_highdigits(pos, nzcount, groups, groupcount);

		set_acc(pos, groups, groupcount, vseq, vacc, rcount);

		//int64_t f10 = 1;
		//for (int i=0 ; i < MODSIGLIMIT ; i++)
		//{
		//	int8_t digit = vseq[i];
		//	if (digit != 0)
		//	{
		//		for (int j = 0; j < groupcount; j++)
		//		{
		//			if (vseq[j] == digit)
		//			{
		//				int64_t acc = rcount * f10;
		//				if (acc >= MODLIMIT)
		//				{
		//					acc %= MODLIMIT;
		//					modwrap_count += 1;
		//					modwrap_count_s += 1;
		//					break;
		//				}
		//				vacc[j] += acc;
		//			}
		//		}
		//	}
		//	f10 *= 10;
		//}
		dbg_sequence_count += 1;
		return rcount;
	}
	
	mem_lowgroups[groups_code] = std::pair<int64_t, int64_t>(count, sum);
	dbg_code_low_stored += 1;
	return count;
}



int64_t sum_groups(std::vector<DigitGroup>& groups)
{
	int nzcount = 0;
	int64_t count = 0;
	int64_t sum = 0;
	std::vector<DigitGroup> tmpGroups = groups;
	for (DigitGroup g:tmpGroups)
	{
		assert(g.digit != 0);
		nzcount += g.count;
	}
	int group_count = (int)tmpGroups.size();
	if (nzcount < DIGITCOUNT)
	{
		//tmpGroups.push_back(DigitGroup(0, std::min(MODSIGLIMIT - 1, DIGITCOUNT - nzcount)));
		tmpGroups.push_back(DigitGroup(0, DIGITCOUNT - nzcount));
	}
	
	dbg_sum_groups_count += 1;
	const int MODPRINT = 10000;
	if (dbg_sum_groups_count % MODPRINT == 0)
	{
		std::cout << "dbg_sum_groups_count: " << dbg_sum_groups_count << std::endl;
		if (dbg_sum_groups_count == MODPRINT * 100)
			exit(0);
	}

	std::vector<int64_t> arr_distr(10 * MODSIGLIMIT, 0);
	int8_t grouplen = (int8_t)tmpGroups.size();

	std::vector<int8_t> vseq(DIGITCOUNT, 0);
	std::vector<int64_t> vacc(grouplen,0);
	count = eval_groups(0, 1, nzcount, tmpGroups, grouplen, arr_distr, vseq, vacc, sum, -1);

	//std::vector<DigitGroup> groups;
	return 0;

}

class ModuloNumbers
{
public:
	ModuloNumbers()
	{
		int64_t factor = 1;
		mod_numbers = std::vector<int64_t>(DIGITCOUNT, 0);

		for (int i = 0; i < DIGITCOUNT; i++)
		{
			mod_numbers[i] = factor;
			factor *= 10;
			if (factor >= MODLIMIT)
				factor %= MODLIMIT;
		}
	}

	int64_t getDecFactor(int i)
	{
		assert(i < DIGITCOUNT);
		return mod_numbers[i];
	}

	int64_t getArrayModNumber(int len)
	{
		int64_t buf_sum = 0;
		//int high_index = len - 1;
		for (int i = 0; i <  MODSIGLIMIT; i++)
		{
			buf_sum += getDecFactor(i) * digit_grouped_arr[i];
			if (buf_sum >= MODLIMIT)
				buf_sum %= MODLIMIT;
		}
		return buf_sum;
	}

	std::vector<int64_t> mod_numbers;
};

ModuloNumbers ms;


void printDigitArr(int  len)
{
	for (int i = 0; i < len; i++)
		std::cout << (int)(digit_grouped_arr[i]);
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

int64_t sum_digitcombinations(std::vector<int8_t>& digit_arr, int startix, int len)
{
	int64_t sum = 0;
	std::vector<DigitGroup> groups;
	int8_t first_digit = digit_arr[startix];

	assert(first_digit != 0);

	int ix = 1;
	while (ix < len && digit_arr[ix + startix] == first_digit)
		ix++;
	groups.push_back(DigitGroup(first_digit, ix));
	// we have at least two different digits, so we need to calculate the combinations.
	while (ix < len)
	{
		int ix_marker = ix;
		int8_t current_digit = digit_arr[ix + startix];
		while (ix < len && digit_arr[ix + startix] == current_digit)
			ix++;
		int current_count = ix - ix_marker;
		groups.push_back(DigitGroup(current_digit, current_count));
	}
	sum = sum_groups(groups);
	return sum;
}

int64_t count_digitcombinations(std::vector<int8_t>& digit_arr, int startix, int len)
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

	std::vector<DigitGroup> groups;

	int8_t first_digit = digit_arr[startix];

	assert(first_digit != 0);

	int ix = 1;
	while (ix < len && digit_arr[ix+startix] == first_digit)
		ix++;
	groups.push_back(DigitGroup(first_digit, ix));
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
			int8_t current_digit = digit_arr[ix + startix];
			while (ix < len && digit_arr[ix + startix] == current_digit)
				ix++;
			groups.push_back(DigitGroup(current_digit, ix - ix_marker));
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

int64_t sum = 0;
int64_t countVariations(std::vector<int8_t>& digit_arr, int digitLimit, int pos, int sq_sum)
{
	assert(pos < DIGITCOUNT);
	
	int64_t count = 0;
	for (int i = digitLimit; i <= 9; i++)
	{
		digit_arr[pos] = i;
		assignCounter += 1;

		int sq_new_sum = sq_sum + i * i;

		if (is_square(sq_new_sum))
		{
			square_count += 1;
			count += sum_digitcombinations(digit_arr,0, pos+1);
			if (count >= MODLIMIT)
			{
				count %= MODLIMIT;
				modwrap_count += 1;
				modwrap_count_s += 1;
			}

			sum += ms.getArrayModNumber(pos + 1);
			if (sum >= MODLIMIT)
			{
				sum %= MODLIMIT;
				modwrap_count += 1;
				modwrap_count_z += 1;
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
			count += countVariations(digit_arr, i, pos+1, sq_new_sum);
			if (count >= MODLIMIT)
			{
				count %= MODLIMIT;
				modwrap_count += 1;
				modwrap_count_s += 1;
			}
		}
	}
	if (count >= MODLIMIT)
	{
		count %= MODLIMIT;
		modwrap_count += 1;
		modwrap_count_s += 1;
	}
	return sum;
}


void test_setarr(int8_t arr[], int len)
{
	for (int i = 0; i < len; i++)
		digit_grouped_arr[i] = arr[i];
}

void test()
{

	std::cout << "MODSIGLIMIT: " << MODSIGLIMIT << std::endl;

	int64_t NineNines = 1 * (2 * 2 * 2 * 2 * 2 * 2 * 2 * 2);
	std::cout << "NineNines" << NineNines << std::endl;

	//std::cout << zdistr(2) << std::endl, std::cout << zdistr(3) << std::endl;

	//int8_t test_arr[]{ 1,2 };
	//test_setarr(test_arr, 2);
	//int64_t test_comb = digitcombinations(0,2);
	//std::cout << "test_comb: " << test_comb << std::endl;
	//ModNumbers* mn = new ModNumbers();
	ModNumbers mn;

	std::cout << "MOGSIGLIMIT:" << MODSIGLIMIT << std::endl;
	std::cout << zdistr(2) << std::endl, std::cout << zdistr(3) << std::endl;

	int8_t test_arr[]{ 1,2 };
	test_setarr(test_arr, 2);
	int64_t test_comb = sum_digitcombinations(digit_grouped_arr, 0, 2);
	std::cout << "test_comb: " << test_comb << std::endl;

	std::vector<int8_t> test_vec(20,0);
	test_vec[0] = 1;
	test_vec[1] = 2;
	test_vec[2] = 7;
	int64_t mod_num = mn.arrToNum(test_vec);
	std::cout << "mod_num: " << mod_num << std::endl;

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
	//test();

	int64_t count = (countVariations(digit_grouped_arr, 1, 0, 0)) % MODLIMIT;	// start with 1, treat leading 0 separately for every found solution in digitcombinations function.
	std::cout << "variations(0," << DIGITCOUNT << ") = " << count << std::endl;
	std::cout << "squares found:" << square_count << std::endl;

	std::cout << "modwrap_count: " << modwrap_count << std::endl;
	std::cout << "modwrap_count_z: " << modwrap_count_z << std::endl;
	std::cout << "modwrap_count_s: " << modwrap_count_s << std::endl;
	std::cout << "modwrap_count_dc: " << modwrap_count_dc << std::endl;
	std::cout << "modwrap_count_dc2: " << modwrap_count_dc2 << std::endl;
	std::cout << "sum: " << sum << std::endl;
	std::cout << "count: " << count << std::endl;
	std::cout << "dbg_sequence_count: " << dbg_sequence_count << std::endl;
	std::cout << "dbg_sum_groups_count: " << dbg_sum_groups_count << std::endl;
	std::cout << "dbg_code_high_used: " << dbg_code_high_used << std::endl;
	std::cout << "dbg_code_high_stored: " << dbg_code_high_stored << std::endl;
	std::cout << "dbg_code_low_used: " << dbg_code_low_used << std::endl;
	std::cout << "dbg_code_low_stored: " << dbg_code_low_stored << std::endl;
	std::cout << "dbg_swap_count: " << dbg_swap_count << std::endl;
	std::cout << "dbg_unswap_count: " << dbg_unswap_count << std::endl;

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
