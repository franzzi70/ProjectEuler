// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <iostream>
#include <chrono>
#include <vector>
#include <bitset>

class MyBigDecimal
{
public:
    MyBigDecimal()
    {
        m_digits = std::vector<char>(1,'0');
    }
    MyBigDecimal(int len)
    {
        m_digits = std::vector<char>(len, '0');
    }

    MyBigDecimal(char value)
    {
        m_digits = std::vector<char>(1, value);
    }

    int length()
    {
        return (int)m_digits.size();
    }

    static MyBigDecimal add(MyBigDecimal& v1, MyBigDecimal& v2)
    {
        int l1 = v1.length();
        int l2 = v2.length();
        int maxLength = std::max(l1, l2);
        MyBigDecimal result(maxLength);
        int carry = 0;
        for (int i = 0; i < maxLength; i++)
        {
			int d1 = 0;
            if (i < l1)
            {
				d1 = v1.m_digits[i] - '0';
			}
			int d2 = 0;
            if (i < l2)
            {
				d2 = v2.m_digits[i] - '0';
			}
			int sum = d1 + d2 + carry;
			carry = sum / 10;
			result.m_digits[i] = (sum % 10) + '0';
		}
        if (carry > 0)
        {
            result.m_digits.push_back(carry + '0');
        }
        return result;
    }

    std::vector<char> m_digits;
};

MyBigDecimal operator+(MyBigDecimal& v1, MyBigDecimal& v2)
{
    return MyBigDecimal::add(v1, v2);
}

bool checkPandigual(const std::vector<char>::iterator& it1, const std::vector<char>::iterator& it2)
{
    std::bitset<9> digits;
    for (auto it = it1; it != it2; it++)
    {
        char digit = *it;
        if (digit == '0')
        {
			return false;
		}

        if (digits.test(digit - '0' - 1))
        {
            return false;
        }
        else
            digits.set(digit - '0' - 1);
    }
    return true;
}

bool checkPandigualBoth(MyBigDecimal& d)
{
    if (!checkPandigual(d.m_digits.begin(), d.m_digits.begin() + 9))
        return false;

    if (!checkPandigual(d.m_digits.end()-9, d.m_digits.end()))
        return false;

	return true;

}

int solve()
{
    std::vector<MyBigDecimal> fibs;
    fibs.push_back(MyBigDecimal('1'));
    fibs.push_back(MyBigDecimal('1'));

    int i = 2;
    do
    {
        MyBigDecimal newFib = fibs[0] + fibs[1];
        fibs[0] = fibs[1];
        fibs[1] = newFib;

        if (i > 100 /*2748*/)
        {
            if (checkPandigualBoth(newFib))
            {
                return i + 1;
            }
        }

        if (i % 1000 == 0)
        {
			std::cout << "i: " << i << std::endl;
		}

        //fibs.push_back(newFib);

        i += 1;

    } while (true);

    return 0;
}

int main()
{

    auto t1 = std::chrono::high_resolution_clock::now();
    int solution = solve();
    auto t2 = std::chrono::high_resolution_clock::now();
    auto microSec = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
    std::cout << "solution: " << solution << std::endl << "duration: " << microSec << " micro seconds (" << ms << " ms)" << std::endl;
}
