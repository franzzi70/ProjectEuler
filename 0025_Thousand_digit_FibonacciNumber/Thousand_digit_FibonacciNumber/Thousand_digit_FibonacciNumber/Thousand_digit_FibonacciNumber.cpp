// Thousand_digit_FibonacciNumber.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stack>
#include <vector>
#include <assert.h>
#include <iostream>
#include <math.h>


class MyBigInteger {
public:
	MyBigInteger(__int32 val) {
		m_base = DEFAULT_BASE;
		num.push_back(val);
	}
	MyBigInteger() {
		m_base = DEFAULT_BASE;
		num.push_back(0);
	};
	MyBigInteger(MyBigInteger& initialNumber)
	{
		m_base = DEFAULT_BASE;
		Add(initialNumber);
	}

	MyBigInteger(__int32 val, int base) {
		m_base = base;
		num.push_back(val);
	}

	void DebugPrintNumber()
	{
		std::stack<__int32> tmp;

		for (std::vector<__int32>::iterator it = num.begin(); it != num.end(); it++)
			tmp.push(*it);
		while (!tmp.empty())
		{
			printf("%06d ", tmp.top());
			tmp.pop();
		}
		printf("\n");
	}

	int GetDigitSum()
	{
		int sz = num.size();
		int digitSum = 0;
		for (int i = 0; i < sz; i++)
		{
			__int32 rest = num[i];
			while (rest != 0)
			{
				digitSum += rest % 10;
				rest /= 10;
			}
		}
		return digitSum;
	}

	void Zero()
	{
		for (int i = 0; i < num.size(); i++)
			num[i] = 0;
	}

	int NumberOfDigitsDec()
	{
		int ixHigh = num.size() - 1;
		int itDigitCap = round(log(m_base) / log(10));
		for (int i = ixHigh; i >= 0; i--)
		{
			if (num[i] >= 0)
			{
				return
					(int)ceil(log(num[i]) / log(10)) + i * itDigitCap;
			}
			if (i == 0 && num[i] == 0)
				return 0;
		}

	}

	void Add(MyBigInteger(&other)) {
		//for (std::vector<__int32>::iterator it = num.begin(); it != num.end(); it++)
		int sz = num.size();
		int szOther = other.num.size();
		__int64 overflow = 0;

		int i = 0;
		for (i = 0; i < szOther; i++)
		{
			if (i >= sz)
				num.push_back(0);

			__int64 numOther = other.num[i];
			__int64 tmp = other.num[i] + num[i] + overflow;

			if (tmp / m_base >= 1)
				overflow = 1;
			else
				overflow = 0;

			num[i] = tmp % m_base;
		}
		if (overflow != 0)
		{
			if (i >= sz)
				num.push_back(0);
			num[i] += overflow;
		}

	}

	void PositiveMult(int factor) {
		__int32 overflow = 0;
		//for (std::vector<__int32>::iterator it = num.begin(); it != num.end(); it++)
		int sz = num.size();
		for (int i = 0; i < sz; i++)
		{
			__int32 prod = num[i] * factor + overflow;
			overflow = prod / m_base;
			num[i] = prod % m_base;
			if (overflow != 0)
			{
				if (i == sz - 1)
				{
					num.push_back(overflow);
					break;
				}
			}
		}
	}
private:
	std::vector<__int32> num;
	int m_base;
	const int DEFAULT_BASE = 1000000;
};

int fib(int i)
{
	//assert(i >= 0);

	if (i == 0 || i == 1)
		return 1;
	else
		return fib(i - 1) + fib(i - 2);
}

void FirstFibWith1000Digits()
{
	MyBigInteger a[3];

	MyBigInteger one(1);

	int index = 2;

	a[0].Add(one);
	a[1].Add(one);
	
	while (true)
	{
		a[index % 3].Zero();
		a[index % 3].Add(a[(index - 1) % 3]);
		a[index % 3].Add(a[(index - 2) % 3]);
		
		a[index % 3].DebugPrintNumber();
		
		int NumberOfDigits = a[index % 3].NumberOfDigitsDec();
		std::cout << "fib_" << index + 1 << ", NumberOfDigits:" << NumberOfDigits << std::endl;

		if (NumberOfDigits >= 1000)
		{
			break;
		}
		//if (index >= 40)
		//	break;

		index += 1;

	}

}

int main()
{
	//std::cout << fib(7) << std::endl;

	FirstFibWith1000Digits();

	//MyBigInteger a(1);
	//MyBigInteger b(1);

	//a.Add(b);
	//a.DebugPrintNumber();
	//a.Add(b);
	//a.DebugPrintNumber();
	//a.Add(b);
	//a.DebugPrintNumber();
	//a.Add(b);
	//a.DebugPrintNumber();
	


    return 0;
}

