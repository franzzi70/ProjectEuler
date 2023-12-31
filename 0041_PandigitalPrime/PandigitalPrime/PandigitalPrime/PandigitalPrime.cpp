// PandigitalPrime.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <algorithm>
#include "primes.h"



__int64 findWithNDigits(primes& p, int digitCount)
{
	int permArr[9] = { 0,1,2,3,4,5,6,7,8 };
	int digitsArr[9] = { 9,8,7,6,5,4,3,2,1 };
	__int64 factors[9] = { 100000000, 10000000, 1000000, 100000, 10000, 1000, 100, 10, 1 };


	bool found = false;

	int generatedNums = 0;
	int offs = 9 - digitCount;

	do
	{
		__int64 testNum = 0;

		if (true)
			//digitsArr[permArr[8]] % 2 != 0)	// test only odd numbers
		{

			for (int i = offs;i < 9;i++)
				testNum += digitsArr[permArr[i]] * factors[i];

			if (p.isPrime(testNum))
				return testNum;
		}
		generatedNums += 1;
		if (generatedNums % 1000000 == 0)
		{
			std::cout << generatedNums << "numbers generated, current number:" << testNum << std::endl;
		}
	} while (std::next_permutation(permArr + offs, permArr + 9));
	
	return 0;
}

__int64 solve()
{
	primes p(100);
	p.addPrimesUpTo(100000);

	__int64 foundNum;

	for (int i = 9;i > 1;i-- )
	{
		foundNum = findWithNDigits(p, i);
		if (foundNum != 0)
			break;
	}
	return foundNum;
}

int main()
{

	std::cout << solve() << std::endl;

	//int myints[] = { 0,1,2,3 };

//	std::sort(myints, myints + 3);

	//std::cout << "The 3! possible permutations with 3 elements:\n";
	//do {
	//	std::cout << myints[0] << ' ' << myints[1] << ' ' << myints[2] << ' ' << myints[3] << '\n';
	//} while (std::next_permutation(myints, myints + 4));

	//std::cout << "After loop: " << myints[0] << ' ' << myints[1] << ' ' << myints[2] << ' ' << myints[3] << '\n';

	return 0;
}

