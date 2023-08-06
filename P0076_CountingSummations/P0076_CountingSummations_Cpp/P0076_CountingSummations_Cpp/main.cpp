#include <iostream>
#include <time.h>
#include <array>

const int HIGHNUM = 100;
const int HIGHNUMPLUS = HIGHNUM + 1;

std::array<int64_t, (HIGHNUMPLUS) * (HIGHNUMPLUS)>arrMem;
int64_t getNhighH(int64_t n, int64_t h)
{
    // memoization makes it about 1000 times faster.
    int arrIx = (n - 1) * (HIGHNUMPLUS) + h - 1;
    if (arrIx >= 0 && arrMem[arrIx] != -1)
        return arrMem[arrIx];
    int64_t sum = 0;
    if (n == 0)
        return 1;
    if (n>=h && h == 1)
        return 1;
    if (n < h)
        return getNhighH(n, h-1);
    if (h == 2)
    {
        return ((int64_t)(n / h)) + 1;
    }

    for (int i = 0; i <= n; i+=h)
    {
        int64_t val = getNhighH(n - i, h - 1);
        sum += val;
    }
    arrMem[arrIx] = sum;
    return sum;
}

int solve()
{
    arrMem.fill(-1);
    int64_t solution = getNhighH(HIGHNUM, HIGHNUM-1);
    return solution;
    //return getNhighH(100, 100) -1;
}

int main()
{
    time_t t1 = clock();
    int solution = solve();
    time_t t2 = clock();
    int ms = (int)(t2 - t1) * 1000 / CLOCKS_PER_SEC;

    std::cout << "solution: " << solve() << std::endl << "duration: " <<  ms << " ms" << std::endl;
}
