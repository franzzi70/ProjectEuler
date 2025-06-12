// P0118_PandigitalPrimeSets.cpp : Defines the entry point for the application.
//

#include "main.h"
#include <vector>
#include <algorithm>

using namespace std;

__int64 fac(__int64 num)
{
	_int64 result = 1;
	for (__int64 i = 1; i <= num; i++)
	{
		result *= i;
	}
	return result;
}

__int64 countPrimeSets(std::vector<int>& digits, int position, int prevNumber)
{
	// build prime number starting from position
	// next number found should be greater than prevNumber


	__int64 cnt = 0;
	return cnt;
}

__int64 solve()
{
	std::vector<int> digits(9);
	for (int i=0;i<9; i++)
	{
		digits[i] = i+1;
	}

	__int64 cnt = 0;
	do
	{
		cnt += countPrimeSets(digits, 0, 0);
	} while (std::next_permutation(digits.begin(), digits.end()));

	
	return cnt;
}

int main()
{
	cout << "Hello CMake." << endl;
	cout << fac(5) << endl;
	cout << fac(9) << endl;
	cout << fac(10) << endl;

	cout << solve() << endl;

	return 0;
}
