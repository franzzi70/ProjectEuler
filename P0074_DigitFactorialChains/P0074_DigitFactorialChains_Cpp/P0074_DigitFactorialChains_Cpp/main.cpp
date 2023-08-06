#include <iostream>
#include <time.h>
#include <array>

int fac[10];
void init()
{
    fac[0] = 1;
    for (int i = 1; i < 10; i++)
    {
        fac[i] = fac[i - 1] * i;
    }
}

int64_t getSumFac(int64_t n)
{
    int64_t sum = 0;
    for (int64_t q = n; q > 0; q /= 10)
    {
        int digit = q % 10;
        sum += fac[digit];
    }
    return sum;
}

std::array<int64_t, 100> chainBuf;
int getChainLen(int n)
{
    int chainLen = 0;
    bool foundLoop = false;
    chainBuf[0] = n;
    for (int64_t nextNum = n; true ; nextNum = getSumFac(nextNum))
    {
        for (int i = 0; i < chainLen; i++)
        {
            if (chainBuf[i] == nextNum)
            {
                return chainLen;
            }
        }
        chainBuf[chainLen] = nextNum;
        chainLen += 1;
    }
}

int solve()
{
    init();
    int chainCount = 0;
    for (int i = 1; i < 1000000; i++)
    {
        int chainLen = getChainLen(i);
        if (chainLen == 60)
            chainCount += 1;
    }
    
    return chainCount;
}

int main()
{
    time_t t1 = clock();
    int solution = solve();
    time_t t2 = clock();
    int ms = (int)(t2 - t1) * 1000 / CLOCKS_PER_SEC;

    std::cout << "solution: " << solve() << std::endl << "duration: " <<  ms << " ms" << std::endl;
}
