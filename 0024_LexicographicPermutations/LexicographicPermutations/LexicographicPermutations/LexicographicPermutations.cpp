// LexicographicPermutations.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <algorithm>

std::string s("0123456789");

int fac(int i)
{
	if (i == 0)
		return 1;
	else
		return i * fac(i-1);
}

using namespace std;

int main()
{
	for (int i = 0; i < 999999; i++)
	{
		(void) next_permutation(s.begin(), s.end());
	}

	std::cout << s.c_str() << std::endl;
	
    return 0;
}

