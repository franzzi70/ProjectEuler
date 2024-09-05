#include "stdafx.h"
#include "MyBigInteger.h"

#include <vector>
#include <stack>



MyBigInteger::MyBigInteger()
{
	m_base = DEFAULT_BASE;
	num.push_back(0);
}

// copy construcor:
MyBigInteger::MyBigInteger(const MyBigInteger& src)
{
	this->m_base = src.m_base;
	this->num = src.num;
}

MyBigInteger::MyBigInteger(__int32 val, int base) {
	m_base = base;
	num.push_back(val);
}

MyBigInteger::~MyBigInteger()
{
}

/*
constexpr bool operator()(const _Ty& _Left, const _Ty& _Right) const
{	// apply operator< to operands
	return (_Left < _Right);
}
*/

bool MyBigInteger::operator < (const MyBigInteger& other)
{
	int ix1 = getHighestNonZeroIndex(*this);
	int ix2 = getHighestNonZeroIndex(other);
	if (ix1 < ix2)
		return true;
	if (ix1 > ix2)
		return false;
	if (num[ix1] < other.num[ix1])
		return true;
	else
		return false;
}

//class MyBigIntegerComperator
//{
//public:
//	MyBigIntegerComperator()
//	{}
//	bool operator() (const MyBigInteger& o1, const MyBigInteger& o2) const
//	{
//		return o1 < o2;
//	}
//};

//MyBigInteger& MyBigInteger::operator< (MyBigInteger& other)
//{
//	int ix1 = getHighestNonZeroIndex();
//	int ix2 = other.getHighestNonZeroIndex();
//	if (ix1 < ix2)
//		return *this;
//	if (ix1 > ix2)
//		return other;
//	if (num[ix1] < other.num[ix1])
//		return *this;
//	else
//		return other;
//
//	return other;
//}


MyBigInteger::MyBigInteger(__int32 val) {
	m_base = DEFAULT_BASE;
	num.push_back(val);
}

void MyBigInteger::DebugPrintNumber()
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

int MyBigInteger::GetDigitSum()
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

void MyBigInteger::PositiveMult(int factor) {
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



int MyBigInteger::getHighestNonZeroIndex(const MyBigInteger& obj)
{
	int ix = obj.num.size() - 1;
	while (ix >= 0 && obj.num[ix] == 0)
	{
		ix -= 1;
	}
	return ix;
}
