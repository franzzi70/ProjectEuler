
#include <iostream>
#include <chrono>
#include <fstream>
#include <vector>


const bool VERBOSE1 = false;
const bool VERBOSE2 = false;
const bool VERBOSE3 = true;
const bool NO_FILEWRITE = true;
const int MAX_ROW_NUM = 1'000'000'000;

uint64_t fac(uint64_t n)
{
	uint64_t result = 1;
	for (uint64_t i = 1; i <= n; i++)
	{
		result *= i;
	}
	return result;
}

uint64_t factorCountOfFactorial(uint64_t n, uint32_t f)
{
	int64_t f_pow = f;
	int64_t f_count = 0;
	while (f_pow <= n)
	{
		f_count += n / f_pow;
		f_pow *= f;
	}
	return f_count;
}


uint64_t checkFactorCount(uint64_t n, uint64_t f)
{
	uint64_t count = 0;
	uint64_t r = n;
	while (r % f == 0)
	{
		r /= f;
		count += 1;
	}
	return count;
}


//int64_t oneCount() {
//	if (m_row_num < m_factor) {
//		return 0; // No numbers divisible by 7 in rows smaller than the factor
//	}
//
//	int64_t count = 0;
//
//	// Iterate over all entries in the row
//	for (int k = 0; k <= m_row_num; k++) {
//		int n = m_row_num;
//		int current_k = k;
//		bool divisible = false;
//
//		// Apply Lucas' Theorem: Check base-7 digits of n and k
//		while (n > 0 || current_k > 0) {
//			int n_digit = n % m_factor;
//			int k_digit = current_k % m_factor;
//
//			if (k_digit > n_digit) {
//				divisible = true;
//				break;
//			}
//
//			n /= m_factor;
//			current_k /= m_factor;
//		}
//
//		if (divisible) {
//			count++;
//		}
//	}
//
//	return count;
//} 
class OffsetRowValues
{
public:
	OffsetRowValues(int factor, int max_pyth_rownum) : m_factor(factor)
	{
		m_group_count = log(max_pyth_rownum) / log(factor) + 1;
		m_group_sizes = std::vector<int>(m_group_count);
		m_group_digits = std::vector<int>(m_group_count, 0);
		m_group_counts = std::vector<int>(m_group_count, 0);
		int f = 1;
		for (int i = 0; i < m_group_count; i++)
		{
			m_group_sizes[i] = f;
			f *= factor;
		}
		m_high_index = 0;
		m_row_num = 0;
	}

	OffsetRowValues(int factor, int max_pyth_rownum, int row_num) : m_factor(factor)
	{
		m_row_num = row_num;
		m_group_count = log(max_pyth_rownum) / log(factor) + 1;
		m_group_sizes = std::vector<int>(m_group_count);
		m_group_digits = std::vector<int>(m_group_count, 0);
		m_group_counts = std::vector<int>(m_group_count, 0);
		int f = 1;
		for (int i = 0; i < m_group_count; i++)
		{
			m_group_sizes[i] = f;
			f *= factor;
			m_group_counts[i] = row_num / f;
		}
		int rest = row_num;
		int ix = 0;
		while (rest > 0)
		{
			int digit = rest % factor;
			m_group_digits[ix] = digit;
			m_high_index = ix;
			ix += 1;
			rest /= factor;
		}
	}

	void setRow(int row_num)
	{
		//if (row_num >= 7)
		//	std::cout << "checkpoint" << std::endl;

		m_row_num = row_num;

		int f = 1;
		for (int i = 0; i < m_group_count; i++)
		{
			f *= m_factor;
			m_group_counts[i] = row_num / f;
		}
		int rest = row_num;
		int ix = 0;
		while (rest > 0)
		{
			int digit = rest % m_factor;
			m_group_digits[ix] = digit;
			m_high_index = ix;
			ix += 1;
			rest /= m_factor;
		}
	}

	int64_t oneCount()
	{

		if (m_row_num == 399)
			std::cout << "checkpoint" << std::endl;

		//if (m_row_num < m_factor)
		//	return 0;

		int upper_count = 0;
		int64_t count = 0;
		for (int i = m_high_index; i >= 1; i--)
		{
			int64_t group_count = (upper_count + 1) * m_group_digits[i];
			int group_size = m_group_sizes[i];
			count += group_count * (group_size - 1 - (m_row_num % group_size));
			upper_count = group_count == 0 ?
				upper_count
				:
				group_count;
//			upper_count = group_count;
		}
		return count;
	}


	void inc()
	{
		if (m_row_num == 100'000'000)
			std::cout << "checkpoint\n";
		m_row_num += 1;
		int group_num = 0;
		int val = m_group_digits[group_num];
		m_group_counts[group_num] += 1;
		bool carry = val + 1 >= m_factor;
		m_group_digits[group_num] = carry ? 0 : val + 1;

		while (carry)
		{
			group_num += 1;
			m_high_index = group_num > m_high_index ? group_num : m_high_index;
			m_group_counts[group_num] += 1;
			val = m_group_digits[group_num];
			carry = val + 1 >= m_factor;
			m_group_digits[group_num] = carry ? 0 : val + 1;
		}
	}

	int m_row_num = 0;
	int m_high_index = 0;
	int m_factor;
	int m_group_count;
	std::vector<int> m_group_digits;
	std::vector<int> m_group_counts;
	std::vector<int> m_group_sizes;

};

OffsetRowValues g_v(7, MAX_ROW_NUM);


int nonPerRow(int row_num, int factor)
{
	//for (int i = 0; i < 1'000'000'000; i++)
	//	g_v.inc();

	g_v.setRow(row_num);
	int one_count = g_v.oneCount();
	return row_num + 1 - one_count;

/*
	int n = row_num - 1;
	int row_num_mod7 = row_num % 7;
	int count_7_groups = row_num / 7;


	int row_num_mod49 = row_num % 49;
	int count_49_groups = row_num / 49;
	if (row_num == 49)
		std::cout << "checkpoint\n";
	int count_49 = count_49_groups * (48 - row_num_mod49);
	int count_7_in_49 = 1 + ((48 - row_num_mod49) / 7);
	count_7_groups -= count_7_in_49 * count_49_groups;
	int count_7 = count_7_groups * (6 - row_num_mod7);
	int count = count_49 + count_7;
	return row_num + 1 - count;

*/

//int count = 0;
	//int f = 7 * 7 * 7 * 7 * 7 * 7 * 7 * 7 * 7 * 7 * 7;
	//int blocked_count = 0;
	//while (f > 1)
	//{
	//	int next_f = f / 7;
	//	if (row_num >= f)
	//	{
	//		//if (row_num == 350)
	//		if (row_num == 56)
	//			//if (row_num == 49)
	//			std::cout << "checkpoint\n";
	//		int gc = row_num / f - blocked_count;
	//		int offset = row_num % f;
	//		int c = gc * (f - 1 - offset);
	//		count += c;
	//		blocked_count = blocked_count * factor + gc * (factor - offset / next_f);
	//		// std::cout << "checkpoint\n";
	//		//free_count -= gc * (f - offset / factor);
	//		//next_group_count = 
	//		// sevenGroupCount -= (factor - offset49 / 7) * gc49;
	//	}
	//	f = next_f;
	//}
	//return row_num + 1 - count;
}

int nonPerRow_prot(int row_num, int factor)
{
	//if (row_num < factor * factor)
	//{
	//	int DivCountPerGroup = factor - (row_num % factor + 1);
	//	return row_num + 1 - (DivCountPerGroup * (row_num / factor));
	//}
	//else
	//{
	//	return 2 * (row_num % factor + 1);
	//}
	//if (row_num == 56)
	//	std::cout << "checkpoint\n";

	int count = 0;
	int sevenGroupCount = row_num / factor;
	if (row_num >= factor * factor)
	{
		int f = factor * factor;
		int gc49 = row_num / f;
		int offset49 = row_num % f;
		int c49 = gc49 * (f - 1 - offset49);
		count += c49;
		sevenGroupCount -= (factor - offset49 / 7) * gc49;
	}
	int sevenOffset = row_num % factor;
	int divsible_count = sevenGroupCount * (factor - 1 - sevenOffset);
	count += divsible_count;
	return row_num + 1 - count;
}

int64_t bruteForceTest(int64_t row_count, int factor)
{
	int64_t count = 0;
	int64_t entry_count = 0;
	for (int64_t i = 0; i <= row_count - 1; i++)
	{
		int64_t countForRow = 0;
		for (int64_t k = 0; k <= i; k++)
		{
			entry_count += 1;
			bool noDiv = false;
			if (
				factorCountOfFactorial(i, factor)
				- factorCountOfFactorial(k, factor)
				- factorCountOfFactorial(i - k, factor) <= 0
				)
			{
				countForRow += 1;
				noDiv = true;
				count += 1;
			}
			if (VERBOSE1)
			{
				if
					//(i == 323)
					(i == 100)
				{
					std::cout << "(" << i << ", " << k << ") " << " divisible: " << !noDiv;
					std::cout << "\n";
				}
			}
		}
		int64_t non_cnt_row = nonPerRow(i, factor);
		if (VERBOSE3)
		{
			if (countForRow != non_cnt_row)
			{
				std::cerr << "Mismatch in row " << i << ": countForRow = " << countForRow << ", nonPerRow = " << non_cnt_row << std::endl;
			}
		}
		if (VERBOSE2)
		{
			if
				//(i == 323)
				(i == 50)
				std::cout << "nonCount: " << nonPerRow(i, factor) << "\n";
		}
	}
	std::cout << "entry count: " << entry_count << std::endl;
	return count;
}

int64_t rowsTest(int64_t row_number, int factor)
{
	int64_t count = 0;
	for (int i = 0; i <= row_number; i++)
		count += nonPerRow(i, factor);
	return count;
}


void print_sampleRows(int factor, int start_row_num, int row_count, std::string fileName, bool printRowHeader)
{
	int max_n = start_row_num + row_count - 1;

	if (NO_FILEWRITE)
	if (NO_FILEWRITE)
		return;
	std::string _fileName;
	if (NO_FILEWRITE)
		_fileName = "NIL:";
	else
		_fileName = fileName;

	std::ofstream outFile(_fileName);
	if (printRowHeader)
	{
		outFile << "*" << '\t';
		for (int i = 0; i <= max_n; i++)
		{
			if (i > 0)
				outFile << "\t";
			outFile << i;
		}
		outFile << "\n";
	}
	int i = start_row_num;

	for (int i = start_row_num; i <= max_n; i++)
	{
		outFile << i << "\t";

		for (int j = 0; j <= i; j++)
		{
			int c1 = factorCountOfFactorial(i, factor);
			int c2 = factorCountOfFactorial(j, factor);
			int c3 = factorCountOfFactorial(i - j, factor);
			int is_div =
				c1 - c2 - c3 <= 0
				?
				0 : 1;
			if (j > 0)
				outFile << "\t";
			outFile << is_div;
		}
		outFile << std::endl;

	}
	outFile.close();
}


void print_sampleMap(int max_n, int factor, std::string fileName)
{
	if (NO_FILEWRITE)
		return;

	std::string _fileName;
	if (NO_FILEWRITE)
		_fileName = "NIL:";
	else
		_fileName = fileName;

	std::ofstream outFile(_fileName);

	outFile << "*" << '\t';

	for (int i = 0; i <= max_n; i++)
	{
		if (i > 0)
			outFile << "\t";
		outFile << i;
	}
	outFile << "\n";
	for (int i = 0; i <= max_n; i++)
	{
		outFile << i << '\t';
		for (int j = 0; j <= i; j++)
		{
			if (i == 8 && j == 7)
				std::cout << "checkpoint";

			int c1 = factorCountOfFactorial(i, factor);
			int c2 = factorCountOfFactorial(j, factor);
			int c3 = factorCountOfFactorial(i - j, factor);

			int is_div =
				c1 - c2 - c3 <= 0
				?
				0 : 1;
			if (j > 0)
				outFile << "\t";
			outFile << is_div;
		}
		outFile << std::endl;
	}
	outFile.close();
}


int64_t solve()
{
	//for (int i = 0; i < 1000'000'000; i++)
	//	g_v.inc();
	//return 0;

	//std::cout << factorCountOfFactorial(20, 3) << std::endl;
	//std::cout << checkFactorCount(fac(20), 3) << std::endl;
	//std::cout << bruteForceTest(1000, 7) << std::endl;
	//std::cout << rowsTest(1000, 7) << std::endl;
	//print_sampleMap(200, 7, "output.txt");

	print_sampleRows(7, 398, 10, "output_samplerows.txt", true);

	std::cout << bruteForceTest(1'000, 7) << std::endl;
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
	if (microSec > 300'000'000)
	{
		std::cout << "(" << ((float)microSec) / 60'000'000 << " minutes )" << std::endl;
	}
}
