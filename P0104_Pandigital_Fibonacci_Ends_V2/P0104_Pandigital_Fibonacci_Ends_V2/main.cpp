// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <iostream>
#include <chrono>
#include <vector>
#include <bitset>


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


int fib(int n)
{
    int fibs[2] { 0, 1 };

	if (n == 0)
		return 0;
	if (n == 1)
		return 1;

    int fibNew = 0;
    for (int i = 2; i <= n; i++)
    {
		fibNew = fibs[0] + fibs[1];
        fibs[i % 2] = fibNew;
    }
    return fibNew;
}

double xTimesPhiPow(int x, int power = 1)
{
    double phi = (1 + sqrt(5)) / 2;
    
    double phiPow = pow(phi, power);
    return x * phiPow;
}

bool checkPandigLow(__int64 fib)
{
    if (fib < 100'000'000)
		return false;

    __int64 rest = fib % 1'000'000'000;
    std::bitset<9> digits;
    while (rest > 0)
    {
        char digit = rest % 10;
        rest /= 10;

        if (digit == 0)
        {
            return false;
        }

        if (digits.test(digit-1))
        {
            return false;
        }
        else
            digits.set(digit- 1);
    }
    return true;
}

bool checkPandigHigh(__int64 fib)
{
    if (fib < 1'000'000'000L)
        return false;

    // normalize to 9 digits
    // first find highest digit:
    __int64 rest = fib;
    while (rest >= 1'000'000'000L)
    {
		rest /= 10;
	}
    return checkPandigLow(rest);
}


bool checkPandigBoth(__int64 fibLow, __int64 fibHigh)
{
    if (checkPandigLow(fibLow))
    {
        if (checkPandigHigh(fibHigh))
        {
			return true;
		}
	}
	return false;
}   

int solve()
{

    __int64 fibLow[2] = { 0, 1 };
    __int64 fibHigh[2] = { 0, 1 };

    int i = 2;
    do
    {

        __int64 newFibLow = fibLow[0] + fibLow[1];
        __int64 newFibHigh = fibHigh[0] + fibHigh[1];
        if (i % 2 == 0)
        {
            fibLow[0] = newFibLow;
            fibHigh[0] = newFibHigh;
        }
        else
        {
			fibLow[1] = newFibLow;
			fibHigh[1] = newFibHigh;
        }

        newFibLow %= 10'000'000'000;
        if (newFibHigh > 100000000000000000L)
        {
            newFibHigh /= 10;
		}

        if (newFibLow > 1000000000)
        {
            if (checkPandigBoth(newFibLow, newFibHigh))
                return i;
		}

        i += 1;
    }
    while (true);

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
