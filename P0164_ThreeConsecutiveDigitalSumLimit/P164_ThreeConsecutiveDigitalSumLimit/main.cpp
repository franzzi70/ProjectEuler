
#include <iostream>
#include <chrono>

const int DIGITCOUNT = 20;
const int ARRSIZE = DIGITCOUNT - 1;

struct rec {
    int digitsum;
    int64_t count;
};


int64_t solve()
{
    rec field[ARRSIZE][100];

    for (int i=0;i<ARRSIZE; i++)
    {
        for (int j=0; j<100; j++)
        {
            field[i][j].digitsum = 0;
            field[i][j].count = 0;
        }
	}

    for (int i = 0; i < 100; i++) {
        int d0 = i % 10;
        int d1 = i / 10;
        field[0][i].digitsum = d0 + d1;
        if (d0 + d1 <= 9)
        {
            field[0][i].count = 1;
        }
        else
        {
            field[0][i].count = 0;
        }
    }

    for (int i = 0; i < ARRSIZE-1; i++)
    {
		int digitpos = i + 3;
        int first_digit = 0;
        if (digitpos == DIGITCOUNT )
        {
            first_digit = 1;
		}
        for (int j = 0; j < 100; j++)
        {
            int j_high = j / 10;
            for (int digit = first_digit; digit <= 9; digit += 1)
            {
                rec r = field[i][j];
                if (r.digitsum + digit <= 9)
                {
                    int ix = j_high + digit * 10;
                    field[i + 1][ix].count += r.count;
                    field[i + 1][ix].digitsum = j_high + digit;
                }
            }
        }
    }

    int64_t sum = 0;
    for (int i = 0; i < 100; i++)
    {
        sum += field[ARRSIZE - 1][i].count;
    }

    return sum;
}

int main()
{

    auto t1 = std::chrono::high_resolution_clock::now();
    volatile int64_t solution = solve();
    auto t2 = std::chrono::high_resolution_clock::now();
    auto microSec = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
    std::cout << "solution: " << solution << std::endl << "duration: " << microSec << " micro seconds (" << ms << " ms)" << std::endl;
    if (microSec > 300'000'000)
    {
        std::cout << "(" << ((float)microSec) / 60'000'000 << " minutes )" << std::endl;
    }
}
