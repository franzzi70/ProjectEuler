
#include <iostream>
#include <chrono>


int getCount(int x, int y, bool addSubCounts = true)
{
	int X = x;
	int Y = y;
	if (y > x)
	{
		X = y;
		Y = x;
	}

	int count = 0;
	for (int w = 1; w <= X; w++)
	{
		for (int h = 1; h <= Y; h++)
		{
			int subCount = 0;
			if (addSubCounts)
			{
				if (w < X || h < Y)
				{
					subCount = getCount(w, h, false);
				}
			}
			count += (X - w + 1) * (Y - h + 1) + subCount;
		}
	}

	;
	// diagonal rectangles:
	for (int w = 1; w <= 2 * X; w++)
	{
		bool foundForW = false;
		for (int h = 1; h <= w; h++)
		{
			bool foundForH = false;
			// CornerOnGrid
			int w1 = 2 + (w - 1) / 2 + (h - 1) / 2;
			int h1 = (1 + w + h) / 2;
			if (w1 <= X && h1 <= Y)
			{
				int d1 = (X - w1 + 1) * (Y - h1 + 1);
				count += d1 * (w == h ? 1 : 2);
				foundForW = true;
				foundForH = true;
			}
			// CornerOnCenter
			int w2 = 1 + w / 2 + h / 2;
			int h2 = (w + h + 2) / 2;
			if (w2 <= X && h2 <= Y)
			{
				int d2 = (X - w2 + 1) * (Y - h2 + 1);
				count += d2 * (w == h ? 1 : 2);
				foundForW = true;
				foundForH = true;
			}
			if (!foundForH)
				break; // no more rectangles possible with greater height
		}
		if (!foundForW) // no more rectangles possible with greater width
			break;
	}

	return count;
}

__int64 solve()
{

	//// test:
	//int c1 = getCount(3, 2);

	return getCount(47, 43);
}

int main()
{

	auto t1 = std::chrono::high_resolution_clock::now();
	__int64 solution = solve();
	auto t2 = std::chrono::high_resolution_clock::now();
	auto microSec = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
	std::cout << "solution: " << solution << std::endl << "duration: " << microSec << " micro seconds (" << ms << " ms)" << std::endl;
	if (microSec > 300'000'000)
	{
		std::cout << "(" << ((float)microSec) / 60'000'000 << " minutes )" << std::endl;
	}
}
