#pragma once

#include <vector>

class MyBigInteger
{
public:
	MyBigInteger();
	MyBigInteger(const MyBigInteger& src); // copy constructor
	MyBigInteger(__int32 val, int base);
	MyBigInteger(__int32 val);
	~MyBigInteger();
	bool operator < (const MyBigInteger& other);
	void DebugPrintNumber();
	int GetDigitSum();
	void PositiveMult(int factor);

private:
	static int getHighestNonZeroIndex(const MyBigInteger& obj);
	std::vector<__int32> num;
	int m_base;
	const int DEFAULT_BASE = 1000000;

friend class MyBigIntegerComperator;
};

class MyBigIntegerComperator
{
public:
	MyBigIntegerComperator()
	{}
	bool operator() (const MyBigInteger& o1, const MyBigInteger& o2) const
	{
		int ix1 = o1.getHighestNonZeroIndex(o1);
		int ix2 = o2.getHighestNonZeroIndex(o2);
		if (ix1 < ix2)
			return true;
		if (ix1 > ix2)
			return false;
		if (o1.num[ix1] < o2.num[ix1])
			return true;
		else
			return false;
	}
};
