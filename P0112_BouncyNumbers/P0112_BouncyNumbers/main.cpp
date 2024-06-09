// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <iostream>
#include <chrono>
#include <vector>

class Number {
public:
    std::vector<int> m_digits;
    int m_len = 1;

    
    Number(__int64 n)
    {
        m_digits = std::vector<int>(30, 0);
        if (n == 0)
        {
            m_len = 1;
            return;
        }

        __int64 rest = n;
        int len = 0;
        while (rest > 0)
        {
            m_digits[len] = rest % 10;
            rest /= 10;
            len += 1;
        }
        m_len = len;
    }

    void nextNumber()
    {
        int carry = 1;
        int ix = 0;
        while (carry)
        {
            int incNumber = m_digits[ix] + carry;
            if (incNumber > 9)
            {
                carry = 1;
                m_digits[ix] = 0;
                if (m_len - 1 == ix)
                {
                    m_len += 1;
                }
            }
            else
            {
                carry = 0;
                m_digits[ix] = incNumber;
            }
            ix += 1;
        }
    }

    bool isBouncing()
    {
        if (m_len < 3)
            return false;

        int startDirection = 0;
        for (int ix = 0; ix < m_len - 1; ix++)
        {
            int diff = m_digits[ix + 1] - m_digits[ix];
            int dir = diff == 0 ? 0 : (diff < 0 ? -1 : 1);
            if (startDirection == 0)
            {
                startDirection = dir;
            }
            else
            {
                if (
                    startDirection != 0 && dir != 0
                    &&
                    startDirection != dir
                    )
                    return true;
            }
        }
        return false;
    }

    __int64 toNumber()
    {
        __int64 num = 0;
        __int64 f = 1;
        for (int i = 0; i < m_len; i++)
        {
            num += m_digits[i] * f;
            f *= 10;
        }
        return num;
    }
};

int solve()
{
    Number n = (1);
    int bouncyCount = 0;
    int i = 1;
    while (true)
    {
        if (n.isBouncing())
            bouncyCount += 1;
        //std::cout << "number: " << n.toNumber() << " is bouncing: " << n.isBouncing() << std::endl;
        n.nextNumber();
        if (bouncyCount * 100 >= i * 99)
            break;

        i += 1;
    }

    return i;
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
