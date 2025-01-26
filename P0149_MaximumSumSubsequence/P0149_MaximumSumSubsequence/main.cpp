
#include <iostream>
#include <time.h>

#undef USETESTFIELD

#ifdef USETESTFIELD
const int FIELDSIZE = 4;
#else
const int FIELDSIZE = 2000;
#endif


const int NUMBERCOUNT = FIELDSIZE * FIELDSIZE;
int numbers[NUMBERCOUNT];


void init()
{
#ifdef USETESTFIELD
	int testArrValues[16]{ -2, 5, 3, 2, 9, -6, 5,1,3,2,7,3,-1,8,-4,8 };
	for (int i = 0; i < 16; ++i)
		numbers[i] = testArrValues[i];
#else	
for (int k = 1; k <= NUMBERCOUNT; k++)
{
	if (k <= 55)
	{
		int64_t n = (100003 - 200003 * k + (int64_t)(300007) * k * k * k) % 1'000'000 - 500'000;
		if (n < -1'000'000)
		{
			std::cout << "error" << std::endl;
		}
		numbers[k - 1] = (int)n;
	}
	else
	{
		int r = (numbers[k - 25] + numbers[k - 56] + 1'000'000);
		if (r < 0)
		{
			std::cout << "error" << std::endl;
		}
		int n = r % 1'000'000 - 500'000;
		if (n < -1'000'000)
		{
			std::cout << "error" << std::endl;
		}
		numbers[k - 1] = n;
	}
}
#endif
}

enum direction
{
	horiz,
	vert,
	diag1,
	diag2
};
direction directions[] = { horiz, vert, diag1, diag2 };

class sequence {
public:
	direction dir;
	int sequence_num;
	int start_index;

	static int last_sequence_num(direction dir)
	{
		switch (dir)
		{
		case horiz:
			return FIELDSIZE - 1;
		case vert:
			return FIELDSIZE - 1;
		case diag1:
			return FIELDSIZE * 2 - 2;
		case diag2:
			return FIELDSIZE * 2 - 2;
		}
	}
};

int get_start_index(int row_num, direction dir)
{
	switch (dir)
	{
	case horiz:
		return row_num * FIELDSIZE;
	case vert:
		return row_num;
	case diag1:
		return row_num < FIELDSIZE ? (FIELDSIZE - row_num - 1) * FIELDSIZE : (row_num - FIELDSIZE) +1;
	case diag2:
		return row_num < FIELDSIZE ? row_num : FIELDSIZE * (row_num - FIELDSIZE + 2) - 1;
	}
}

int get_sequence_len(int row_num, direction dir)
{
	switch (dir)
	{
	case horiz:
		return FIELDSIZE;
	case vert:
		return FIELDSIZE;
	case diag1:
		return row_num < FIELDSIZE ? row_num + 1 : FIELDSIZE - (row_num - FIELDSIZE) - 1;
	case diag2:
		return row_num < FIELDSIZE ? row_num + 1 : FIELDSIZE - (row_num - FIELDSIZE) - 1;
	}
}

int get_mext_index(int index, direction dir)
{
	switch (dir)
	{
	case horiz:
		return index + 1;
	case vert:
		return index + FIELDSIZE;
	case diag1:
		return index + FIELDSIZE + 1;
	case diag2:
		return index + FIELDSIZE - 1;
	}
}

void test()
{
	int ix1 = get_start_index(1999, direction::diag2);
	int ix2 = get_start_index(2000, direction::diag2);
}

int64_t solve()
{
	init();
	//test();
	//return 0;
	int64_t max_sum = 0;
	for (auto dir : directions)
	{
		int last_sequence_num = sequence::last_sequence_num(dir);
		for (int i = 0; i <= last_sequence_num; i++)
		{
			int64_t run_sum = 0;
			int seq_ix = 1;

			int start_ix = get_start_index(i, dir);
			int sequence_len = get_sequence_len(i, dir);
			int current_ix = start_ix;

			while (seq_ix <= sequence_len)
			{
				int num = numbers[current_ix];
				if (num > 0)
				{
					run_sum += num;
					if (run_sum > max_sum)
					{
						max_sum = run_sum;
					}
				}
				if (num < 0)
				{
					if ((run_sum + num) > 0)
					{
						run_sum += num;
					}
					else
					{
						run_sum = 0;
					}
				}

				current_ix = get_mext_index(current_ix, dir);
				seq_ix += 1;
			}
		}
	}
	return max_sum;
}

int main()
{
	time_t t1 = clock();
	int64_t solution = solve();
	time_t t2 = clock();
	int ms = (int)(t2 - t1) * 1000 / CLOCKS_PER_SEC;

	std::cout << "solution: " << solution << std::endl << "duration: " << ms << " ms" << std::endl;
}
