// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <iostream>
#include <chrono>
#include <vector>

const bool PRINTSEQUENCES = false;
const bool PRINTFOUNDNUMBERS = false;

#define DIGITCOUNT 8

__int64 g_digitValues[DIGITCOUNT][10];

const __int64 SQUARENUMBERCOUNT = (__int64) sqrt(exp(log(10) * (DIGITCOUNT))) + 2;
std::vector<__int64> g_squareNumbers(SQUARENUMBERCOUNT, 0);

//
//class PalindroFactory
//{
//    static 
//};

class PalindromFactory
{
public:
    PalindromFactory()
    {
        _initForLength(2);
    }

    __int64 nextPalindrome()
    {
        if (overflow)
        {
			return 0;
		}
        if (initial)
        {
			initial = false;
			return palindrom;
		}
        int carry = 0;
        for (int i = halfIndex; i >= 0; i--)
        {
            int d = digits[i];
            if (d == 9)
			{
                if (i == 0)
                {
                    _initForLength(digitCount + 1);
                    return nextPalindrome();
                }
                digits[i] = 0;
                int i2 = digitCount - i - 1;
                palindrom = palindrom - g_digitValues[i][9];
                if (i != i2)
				{
					digits[i2] = 0;
					palindrom = palindrom - g_digitValues[i2][9];
				}
			}
			else
			{
                int newd = d + 1;
                int i2 = digitCount - i - 1;
				digits[i] = newd;
                palindrom =
                    palindrom
                    + g_digitValues[i][newd] - g_digitValues[i][d];
                if (i != i2)
                {
                    if (i2 >= 0 && i2 < digitCount) // to please the compiler which suspects out of bounds error
                    {
                        digits[i2] = newd;
                        palindrom =
                            palindrom
                            + g_digitValues[i2][newd] - g_digitValues[i2][d];
                    }

                }
				break;
			}

		}
		return palindrom;
	}
    __int64 palindrom;
    int digitCount;
    int halfIndex;
    int digits[DIGITCOUNT];
    bool initial = false;
    bool overflow = false;

private:
    void _initForLength(int length)
	{
        if (length > DIGITCOUNT)
        {
            overflow = true;
            for (int i = 0; i < length; i++)
            {
                digits[i] = 0;
            }
            palindrom = 0;
            halfIndex = 0;
            initial = true;
            return;
        }
//        std::cout << "initForLength: " << length << std::endl;
		digitCount = length;
		halfIndex = (length-1) / 2;
		for (int i = 1; i < length-1; i++)
		{
			digits[i] = 0;
		}
        digits[0] = 1;
        digits[length - 1] = 1;
        palindrom = g_digitValues[0][1] + g_digitValues[length - 1][1];
        initial = true;
	}
};


void init()
{
    for (int i=0;i<10;i++)
    {
		g_digitValues[0][i] = i;
	}
    for (int i = 1;i< DIGITCOUNT;i++)
	{
        for (int j = 0; j < 10; j++)
        {
            g_digitValues[i][j] = g_digitValues[i - 1][j] * 10;
        }
	}

    __int64 sq = 0;
    __int64 diff = -1;
    for (int i = 0; i < SQUARENUMBERCOUNT; i++)
    {
        g_squareNumbers[i] = sq;
        diff += 2;
        sq += diff;
    }

}


bool isSumOfConsequitiveSquares(__int64 n)
{
    int len = 2;
    __int64 half = n / 2;
    int upperIx;
    int lowerIx = (int)(SQUARENUMBERCOUNT - 1);
    __int64 sum = 0;
    while (half <= g_squareNumbers[lowerIx])
	{
		lowerIx -= 1;
	}
    upperIx = lowerIx + 1;
    __int64 upperNum = g_squareNumbers[upperIx];
    __int64 lowerNum = g_squareNumbers[lowerIx];
    sum = upperNum + lowerNum;

    if (sum == n)
    {
        if (PRINTSEQUENCES)
            for (int i = 0; i < len; i++)
            {
                std::cout << g_squareNumbers[lowerIx + i] << " ";
            }
        return true;
    }

    while (true)
    {
        len += 1;
        lowerIx -= 1;
        if (lowerIx == 0)
            return false;
        lowerNum = g_squareNumbers[lowerIx];
        sum += lowerNum;
        if (sum == n)
        {
            if (PRINTSEQUENCES)
                for (int i = 0; i < len; i++)
			    {
				    std::cout << g_squareNumbers[lowerIx + i] << " ";
			    }
            return true;
        }
        while (sum > n)
		{
			sum -= upperNum;
			upperIx -= 1;
			upperNum = g_squareNumbers[upperIx];
            lowerIx -= 1;
            if (lowerIx == 0)
				return false;
            lowerNum = g_squareNumbers[lowerIx];
            sum += lowerNum;
            if (sum==n)
            {
                if (PRINTSEQUENCES)
                    for (int i = 0; i < len; i++)
                    {
                        std::cout << g_squareNumbers[lowerIx + i] << " ";
                    }
                return true;
            }
        }
    }
    return false;
}

__int64 solve()
{
    init();

    __int64 sum = 0;

    PalindromFactory f;
    __int64 palim = 0;
    while (true)
	{
		palim = f.nextPalindrome();
        if (palim == 0)
			break;
        //std::cout << palim << std::endl;
        if (isSumOfConsequitiveSquares(palim))
        {
            sum += palim;
            if (PRINTFOUNDNUMBERS)
			    std::cout << "found: " << palim << std::endl;
        }
	}

    return sum + 5; // + 1*1 + 2*2
}


int main()
{

    auto t1 = std::chrono::high_resolution_clock::now();
    __int64 solution = solve();
    auto t2 = std::chrono::high_resolution_clock::now();
    auto microSec = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
    std::cout << "solution: " << solution << std::endl << "duration: " << microSec << " micro seconds (" << ms << " ms)" << std::endl;
}
