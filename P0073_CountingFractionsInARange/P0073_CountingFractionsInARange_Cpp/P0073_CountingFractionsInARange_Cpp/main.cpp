#include <iostream>
#include <time.h>

int ggt(int a, int b)
{
    int _a = a, _b = b;
    int r;
    do {
        r = _a % _b;
        if (r == 0)
        {
            // std::cout << "ggt(" << a << "," << b << ") = " << _b << std::endl;
            return _b;
        }

        _a = _b;
        _b = r;
    } while (true);

}

int countFractions(int denom)
{
    int count = 0;

    int numHigh = denom / 2;
    if (numHigh * 2 >= denom)
        numHigh -= 1;

    int numLow = denom / 3;
    if (numLow * 3 <= denom)
        numLow += 1;

    for (int i = numLow; i <= numHigh; i++)
    {
        if (ggt(i, denom) == 1)
        {
            count += 1;
        }
    }
    return count;
}

int solve()
{
    int64_t sum = 0;
    for (int i = 2; i <= 12000; i++)
    {
        sum += countFractions(i);
    }
    return sum;
}

int main()
{
    time_t t1 = clock();
    int solution = solve();
    time_t t2 = clock();
    int ms = (int)(t2 - t1) * 1000 / CLOCKS_PER_SEC;

    std::cout << "solution: " << solve() << std::endl << "duration: " <<  ms << " ms" << std::endl;
}
