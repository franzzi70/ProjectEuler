#include <iostream>
#include <time.h>
#include <array>

const int MAXLEN = 1500000;
std::array<int, MAXLEN+1> arrLines;

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

int solve()
{
    int count = 0;
    arrLines.fill(0);
    int m = 2;
    int n = 1;
    for( int m=2 ; m * m < MAXLEN ; m++)
    {
        for (int n = 1; n < m; n++)
        {
            if (ggt(n, m) == 1 && !(n % 2 == 1 && m % 2 == 1))
            {
                // primitives pythagoreisches tripel
                int len = m * (m + n) * 2;
                if (len >= MAXLEN)
                    break;
                for (int i = len; i <= MAXLEN; i += len)
                {
                    arrLines[i] += 1;
                }
            }
        }
    }

    for (int i = 2; i <= MAXLEN; i++)
    {
        if (arrLines[i] == 1)
        {
            count += 1;
        }
    }
    return count;
}

int main()
{
    time_t t1 = clock();
    int solution = solve();
    time_t t2 = clock();
    int ms = (int)(t2 - t1) * 1000 / CLOCKS_PER_SEC;

    std::cout << "solution: " << solve() << std::endl << "duration: " <<  ms << " ms" << std::endl;
}
