// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <iostream>
#include <time.h>

const int maxW = 53;
const int goalRecCount = 2000000;

int recCount(int w, int h)
{
    return (w * (w + 1) * h * (h + 1)) / 4;
    //int count = 0;
    //for (int i = 1; i <= w; i++)
    //{
    //    for (int j = 1; j <= h; j++)
    //    {
    //        count += i * j;
    //    }
    //}
    //return count;
}

int solve()
{
    int bestW = 1;
    int bestH = 1;
    int bestDist = 1000000;
    
    for (int w = 1; w <= maxW; w++)
    {
        int prevDist;
        int dist = -1000000;
        int h = 0;

        do {
            h += 1;
            prevDist = dist;
            dist = recCount(w, h) - goalRecCount;
        } while (dist < 0);
        if (dist < bestDist)
        {
            bestW = w;
            bestH = h;
            bestDist = dist;
            if (w == 36 && h == 78)
                std::cout << dist << std::endl;
        }
        if ((-prevDist) < bestDist)
        {
            bestW = w;
            bestH = h - 1;
            bestDist = (-prevDist);
        }
    }

    return bestW * bestH;
}

int main()
{
    time_t t1 = clock();
    int solution = solve();
    time_t t2 = clock();
    int ms = (int)(t2 - t1) * 1000 / CLOCKS_PER_SEC;

    std::cout << "solution: " << solution << std::endl << "duration: " << ms << " ms" << std::endl;
}
