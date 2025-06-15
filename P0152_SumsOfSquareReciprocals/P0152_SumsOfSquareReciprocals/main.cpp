
#include <iostream>
#include <chrono>
#include <vector>
#include <numeric>

//const int32_t MAX_NUM = 45;
const int32_t MAX_NUM = 80;

int64_t g_call_count = 0;

class ratio_of_half_test
{
public:
    ratio_of_half_test() {
        init();
        };
    static uint32_t numbers[MAX_NUM + 1];
	static double tail_sums[MAX_NUM + 1];
    static double arr_sqr_inv[MAX_NUM + 1];
    bool find_sequences(uint32_t current_ix = 2, uint32_t sequence_len = 0, double current_sum = 0.0)
    {
		g_call_count += 1;
		if (g_call_count % 100'000'000 == 0)
		{
			//std::cout << "g_call_count: " << g_call_count << " seqence_len: " << sequence_len << std::endl;

			//numbers[sequence_len] = ix_start;
			//sequence_len += 1;
			double p_sum = 0;
			double p_ck_curr_sum = 0;
			std::cout << "call: ";
			for (uint32_t i = 0; i < sequence_len; i++)
			{
				int32_t p_n = numbers[i];
				std::cout << p_n << " ";
				p_sum += 1.0 / (p_n * p_n);
				if (i < sequence_len - 1)
					p_ck_curr_sum += 1.0 / (p_n * p_n);
			}
			std::cout << " sum: " << p_sum;
			std::cout << " check current sum: " << p_ck_curr_sum;
			std::cout << current_ix << std::endl;
		}

		bool found = false;
		double inv_sqr = 1.0 / (current_ix * current_ix);
		uint32_t ix_start = current_ix;
		while (ix_start <= MAX_NUM && current_sum + inv_sqr > 0.5)
		{
			ix_start++;
			inv_sqr = 1.0 / (ix_start * ix_start);
		}
		if (ix_start > MAX_NUM)
            return false;

		double ck_diff = current_sum + inv_sqr - 0.5;
		if (ck_diff < 0.0)
			ck_diff = -ck_diff;
        if (ck_diff < 1E-20)
        {
			numbers[sequence_len] = ix_start;
			sequence_len += 1;
			double p_sum = 0;
			double p_ck_curr_sum = 0;
			std::cout << "Found sequence: ";
			for (uint32_t i = 0; i < sequence_len; i++)
			{
				int32_t p_n = numbers[i];
				std::cout << p_n << " ";
				p_sum += 1.0 / (p_n * p_n);
				if (i < sequence_len-1)
					p_ck_curr_sum += 1.0 / (p_n * p_n);
			}
			std::cout << " sum: " << p_sum;
			std::cout << " check current sum: " << p_ck_curr_sum;
			std::cout << current_ix << std::endl;
        }

        for (uint32_t i = ix_start; i < MAX_NUM; i++)
        {
			double tail_sum = tail_sums[i];
			if (current_sum + tail_sum < 0.5)
				break;
			numbers[sequence_len] = i;
			if (find_sequences(i + 1, sequence_len + 1, current_sum + arr_sqr_inv[i]))
				found = true;
		}
		return found;
	}

	void test()
	{
		for (int32_t i = MAX_NUM; i > 1; i--)
		{
			double inv_sqr = 1.0 / (i * i);
        }
    }

private:
	void init()
	{
		double sum = 0.0;
        for (int32_t i = MAX_NUM; i > 1; i--)
		{
			double sqr_inv = 1.0 / (i * i);
			sum += sqr_inv;
			arr_sqr_inv[i] = sqr_inv;
			tail_sums[i] = sum;
		}
	}
};
uint32_t ratio_of_half_test::numbers[MAX_NUM + 1];
double ratio_of_half_test::tail_sums[MAX_NUM + 1];
double ratio_of_half_test::arr_sqr_inv[MAX_NUM + 1];


void explore()
{
 //   double sum = 0;
 //   for (int i=80;i>1;i--)
 //   {
	//	sum += 1.0 / (i * i);
	//	std::cout << "i: " << i << " sum: " << sum << std::endl;
 //   }
	//double diff05 = sum - 0.5;

 //   double sum2 = 0;
 //   for (int i = 80; i > 1; i--)
 //   {
	//	double inv_sqr = 1.0 / (i * i);
 //       if (sum2 + inv_sqr > diff05)
 //           break;
 //       sum2 += 1.0 / (i * i);
 //       std::cout << "i: " << i << " sum2: " << sum2 << std::endl;
 //   }

	ratio_of_half_test r;
	r.find_sequences();
}

int64_t solve()
{
    explore();
    return 0;
}


int main()
{
    auto t1 = std::chrono::high_resolution_clock::now();
    volatile int64_t solution = solve();
    auto t2 = std::chrono::high_resolution_clock::now();
    auto microSec = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
    std::cout << "solution: " << solution << std::endl << "duration: " << microSec << " micro seconds (" << ms << " ms)" << std::endl;
}
