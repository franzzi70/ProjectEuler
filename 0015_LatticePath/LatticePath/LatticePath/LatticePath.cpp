// LatticePath.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int main()
{

	// the solution is 40! / (20! * 20!)
	// ==> 137846528820
	// habe taschenrechner verwended, weiss nicht wie in c++.
	// folgeder code funktioniert nicht:

/*
__int64 result = 1;
	for (__int64 i = 40; i >= 21; i--)
		result *= i;

	__int64 fac10 = 1;
	for (__int64 i = 1; i <= 20; i++)
		fac10 *= i;

	result /= fac10;
*/

	double prod = 1.0;
	for (int i = 1; i <= 20; i++)
		prod *= ((double)20 + i) / i;

	return 0;
}


