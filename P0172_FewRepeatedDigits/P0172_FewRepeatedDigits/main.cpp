
/*
The program should calculate how many 18 digtit numbers withoutt leading zeroes exist, which do not have any digit occuring more than  3 times.
*/


#include <iostream>
#include <chrono>
#include <unordered_map>
#include <algorithm>
#include <cassert>

//const int DIGITCOUNT = 18;
const int DIGITCOUNT = 4;


#define DBG_PRINT

#ifdef DBG_PRINT

int64_t dbg_sequence_count = 0;
int64_t calc_callcount = 0;
int64_t dbg_eval_count = 0;

#endif


class DigitGroup {
public:
	DigitGroup()
	{
		digit = 0;
		count = 0;
	}

	DigitGroup(int8_t digit, int count)
	{
		this->digit = digit;
		this->count = count;
	}

	DigitGroup(const DigitGroup& other)
	{
		this->digit = other.digit;
		this->count = other.count;
	}

	bool operator < (const DigitGroup& other) const
	{
		if (digit == 0)
			return false;
		if (other.digit == 0)
			return true;
		return count > other.count;
	}

	int8_t digit;
	int count;
};

typedef std::vector<DigitGroup> groups_t;


#ifdef DBG_PRINT
void dbg_printGroups(groups_t& groups, int groupcount)
{
	// cmopact one line format for group printing:
	for (int i = 0; i < groupcount; i++)
	{
		std::cout << (int)groups[i].digit << ":" << groups[i].count << " ";
	}
}
#endif


int64_t fac(int n)
{
	static int64_t fac_buf[10] = { 1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880 };
	if (n < 10)
	{
		return fac_buf[n];
	}
	int64_t res = 1;
	for (int i = 2; i <= n; i++)
	{
		res *= i;
	}
	return res;
}

int64_t comb(int n, int k)
{
	int64_t result = 1;
	if (k == 0 || k == n)
	{
		return 1;
	}
	int64_t numerator = 1;
	int64_t denominator = 1;
	for (int i = 1; i <= k; i++)
	{
		numerator *= n - (i - 1);
		denominator *= i;
		result = numerator / denominator;
	}
	return result;	
}

int64_t mcomb(groups_t& groups, int groupsize)
{
	int n = 0;
	int64_t result = 1;

	for (int i = 0; i < groupsize; i++)
	{
		if (groups[i].count != 0 && groups[i].digit != 0)
		{
			int k = groups[i].count;
			n += k;
			result *= comb(n, k);
		}
	}
	return result;
}

int64_t mcombz(groups_t& groups, int groupsize, int zcount)
{
	int n = 0;
	int64_t result = 1;

	for (int i = 0; i < groupsize; i++)
	{
		if (groups[i].count != 0 && groups[i].digit != 0)
		{
			int k = groups[i].count;
			n += k;
			result *= comb(n, k);
		}
	}
	n += zcount;
	result *= comb(n, zcount);

	return result;
}

int64_t pos_factor_comb(groups_t& groups, int groupcount)
{
	int64_t prod = 1;

	groups_t tmpGroups = std::vector(groups.begin(), groups.begin()+groupcount);
	std::sort(tmpGroups.begin(), tmpGroups.end());	// sort not needed because groups have already montone falling remaining counts.
	int digits_left = 9;
	int last_count = 0;
	int membercount = 0;

	for (int i = 0; i < groupcount; i++)
	{
		if (tmpGroups[i].digit == 0)
		{
			continue;
		}
		int digit_count = tmpGroups[i].count;
		if (i > 0)
		{
			if (digit_count != last_count)
			{
				prod *= comb(digits_left, membercount);
				digits_left -= membercount;
				membercount = 0;
			}
		}
		membercount += 1;
		last_count = digit_count;
	}
	if (membercount > 0)
	{
		prod *= comb(digits_left, membercount);
	}
	return prod;
}

int64_t calcGroups_rec(
	groups_t& groups,
	int groupindex,
	int nzcount,
	int high_limit,
	int64_t& overall_sum
	)
{
	const int64_t dbg_2pow63m1 = ((uint64_t)1 << 63) - 1;

#ifdef DBG_PRINT
	calc_callcount += 1;
	if (calc_callcount % 10000 == 0)
	{
		std::cout << "calc call count: " << calc_callcount << std::endl;
	}
#endif

	int64_t rec_count = 0;

	if (groupindex < 9)
	{
		int digit_count = groups[groupindex].count;

		if (groupindex != 0)
		{
			if (DIGITCOUNT - nzcount <= 3)
			{

				int64_t count = 1;

				// now position existing zeroes at allowed posisions:
				int zcount = DIGITCOUNT - nzcount;
				if (zcount > 0)
				{
					int64_t val1 = mcombz(groups, groupindex, zcount);
					int64_t val2 = mcombz(groups, groupindex, zcount - 1);
					count = val1 - val2;
				}
				else
				{
					count = mcomb(groups, groupindex);
				}

				// calculate permutations of actual digits for non-zeroes:
				int64_t f = pos_factor_comb(groups, groupindex);

				// now calculate the overall count of permutations for this group configuration:
				int64_t count_permutations = count * f;
#ifdef DBG_PRINT
				dbg_eval_count += 1;
				// print group using db_printGroups and count for debugging:
				dbg_printGroups(groups, groupindex);
				std::cout << " count: " << count << " f: " << f << " count_permutations: " << count_permutations << std::endl;

#endif
				overall_sum += count_permutations;


			}
		}

		for (int i = 1; i <= high_limit; i++)
		{
			int par_nzcount = i + nzcount;
			if (par_nzcount > DIGITCOUNT)
			{
				break;
			}
			int par_high_limit = i;
			groups[groupindex].count += 1;
			assert(groups[groupindex].count <= 3);
			rec_count += calcGroups_rec(groups, groupindex + 1, par_nzcount, par_high_limit, overall_sum);
		}
		groups[groupindex].count = digit_count;
		assert(digit_count == 0);
	}
	return rec_count;
}


int64_t calcGroups()
{
	groups_t groups;
	int64_t acc_count = 0;

	for (int i = 1; i <= 10; i++)
	{
		groups.push_back(DigitGroup(i % 10, 0));
	}
	int64_t count = calcGroups_rec(groups, 0, 0, 3, acc_count);
	return acc_count;
}


int64_t solve()
{
	int64_t count = calcGroups();
#ifdef DBG_PRINT
	std::cout << log(count) / log(10) << " digits in solution" << std::endl;
	std::cout << "dbg_eval_count: " << dbg_eval_count << std::endl;
#endif
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
