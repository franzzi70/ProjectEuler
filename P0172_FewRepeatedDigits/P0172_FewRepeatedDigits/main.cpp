
/*
The program should calculate how many 18 digtit numbers withoutt leading zeroes exist, which do not have any digit occuring more than  3 times.
*/


#include <iostream>
#include <chrono>
#include <unordered_map>
#include <algorithm>
#include <cassert>

const int DIGITCOUNT = 18;


#define DBG_PRINT

#ifdef DBG_PRINT

int64_t dbg_code_high_used = 0;
int64_t dbg_code_high_stored = 0;
int64_t dbg_sequence_count = 0;
int64_t calc_callcount = 0;

#endif


class DigitGroup {
public:
	DigitGroup()
	{
		digit = 0;
		count = 0;
		remaining = 0;
		acc = 0;
	}
	DigitGroup(int8_t digit, int count)
	{
		this->digit = digit;
		this->count = count;
		this->remaining = count;
		this->acc = 0;
	}

	DigitGroup(int8_t digit, int count, int remaining)
	{
		this->digit = digit;
		this->count = count;
		this->remaining = remaining;
		this->acc = 0;
	}

	DigitGroup(const DigitGroup& other)
	{
		this->digit = other.digit;
		this->count = other.count;
		this->remaining = other.remaining;
		this->acc = other.acc;
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
	int64_t acc;
};

typedef std::vector<DigitGroup> groups_t;



int64_t encode_groupscounts(int pos, groups_t& groups, int grouplen)
{
	int ix = 0;
	int i = 0;
	int shift = 0;
	static int8_t buf[10];
	int z_remaining = 0;
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
			else
			{
				z_remaining = groups[i].remaining;
			}
		}
		i++;
	}
	std::sort(buf, buf + ix);
	int64_t val = 0;
	for (int i = 0; i < ix; i++)
	{
		val += ((int64_t)buf[i]) << shift;
		shift += 5;
	}
	int pos_shift = 5 * 10;
	val += ((int64_t)z_remaining) << pos_shift;
	pos_shift += 5;
	val += ((int64_t)pos) << pos_shift;
	return val;
}

int64_t eval_groups_highdigits(
	int pos,
	int nzcount,
	groups_t& groups,
	int8_t groupcount
)
{
	assert(nzcount > 0);

	static std::unordered_map<int64_t, int64_t> mem_highgroups;

	int64_t groups_code = encode_groupscounts(pos, groups, groupcount);
	auto mem_it = mem_highgroups.find(groups_code);
	if (mem_it != mem_highgroups.end())
	{
#ifdef DBG_PRINT
		dbg_code_high_used += 1;
#endif
		int64_t val = mem_it->second;
		return val;
	}

	int64_t sum = 0;
	if (pos < DIGITCOUNT - 1)
	{

		for (int8_t i = 0; i < groupcount; i++)
		{
			if (groups[i].remaining > 0)
			{
				int8_t digit = groups[i].digit;
				groups[i].remaining -= 1;
				int new_nzcount = digit == 0 ? nzcount : nzcount - 1;
				int64_t val = 0;	// does not count if leading zeroes only.
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
		mem_highgroups[groups_code] = 1;
#ifdef DBG_PRINT
		dbg_sequence_count += 1;
		dbg_code_high_stored += 1;
#endif
		return 1;
	}

	mem_highgroups[groups_code] = sum;
#ifdef DBG_PRINT
	dbg_code_high_stored += 1;
#endif
	return sum;
}

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
	//if (k > n)
	//{
	//	return 0;
	//}
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

//int64_t pos_factor(int n)
//{
//	static std::vector<int64_t> posfac_buf(10, 0);
//	if (posfac_buf[n] != 0)
//	{
//		return posfac_buf[n];
//	}
//	int f = 10;
//	int64_t prod = 1;
//	for (int i = 0; i < n; i++)
//	{
//		prod *= f;
//		f -= 1;
//	}
//	posfac_buf[n] = prod;
//	return prod;
//}

int64_t pos_factor_comb(groups_t& groups, int groupcount)
{
	int64_t prod = 1;

	groups_t tmpGroups = std::vector(groups.begin(), groups.begin()+groupcount);
	std::sort(tmpGroups.begin(), tmpGroups.end());	// sort not needed because groups have already montone falling remaining counts.
	int digits_left = 9;
	int last_remaining = 0;
	int membercount = 0;

	for (int i = 0; i < groupcount; i++)
	{
		if (tmpGroups[i].digit == 0)
		{
			continue;
		}
		int  remaining = tmpGroups[i].remaining;
		if (i > 0)
		{
			if (remaining != last_remaining)
			{
				prod *= comb(digits_left, membercount);
				digits_left -= membercount;
				membercount = 0;
			}
		}
		membercount += 1;
		last_remaining = remaining;
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

	int64_t count = 0;
	int64_t rec_count = 0;

	if (groupindex < 9)
	{
		int remaing = groups[groupindex].remaining;

		if (groupindex != 0)
		{
			if (DIGITCOUNT - nzcount <= 3)
			{
				DigitGroup group_bak = groups[groupindex];
				groups[groupindex].digit = 0;
				groups[groupindex].count = DIGITCOUNT - nzcount;
				groups[groupindex].remaining = DIGITCOUNT - nzcount;
				groups[groupindex].acc = 0;
				int8_t eval_groupcount = groupindex + 1;
				int eval_pos = 0;
				count = eval_groups_highdigits(eval_pos, nzcount, groups, eval_groupcount);
				int64_t f = pos_factor_comb(groups, eval_groupcount);
				groups[groupindex] = group_bak;
				int64_t count_permutations = count * f;
				overall_sum += count_permutations;
				if (overall_sum > dbg_2pow63m1 * 0.9)
					std::cout << "near overflow." << std::endl;
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
			groups[groupindex].remaining += 1;
			assert(groups[groupindex].remaining <= 3);
			rec_count += calcGroups_rec(groups, groupindex + 1, par_nzcount, par_high_limit, overall_sum);
		}
		groups[groupindex].remaining = remaing;
		assert(remaing == 0);
	}
	return rec_count;
}


int64_t calcGroups()
{
	groups_t groups;
	int64_t acc_count = 0;

	for (int i = 1; i <= 10; i++)
	{
		groups.push_back(DigitGroup(i % 10, 3, 0));
	}
	int64_t count = calcGroups_rec(groups, 0, 0, 3, acc_count);
	return acc_count;
}


int64_t solve()
{
	int64_t count = calcGroups();
#ifdef DBG_PRINT
	std::cout << log(count) / log(10) << " digits in solution" << std::endl;
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
