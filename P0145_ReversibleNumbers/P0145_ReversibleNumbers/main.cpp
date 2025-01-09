
#include <iostream>
#include <chrono>
#include <string>
#include <thread>
#include <mutex>
#include <vector>

//#define VERBOSE


int reverseNum(int n)
{
	int reversedNum = 0;
	int r = n;
	while (r > 0)
	{
		int digit = r % 10;
		reversedNum = reversedNum * 10 + digit;
		r /= 10;
	}
	return reversedNum;
}


int solve_bf_simple() // check requriement
{
	int reversableCount = 0;

	for (int i = 1; i < 1'000'000'000; i++)
	{
#ifdef VERBOSE
		if (i % 1'000'000 == 0)
			std::cout << i << std::endl;
#endif
		if (i % 10 == 0)
			continue;
		int i_reverse = reverseNum(i);
		int n = i + i_reverse;;
		bool reversible = true;
		while (n > 0)
		{
			if ((n % 10) % 2 == 0)
			{
				reversible = false;
				break;
			}
			n /= 10;
		}
		if (reversible)
		{
			reversableCount += 1;
		}
	}
	return reversableCount;
}


const int MAXNUMBER = 1'000'000'000;
const int THREADCOUNT = 20;
int th_nextNumber = 0;
const int th_numbersPerRun = 10'000;
int th_reversableCount = 0;
std::mutex th_mutex;

void threadProc()
{
	while (true)
	{
		int reversableCount = 0;
		th_mutex.lock();
		int start = th_nextNumber;
		th_nextNumber += th_numbersPerRun;
		th_mutex.unlock();
		if (start >= MAXNUMBER)
			return;


		int limit = start + th_numbersPerRun;
		if (limit > MAXNUMBER)
			limit = MAXNUMBER;

		for (int i = start; i < limit; i++)
		{
#ifdef VERBOSE
			if (i % 1'000'000 == 0)
				std::cout << i << std::endl;
#endif
			if (i % 10 == 0)
				continue;
			int i_reverse = reverseNum(i);
			int n = i + i_reverse;;
			bool reversible = true;
			while (n > 0)
			{
				if ((n % 10) % 2 == 0)
				{
					reversible = false;
					break;
				}
				n /= 10;
			}
			if (reversible)
			{
				reversableCount += 1;
				//std::cout << i << std::endl;
			}
		}

		th_mutex.lock();
		th_reversableCount += reversableCount;
		th_mutex.unlock();
	}
}


int solve_bf_multithreaded() // check requriement
{
	std::vector<std::thread> threads;
	for (int i = 0; i < THREADCOUNT; i++)
	{
		threads.push_back(std::thread(threadProc));
	}

	for (int i = 0; i < THREADCOUNT; i++)
	{
		threads[i].join();
	}

	return th_reversableCount;
}

void test()
{
	int val;
	for (int i = 0; i < 10; i++)
	{
		th_nextNumber = 0;
		th_reversableCount = 0;
		val = solve_bf_multithreaded();
	}
}

int64_t solve()
{
	//{ test(); return 0;}
	//return solve_bf_simple();
	return solve_bf_multithreaded();
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
