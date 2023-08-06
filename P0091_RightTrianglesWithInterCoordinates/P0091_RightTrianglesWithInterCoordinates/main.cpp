
#include <iostream>
#include <time.h>



bool checkOrth(int  x1, int y1, int x2, int y2)
{
    if (x1 == 0 && y1 == 0)
        return false;

    if (x2 == 0 && y2 == 0)
        return false;

    if (x1 == x2 && y1==y2)
    {
        return false;
    }

    if (x1 == 0 && x2 == 0 || y1 == 0 && y2 == 0)
    {
        return false;
    }


    if (x1 * (x2-x1) + y1 * (y2-y1) == 0)
        return true;

    if (x2 * (x1 - x2) + y2 * (y1 - y2) == 0)
        return true;

    if (x1 * x2 + y1 * y2 == 0)
        return true;



    return false;
}


int solve()
{

    const int width = 50;
    const int heigth = 50;

    int sum = 0;

    for (int x1 = 0; x1 <= width; x1++)
    {
        for (int y1 = 0; y1 <= heigth; y1 ++)
        {
            for (int y2 = y1 + 1; y2 <= heigth; y2++)   // all points with equal x and higher y
            {
                if (checkOrth(x1, y1, x1, y2))
                {
                    sum += 1;
                }
            }
            for (int x2 = x1 +1 ; x2 <= width; x2++)   // all points with higher x
            {
                for (int y2 = 0; y2 <= heigth; y2++)
                {
                    if (checkOrth(x1, y1, x2, y2))
                    {
                        sum += 1;
                    }
                }
            }
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
