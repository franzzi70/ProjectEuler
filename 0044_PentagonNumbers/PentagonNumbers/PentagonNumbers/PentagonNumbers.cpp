// PentagonNumbers.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <set>
#include <iostream>
#include <exception>
#include <string>

class NumberTooHighException : public std::exception
{
public:
	NumberTooHighException(const char* msg) : std::exception::exception(msg)
	{
		std::cout << "create exception." << std::endl;
	}
};


class PentagonNumbers {
public:
	PentagonNumbers() {
		int InitUpper = 100;
		int num = 0;
		for (int i = 1; i <= InitUpper; i++)
		{
			num = i * (3 * i - 1) / 2;
			m_pentagonNumbers.insert(num);
		}
		m_highestPentagonIndex = InitUpper;
		m_highestPentagonNumber = num;
	};

	bool isPentagon(int num)
	{
		if (num > m_highestPentagonNumber)
		{
			throw new NumberTooHighException("numberTooHigh");
		}
		if (m_pentagonNumbers.count(num) != 0)
			return true;
		else
			return false;
	}

private:
	int m_highestPentagonIndex = 0;
	int m_highestPentagonNumber = 0;
	std::set<int> m_pentagonNumbers;
};




int solve()
{

	PentagonNumbers pn;
	try {
		pn.isPentagon(100000);
	}
	catch (NumberTooHighException &e)
	{
		std::cout << "got NumberTooHighExcetion." << std::endl;
	}
	catch (std::exception &e)
	{
		std::cout << "got exception." << std::endl;
	}
	
	return 0;
}

int main()
{

	std::cout << solve() << std::endl;

    return 0;
}

