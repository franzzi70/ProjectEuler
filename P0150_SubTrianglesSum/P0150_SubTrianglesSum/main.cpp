
#include <iostream>
#include <chrono>

const int TRIANGLE_FIELD_SIDELEN = 1000;
const int TRIANGLE_FIELD_SIZE = TRIANGLE_FIELD_SIDELEN * (TRIANGLE_FIELD_SIDELEN + 1) / 2;

int triangleField[TRIANGLE_FIELD_SIZE];
int64_t triangleMem[TRIANGLE_FIELD_SIZE];
int triangleRowsStarts[TRIANGLE_FIELD_SIDELEN];


void init()
{
	const int twoPow20 = 1 << 20;
	const int twoPow19 = 1 << 19;

	int64_t t = 0;
	for (int k = 1; k <= TRIANGLE_FIELD_SIZE; k++)
	{
		t = (615949 * t + 797807) % twoPow20;
		triangleField[k - 1] = t - twoPow19;
		triangleMem[k - 1] = triangleField[k - 1];
	}

	int prevRowStart = 0;
	int newRowStart = 0;
	for (int i = 0; i < TRIANGLE_FIELD_SIDELEN; i++)
	{
		newRowStart = prevRowStart + i;
		triangleRowsStarts[i] = newRowStart;
		prevRowStart = newRowStart;
	}
}

int64_t solve()
{
	init();

	int64_t min_sum = 1'000'000;

	for (int sidelen = 1; sidelen < TRIANGLE_FIELD_SIDELEN; sidelen++)
	{
		// std::cout << "side len:" << sidelen << std::endl;
		for (int i = 0; i <= TRIANGLE_FIELD_SIDELEN - sidelen; i++)
		{
			for (int j = 0; j <= i; j++)
			{
				int64_t prevSum = triangleMem[triangleRowsStarts[i] + j];
				int64_t triangleSum = prevSum;

				if (sidelen > 1)
				{
					int rowStartIx = triangleRowsStarts[i+sidelen-1] + j;
					for (int k = 0; k < sidelen; k++)
					{
						triangleSum += triangleField[rowStartIx + k];
					}
					if (triangleSum < min_sum)
					{
						min_sum = triangleSum;
					}
					triangleMem[triangleRowsStarts[i] + j] = triangleSum;
				}
			}

		}
	}
	// -271248680
	return min_sum;
	;
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
