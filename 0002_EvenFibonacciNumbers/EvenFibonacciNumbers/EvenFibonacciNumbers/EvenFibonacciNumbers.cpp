// EvenFibonacciNumbers.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <assert.h>

int CallCount;

int fib(int n)
{
	assert(n >= 0);

	CallCount += 1;
	if (n <= 1)
		return n;
	return fib(n - 1) + fib(n - 2);
}

int fib_count(int n)
{
	CallCount = 0;
	printf("fib(%d) = %d, callCount: %d\n", n, fib(n), CallCount);
	return fib(n);
}

int main()
{
	fib_count(3);
	fib_count(5);
	fib_count(10);
	fib_count(20);

	int sum = 0;
	for (int i = 0; ; i++)
	{
		int fibNumber = fib(i);
		if (fibNumber > 4000000)
			break;
		if (fibNumber % 2 == 0)
			sum += fibNumber;
	}

	printf("sum:%d\n", sum);

    return 0;
}

