#include <iostream>
#include <time.h>
#include <vector>


/*
a = m^2 - n^2
b = 2 * m * n
c = m^2 + n^2
*/


int test(int M)
{
    //int M = 100;
    //int sqrtM = round(sqrt((double)M));
    //int maxC = ceil(sqrt(M * M * 3));
    //int maxCSquared = maxC * maxC;

    int  count = 0;
    for (int l1 = 1; l1 <= M; l1++)
    {
        for (int l2 = 1; l2 <= l1; l2++)
        {
            for (int l3 = 1; l3 <= l2; l3++)
            {
                int c2 = l1 * l1 + (l2 + l3) * (l2 + l3);
                int sqC = (int)round(sqrt(c2));
                if (sqC * sqC == c2)
                {
                    count += 1;
                }
            }
        }
    }
    return count;
}


int getCount(int M, int MPrev, int countPrev)
{
    //int M = 100;
    //int sqrtM = round(sqrt((double)M));
    //int maxC = ceil(sqrt(M * M * 3));
    //int maxCSquared = maxC * maxC;

    int  count = countPrev;

    for (int l1 = MPrev + 1; l1 <= M; l1++)
    {
        for (int l2 = 1; l2 <= l1; l2++)
        {
            for (int l3 = 1; l3 <= l2; l3++)
            {
                int c2 = l1 * l1 + (l2 + l3) * (l2 + l3);
                int sqC = (int)round(sqrt(c2));
                if (sqC * sqC == c2)
                {
                    count += 1;
                }
            }
        }
    }
    return count;
}

int solve()
{
    int lastCount = 0;
    int lastM = 1;
    for (int i = 2;; i++)
    {
        int MCount = getCount(i, i - 1, lastCount);
        lastCount = MCount;
        if (MCount > 1000000)
            return i;
    }
    return 0;
}

int main()
{
    time_t t1 = clock();
    int solution = solve();
    time_t t2 = clock();
    int ms = (int)(t2 - t1) * 1000 / CLOCKS_PER_SEC;

    std::cout << "solution: " << solution << std::endl << "duration: " <<  ms << " ms" << std::endl;
}
