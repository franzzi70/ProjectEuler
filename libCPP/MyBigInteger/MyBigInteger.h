#pragma once

#include <vector>

class MyBigInteger
{
public:
	MyBigInteger();
	MyBigInteger(const MyBigInteger& src);
	MyBigInteger(__int32 val, int base);
	MyBigInteger(__int32 val);
	~MyBigInteger();
	bool operator < (const MyBigInteger& other);
	int getHighestNonZeroIndex(const MyBigInteger& obj);
	void DebugPrintNumber();
	int GetDigitSum();
	void PositiveMult(int factor);

private:
	std::vector<__int32> num;
	int m_base;
	const int DEFAULT_BASE = 1000000;

};
