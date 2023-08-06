
#include <iostream>
#include <time.h>

int getNumFromDigits(int testNum)
{
    int rest = testNum;
    int sum = 0;
    while (rest > 0)
    {
        int digit = rest % 10;
        sum += (int)(digit * digit);
        rest /= 10;
    }
    return sum;
}


bool checkChain89(int n)
{
    bool circleNumberFound = false;
    int testNum = n;
    while (!circleNumberFound)
    {
        testNum = getNumFromDigits(testNum);

        if (testNum == 89)
            return true;
        if (testNum == 1)
            return false;
    }
    return false;
}

int solve()
{

    int sum = 0;
    for (int i = 2; i < 10000000; i++)
    {
        if (checkChain89(i))
        {
            sum += 1;
        }
    }
    return sum;
}


int main()
{
    time_t t1 = clock();
    int solution = solve();
    time_t t2 = clock();
    int ms = (int)(t2 - t1) * 1000 / CLOCKS_PER_SEC;

    std::cout << "solution: " << solution << std::endl << "duration: " << ms << " ms" << std::endl;
}
