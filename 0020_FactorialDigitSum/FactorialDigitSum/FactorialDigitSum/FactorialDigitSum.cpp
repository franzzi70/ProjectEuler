// FactorialDigitSum.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <vector>
#include <stack>

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

	void PositiveMult(int factor){
		__int32 overflow = 0;
		//for (std::vector<__int32>::iterator it = num.begin(); it != num.end(); it++)
		int sz = num.size();
		for (int i =0 ; i < sz ; i++)
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

void Solve()
{
	MyBigInteger m(1);

	for (int i = 1; i <= 100; i++)
	{
		m.PositiveMult(i);
		m.DebugPrintNumber();
	}

	printf("\n%d\n", m.GetDigitSum());
}

void test() {
	MyBigInteger m(12, 100);
	m.PositiveMult(12);
	m.DebugPrintNumber();
	m.PositiveMult(513);
	m.DebugPrintNumber();

	printf("%d\n", m.GetDigitSum());
	}

int main()
{
	//test();
	Solve();

	return 0;
}

