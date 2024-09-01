// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <iostream>
#include <chrono>
#include <vector>

class RepUnit {
public:
    RepUnit()
    {
        m_oneCount = 0;
    }
    RepUnit (int n)
    {
        calculate(n);
    }
    void calculate(int n)
	{

		m_oneCount = 1;
        int lastDigit = n % 10;
        int factorTableIndex = 0;

        switch (lastDigit) {
            case 1: factorTableIndex = 0; break;
            case 3: factorTableIndex = 1; break;
            case 7: factorTableIndex = 2; break;
            case 9: factorTableIndex = 3; break;
            };

        
        int diff = 1;
        int carry = 0;

        while (true)
		{
            int factor = factorTable[factorTableIndex][diff];
            carry += factor * n;
            carry /= 10;
            if (carry == 0)
            {
                //std::cout << n << ": " << m_oneCount << std::endl;
                break;
            }
            lastDigit = carry % 10;
            diff = 11 - lastDigit;
            if (diff >= 10)
                {
				diff -= 10;
			    }
            m_oneCount += 1;
        }
	}
    int m_oneCount;
    static std::vector<std::vector<int>> factorTable;
};

std::vector<std::vector<int>> RepUnit::factorTable {
    {0,1,2,3,4,5,6,7,8,9},  // 1
    {0,7,4,1,8,5,2,9,6,3},  // 3
    {0,3,6,9,2,5,8,1,4,7},  // 7
	{0,9,8,7,6,5,4,3,2,1}   // 9
    };


int MAXNUM = 10'000'000;

int solve()
{
    int THRESHOLD = 1'000'000;

    RepUnit ru(41);
    //std::cout << "oneCount: " << ru.m_oneCount << std::endl;

    int prefix = THRESHOLD;
    RepUnit ru1;
    RepUnit ru3;
    RepUnit ru7;
    RepUnit ru9;

    while (true)
    {
        int num1 = prefix + 1;
        int num3 = prefix + 3;
        int num7 = prefix + 7;
        int num9 = prefix + 9;

        ru1.calculate(num1);
        ru3.calculate(num3);
        ru7.calculate(num7);
        ru9.calculate(num9);


        if (ru1.m_oneCount> THRESHOLD)
		{
            //std::cout << "prefix: " << prefix << " ru1: " << ru1.m_oneCount << std::endl;
			return num1;
		}
        if (ru3.m_oneCount > THRESHOLD)
		{
			//std::cout << "prefix: " << prefix << " ru3: " << ru3.m_oneCount << std::endl;
			return num3;
		}
        if (ru7.m_oneCount > THRESHOLD)
        {
            //std::cout << "prefix: " << prefix << " ru7: " << ru7.m_oneCount << std::endl;
            return num7;
		}
        if (ru9.m_oneCount > THRESHOLD)
        {
            //std::cout << "prefix: " << prefix << " ru9: " << ru9.m_oneCount << std::endl;
            return num9;
        }
        prefix += 10;
        if (prefix > MAXNUM)
        {
            std::cout << "out of range." << std::endl;
            return 0;
        }
    }

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
