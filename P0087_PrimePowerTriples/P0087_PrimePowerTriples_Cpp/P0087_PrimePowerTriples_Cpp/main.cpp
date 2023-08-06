#include <iostream>
#include <time.h>
#include <bitset>
#include <vector>

const int SEARCHLIMIT = 50000000;
std::vector<int> primes;
int primesCount = 0;

void createPrimes(int PrimeMax)
{
    int InitialVecorSize = (int)(1.2 * (double)PrimeMax / log(((double)PrimeMax)));
    primes.reserve(InitialVecorSize);

    primes.push_back(2);
    primesCount = 1;

    // int storedMax = ceil(sqrt(PrimeMax));
    for (int testP = 3; testP <= PrimeMax; testP += 2)
    {
        for (int i = 0; i < primesCount; i++)
        {
            int p = primes[i];
            if (testP % p == 0)
                break;
            if (p * p > testP)
            {
                //primes[primesCount] = testP;
                primes.push_back(testP);
                primesCount++;
                break;
            }
        }
    }
}

void init()
{
    int PrimeMax = (int) ceil(sqrt(SEARCHLIMIT));
    createPrimes(PrimeMax);
}


int solve(int& collisions)
{
    static std::bitset<SEARCHLIMIT> nums;
    collisions = 0;
    init();
    int i = 0, j = 0, k = 0;
    int p1, p2, p3;
    int count = 0;
    int valK = 0;
    int maxK = (int) ceil(sqrt(sqrt(SEARCHLIMIT)));
    do
    {
        j = 0;
        p3 = primes[k];
        valK = p3 * p3 * p3 * p3;
        int valJK = 0;
        do {
            i = 0;
            p2 = primes[j];
            valJK = valK + (p2 * p2 * p2);
            int valIJK = 0;
            do {
                p1 = primes[i];
                int valIJK = valJK + p1 * p1;
                if (valIJK < SEARCHLIMIT)
                {
                    if (nums.test(valIJK))
                    {
                        collisions += 1;
                    }
                    else
                    {
                        nums.set(valIJK);
                        count += 1;
                    }
                }
                i += 1;
            } while (valIJK < SEARCHLIMIT && i < primesCount);

            j += 1;
        } while (valJK < SEARCHLIMIT);

        k += 1;
    } while (p3 <= maxK);

    return count;
}

int main()
{
    int collissions = 0;
    time_t t1 = clock();
    int solution = solve(collissions);
    time_t t2 = clock();
    int ms = (int)(t2 - t1) * 1000 / CLOCKS_PER_SEC;

    std::cout << "solution: " << solution << std::endl << "duration: " <<  ms << " ms" << std::endl;
    std::cout << "collisions: " << collissions << std::endl;
}
