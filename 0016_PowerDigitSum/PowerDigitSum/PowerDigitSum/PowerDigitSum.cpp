// PowerDigitSum.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include<stack>

typedef unsigned __int16 bin16;


void SumOfDigits()
{

	bin16 a[63];

	for (int i=0; i < 63; i++)
		a[i] = 0;
	a[62] = 256;


	std::stack<int> digitStack;

	while (true)
	{

		// check if done: all values empty
		bool isZero = true;
		for (int i = 0; i < 63; i++)
		{
			if (a[i] != 0)
				isZero = false;
		}
		if (isZero)
		{
			// DONE:
			break;
		}

		int rest = 0;
		for (int i = 62; i >= 0; i--)
		{
			int baseValue = rest * 65536 + a[i];
			rest = baseValue % 10;
			a[i] = baseValue / 10;
		}
		int digit = rest;
		digitStack.push(digit);
	}

	int digitSum = 0;
	while (!digitStack.empty())
	{
		int digit = digitStack.top();
		digitStack.pop();

		digitSum += digit;
		printf("%d", digit);

	}
	printf("\ndigitSum:%d\n", digitSum);

}

int main()
{

	SumOfDigits();


    return 0;
}

