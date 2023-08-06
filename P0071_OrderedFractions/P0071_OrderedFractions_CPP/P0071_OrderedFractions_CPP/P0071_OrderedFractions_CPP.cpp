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

bool checkRP(int a, int b)
{
    return (ggt(a, b) == 1);
}

bool getDistFor(int d, int& num, double& dist)
{
    if (d == 7)
        return false;
    int num1 = (d * 3) / 7;
    for (int testNum = num1; testNum > 0 ; testNum -= 1)
    {
        if (checkRP(testNum, d))
        {
            num = testNum;
            dist = ((double)3 / 7) - (double)testNum / d;
            return true;
        }
    }
    return false;
}


int solve()
{
    int num = 0;
    double dist = 1;
    double smallestDist = 1;
    int numForsmallestDist = 0;

    for (int d = 2; d <= 1000000; d++)
    {
        double d_dist;
        if (getDistFor(d, num, d_dist))
        {
            if (d_dist < smallestDist)
            {
                smallestDist = d_dist;
                numForsmallestDist = num;
            }
        }
    }
    return numForsmallestDist;
}

int main()
{
    time_t t1 = clock();
    int solution = solve();
    time_t t2 = clock();
    int ms = (int)(t2 - t1) * 1000 / CLOCKS_PER_SEC;

    std::cout << "solution: " << solve() << std::endl << "duration: " <<  ms << " ms" << std::endl;
}
