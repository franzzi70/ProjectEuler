
#include <iostream>
#include <chrono>
#include <vector>
#include <format>
#include <map>

//#define VERBOSE
//#undef VERBOSE

typedef std::map<int64_t, int64_t> mapType;

mapType m;

void mapValue(mapType& m, int64_t key, int64_t value)
{
	if (m.find(key) == m.end())
	{
		m[key] = value;
	}
	else
	{
		m[key] += value;
	}
}

int64_t generateMapKey(int batchNo, int content, int oneCount)
{
	return (((int64_t)batchNo) << 36) + (((int64_t)oneCount) << 32) + content;
}

void printMap(std::map<int64_t, int64_t>& m)
{
	for (auto const& [key, val] : m)
	{
		int content = key & 0xFFFFFFFF;
		int batchNo = key >> 36;
		int oneCount = (key >> 32) & 0xF;

		std::cout
			<< std::format("{:2}", batchNo)
			<< " " << std::format("{:1}", oneCount)
			<< " " << std::format("0x{:04X}", content)
			<< " " << std::format("{:10}", val) << std::endl;
	}
}

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

int process_sheet(int content, int sheet)
{
	return content - sheet + reduce(sheet);
}

int test_step(int config)
{
	if ((config & 0xF000) != 0)
		return process_sheet(config, 0x1000);
	if ((config & 0xF00) != 0)
		return process_sheet(config, 0x100);
	if ((config & 0xF0) != 0)
		return process_sheet(config, 0x10);
	if ((config & 0x0F) != 0)
		return process_sheet(config, 0x1);
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
		(content = test_step(content)) != 0
		);
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

std::vector<int64_t> ones_paths(16, 0);
int64_t _rec_terminate_count = 0;
void generate(int batchNo = 2, int content = 0x1111, int64_t mult = 1, int one_count = 0)
{
	mapValue(m, generateMapKey(batchNo, content, one_count), mult);

	// print parameters
#ifdef VERBOSE
	std::string str = std::format("0x{:04X}", content);
	std::cout << batchNo << " " << str << " " << mult << std::endl;
#endif

	//int resultStart = (batchNo)*FIELDSIZE;
	int rest = content;
	const int posMask[4] = { 1, 0x10, 0x100, 0x1000 };
	bool is_one = is_one_sheet(content);
	int new_OneCount = one_count + (is_one ? 1 : 0);

	for (int pos = 0; pos < 4; pos++)
	{
		int digit = rest & 0xF;
		rest >>= 4;
		if (digit != 0)
		{
			int sheet = posMask[pos];
			int newContent = process_sheet(content, sheet);
			int64_t newMult = mult * digit;

			if (newContent > 1)
			{
				generate(batchNo + 1, newContent, newMult, new_OneCount);
			}
			else
			{
				ones_paths[new_OneCount] += mult;
				_rec_terminate_count += 1;
				// std::cout << "batch: " << batchNo << std::endl;
			}
		}
	}
}

double analyze()
{
	double expectation_one = 0;

	int64_t total_path_sum = 0;
	int64_t total_path_count = 0;

	for (int ones_count = 0; ones_count < ones_paths.size(); ones_count++)
	{
		int64_t val = ones_paths[ones_count];
		if (val != 0)
		{
			total_path_count += val;
			total_path_sum += ones_count * val;
		}
	}
	expectation_one = total_path_sum / (double)total_path_count;

	return expectation_one;
}

std::string solve()
{
	test();
	generate();
	printMap(m);
	double rate = analyze();
	std::string rate_string = std::format("{:.6f}", rate);

	std::cout << "rate: " << rate_string << std::endl;
	return rate_string;
}


int main()
{
	auto t1 = std::chrono::high_resolution_clock::now();
	std::string solution = solve();
	auto t2 = std::chrono::high_resolution_clock::now();
	auto microSec = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
	std::cout << "solution: " << solution << std::endl << "duration: " << microSec << " micro seconds (" << ms << " ms)" << std::endl;
}
