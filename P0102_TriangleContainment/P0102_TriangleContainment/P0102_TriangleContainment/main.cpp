// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <iostream>
#include <chrono>
#include <vector>
#include <algorithm>

extern std::vector<std::vector<int>> triangles;

bool containsOrigin(std::vector<int> triangle)
{
    double cx[2] = { 0, 0 };
    double cy[2] = { 0, 0 };
    int xCount = 0;
    int yCount = 0;

    for (int i = 0; i < 3; i++)
    {
        int xIx1 = i * 2;
        int xIx2 = (i + 1) * 2 % 6;
        int x1 = triangle[xIx1];
        int x2 = triangle[xIx2];
        int yIx1 = xIx1 + 1;
        int yIx2 = xIx2 + 1;
        int y1 = triangle[yIx1];
        int y2 = triangle[yIx2];

        if ((bool)(x1 < 0) ^ (bool)(x2 < 0))
        {
            // crosses x axis
            double yCrossingPoint = (double)(y2 - y1) / (double)(x2 - x1) * (double)(0 - x1) + y1;
            cy[yCount] = yCrossingPoint;
            yCount += 1;
        }
        if ((bool)(y1 < 0) ^ (bool)(y2 < 0))
        {
            // crosses y axis
            double xCrossingPoint = (double)(x2 - x1) / (double)(y2 - y1) * (double)(0 - y1) + x1;
            cx[xCount] = xCrossingPoint;
            xCount += 1;
        }
    }

    if (xCount == 2 && yCount == 2)
    {
        if (
            ((cx[0] < 0) ^ (cx[1] < 0))
            &&
			((cy[0] < 0) ^ (cy[1] < 0))
			)
		{
			return true;
		}
    }
    return false;
}


int solve()
{
    int count = 0;
    int TriangleCount = triangles.size();
    for (int i = 0; i < TriangleCount; i++)
	{
		if (containsOrigin(triangles[i]))
		{
			count += 1;
		}
	}
    return count;
}

int main()
{

    auto t1 = std::chrono::high_resolution_clock::now();
    int solution = solve();
    auto t2 = std::chrono::high_resolution_clock::now();
    auto microSec = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
    std::cout << "solution: " << solution << std::endl << "duration: " << microSec << " micro seconds (" << ms << " ms)" << std::endl;
}
