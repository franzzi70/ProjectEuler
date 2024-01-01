// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <iostream>
#include <chrono>
#include <vector>
#include <algorithm>

#include <stdio.h>

#define N 10000
constexpr int arrSize(int n) { return (n + 1) * (n + 1); }

int combArr[arrSize(N)];

void init()
{
    int highI = arrSize(N);
    for (int i = 0; i < highI; i++)
        combArr[i] = -1;
}

// Function to calculate combination (n, k)
int comb(int n, int k) {

    int arrValue = combArr[n * N + k];
    if (arrValue != -1) {
        return arrValue;
	}

    if (k == 0 || k == n) {
        return 1;
    }
    int result = comb(n - 1, k - 1) + comb(n - 1, k);
    combArr[n * N + k] =  result;
    return result;
}


int solve()
{
    init();

    int n= 12;
    int count = 0;
    for (int l = 2; l*2 <= n; l += 1)
    {
        int iCount = 0;
        std::vector<int>v(l * 2, 1);
        for (int i = 0; i < l; i++)
        {
            v[i] = -1;
        }
        do
        {
			int rsum = 0;
            bool check = true;
            int highI = l * 2 - 1;
            if (v[highI] == -1)
            {
				check = false;
				continue;
			}
            for (int i = highI; i >=0; i--)
            {
				rsum += v[i];
                if (rsum < 0)
                {
                    check = false;
                    break;
                }
			}
            if (!check)
            {
				iCount++;
			}
        } while (std::next_permutation(v.begin(), v.end()));
        // how many such len l*2 subsets are possible in length n ?
        int f = comb(n, l * 2);
        count += f * iCount;
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
