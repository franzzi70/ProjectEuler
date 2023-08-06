#include <iostream>
#include <time.h>
#include <vector>

const int SEARCHLIMIT = 10000;
std::vector<int> primes;
int primesCount = 0;

void createPrimes(int PrimeMax)
{
    int InitialVecorSize = 1.2 * (double)PrimeMax / log(((double)PrimeMax));
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
    createPrimes(SEARCHLIMIT);
}

int64_t callCount = 0;
int countTerms(int n, int sumDone, int primeIx)
{
    callCount += 1;

    int testPrime = primes[primeIx];
    int rest = n - sumDone;
    int count = 0;

    if (primeIx == 0)   // down to level prime number 2 to check if sum possible.
    {
        if (rest % 2 == 0)
            return 1;
        else
            return 0;
    }
    for (int i = sumDone; i <= n; i += testPrime)
    {
        count += countTerms(n, i, primeIx - 1);
    }

    return count;
 }

int check(int n)
{
    int PrimeIx = 0;
    do {
        if (primes[PrimeIx + 1] >= n)
            break;
        PrimeIx += 1;
    } while (true);

    int TermCount = countTerms(n, 0, PrimeIx);
    return TermCount;
}

void test()
{
    int solutionCount = check(10);
    std::cout << callCount << std::endl;
    return;
}
int solve()
{
    init();
    int TermCount;
    int solution = 0;
    //test();
    for (int i = 7; i < SEARCHLIMIT; i++)
    {
        TermCount = check(i);
        if (TermCount > 5000)
        {
            solution = i;
            break;
        }
    }
    
    return solution;
}

int main()
{
    time_t t1 = clock();
    int solution = solve();
    time_t t2 = clock();
    int ms = (int)(t2 - t1) * 1000 / CLOCKS_PER_SEC;

    std::cout << "solution: " << solve() << std::endl << "duration: " <<  ms << " ms" << std::endl;
}
