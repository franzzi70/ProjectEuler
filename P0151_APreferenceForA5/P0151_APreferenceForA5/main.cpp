
#include <iostream>
#include <chrono>
#include <vector>
#include <format>

//#define VERBOSE
//#undef VERBOSE

const int FIELDSIZE = 0x2000;
std::vector<int64_t> arr(16 * FIELDSIZE, 0);

int reduce(int sheet)
{
	switch (sheet)
	{
	case 1:
		return 0;
	case 0x10:
		return 1;
	case 0x100:
		return 0x11;
	case 0x1000:
		return 0x111;
	default:
		return -1;
	}
}

int test_walk(int config)
{
	if ((config & 0xF000) != 0)
		return config - 0x1000 + reduce(0x1000);
	if ((config & 0xF00) != 0)
		return config - 0x100 + reduce(0x100);
	if ((config & 0xF0) != 0)
		return config - 0x10 + reduce(0x10);
	if ((config & 0x0F) != 0)
		return config - 1 + reduce(0x1);
	return 0;
}

void test()
{
	int i = 2;
	int content = 0x1111;
	do
	{
#ifdef VERBOSE
		std::string str = std::format("0x{:04X}", content);
		std::cout << i << " " << str << std::endl;
#endif
		i += 1;
	} while (
		(content = test_walk(content)) != 0
		);
}

void generate(int batchNo, int content, int64_t mult = 1)
{
	// print parameters
#ifdef VERBOSE
	std::string str = std::format("0x{:04X}", content);
	std::cout << batchNo << " " << str << " " << mult << std::endl;
#endif

	int resultStart = (batchNo)*FIELDSIZE;
	int rest = content;
	const int posMask[4] = { 1, 0x10, 0x100, 0x1000 };

	for (int pos = 0; pos < 4; pos++)
	{
		int digit = rest & 0xF;
		rest >>= 4;
		if (digit != 0)
		{
			int sheet = posMask[pos];
			int newContent = content - sheet + reduce(sheet);
			int64_t newMult = mult * digit;
			arr[resultStart + newContent] += newMult;

			if (newContent > 1)
			{
				generate(batchNo + 1, newContent, newMult);
			}
			//else
			//{
			//	std::cout << "batch: " << batchNo << std::endl;
			//}
		}
	}

}

bool is_one_sheet(int content)
{
	if (
		content == 0x1000
		||
		content == 0x100
		||
		content == 0x10
		||
		content == 0x1
		)
		return true;
	return false;
}

double analyze()
{
	std::cout << "analyze" << std::endl;
	int64_t overall_count = 0;
	int64_t one_sheet_count = 0;
	double expectation_one = 0;

	for (int batch = 2; batch <= 15; batch++)
	{
		int64_t batch_overall_count = 0;
		int64_t batch_one_sheet_count = 0;

		int fieldStartIx = (batch - 1) * FIELDSIZE;
		for (int i = 0; i < FIELDSIZE; i++)
		{
			int64_t mult = arr[fieldStartIx + i];
			if (mult != 0)
			{
				overall_count += mult;
				batch_overall_count += mult;

				std::string str = std::format("0x{:04X}", i);
				std::cout << batch << " " << str << " " << mult << std::endl;
				if (is_one_sheet(i))
				{
					one_sheet_count += mult;
					batch_one_sheet_count += mult;
					std::cout << "************" << std::endl;
				}
			}
		}

		expectation_one += one_sheet_count / (double)batch_overall_count;
	}

	return expectation_one;
}

int64_t solve()
{
	test();
	arr[FIELDSIZE + 0x1111] = 1;
	generate(2, 0x1111);
	double rate = analyze();
	std::string rate_string = std::format("{:.6f}", rate);
	std::cout << "rate:" << rate_string << std::endl;
	return 0;
}


int main()
{
	auto t1 = std::chrono::high_resolution_clock::now();
	int64_t solution = solve();
	auto t2 = std::chrono::high_resolution_clock::now();
	auto microSec = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
	std::cout << "solution: " << solution << std::endl << "duration: " << microSec << " micro seconds (" << ms << " ms)" << std::endl;
}
