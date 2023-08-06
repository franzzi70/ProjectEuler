
#include <iostream>
#include <time.h>



bool hasIntegerSquareRoot(__int64 n, __int64& result)
{
    __int64 q = (__int64)round(sqrt(n));
    if (q * q == n)
    {
        result = q;
        return  true;
    }
    else
        return false;
}


bool testTriangle(__int64 baseLen, __int64 sideLen)
{
    __int64 result;
    if (hasIntegerSquareRoot(sideLen * sideLen * 4 - baseLen * baseLen, result))
    {
        if ((result % 4 != 0) && (baseLen % 2 != 0))
            return false;
        else
            return true;
    }
    return false;
}

__int64 solve()
{
    __int64 sum = 0;
    const __int64 maxSideLen = 1000 * 1000 * 1000;

    for (__int64 i = 2; i <= maxSideLen; i++)
    {
        __int64 baseLen = i - 1;
        __int64 perim = ((__int64)baseLen) + ((__int64)i) * 2;
        
        if (testTriangle(baseLen, i))
        {
            if (perim <= maxSideLen)
                sum+= perim;
        }

        baseLen = i + 1;
        perim += 2;

        if (testTriangle(baseLen, i))
        {
            if (perim <= maxSideLen)
                 sum += perim;
        }

        if (perim > maxSideLen)
            break;
    }
    return sum;
}

int main()
{
    time_t t1 = clock();
    __int64 solution = solve();
    time_t t2 = clock();
    int ms = (int)(t2 - t1) * 1000 / CLOCKS_PER_SEC;

    std::cout << "solution: " << solution << std::endl << "duration: " << ms << " ms" << std::endl;
}
