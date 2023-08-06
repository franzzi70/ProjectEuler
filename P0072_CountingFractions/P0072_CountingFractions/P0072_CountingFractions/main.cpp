#include <iostream>
#include <time.h>

int MAXNUM = 1000000;
int primes[1000];
int primesCount = 0;

void createPrimes(int PrimeMax)
{
    primes[0] = 2;
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
                primes[primesCount] = testP;
                primesCount++;
                break;
            }
        }
    }
}


bool next_combination(int comb[], int elementCount, int combinationSize)
{
    bool moved = false;
    for (int i = 0; i < combinationSize; i++)
    {
        int ix = comb[combinationSize - 1 - i];
        if (ix < elementCount - 1 - i)
        {
            int newIx = ix + 1;
            comb[combinationSize - 1 - i] = newIx;
            for (int j = 1; j <= i; j++)
            {
                comb[combinationSize - 1 - i + j] = newIx + j;
            }
            moved = true;
            break;
        }
    }
    if (moved)
        return true;
    else
    {
        for (int i = 0; i < combinationSize; i++)
            comb[i] = i;
        return false;
    }
}

int comb[100];
int calcPossibleNumbers(int n, int NumberPrimes[], int elementCount, int combinationSize)
{
    int sum = 0;
    if (combinationSize == 1)
    {
        for (int i = 0; i < elementCount; i++)
        {
            sum += (n - 1) / NumberPrimes[i];
        }
        return sum;
    }
    else {
        //int sum = calcPossibleNumbers(n, NumberPrimes, combinationSize - 1);
        // build all combinationSize combinations and calculate possible factors < n
        for (int i = 0; i < combinationSize; i++)
            comb[i] = i;
        do
        {
            int factor = 1;
            for (int i = 0; i < combinationSize; i++)
                factor *= NumberPrimes[comb[i]];
            sum += (n - 1) / factor;
        } while
            (next_combination(comb, elementCount, combinationSize));

        if (combinationSize % 2 == 0)
        {
            return (-sum) + calcPossibleNumbers(n, NumberPrimes, elementCount, combinationSize - 1);
        }
        else
        {
            return sum + calcPossibleNumbers(n, NumberPrimes, elementCount, combinationSize - 1);
        }

    }
}

int NumberPrimes[100];
int TotientCount(int n)
{
    int nq = n;
    int vecSize = 0;
    int primeIx = 0;
    int lastFoundPrime = 0;
    int PrimeCount = primesCount;

    // get prime factors of n
    while (nq > 1 && primeIx < PrimeCount)
    {
        int testPrime = primes[primeIx];
        if (nq % testPrime == 0)
        {
            NumberPrimes[vecSize] = testPrime;
            vecSize += 1;
            nq /= testPrime;
            while (nq % testPrime == 0)
            {
                nq /= testPrime;
            }
        }
        primeIx++;
    }
    if (nq > 1)
    {
        NumberPrimes[vecSize] = nq;
        vecSize += 1;
    }

    int possibleNumbers = calcPossibleNumbers(n, NumberPrimes, vecSize, vecSize);
    int Phi = n - 1 - possibleNumbers;
    return Phi;

    //double Phi = n;
    //for (int i = 0; i < vecSize; i++)
    //{
    //    Phi *= 1 - (((double)1) / NumberPrimes[i]);
    //}
    //return (int64_t)round(Phi);
}

int64_t solve()
{
    createPrimes(ceil(sqrt(1000000))+100);
    int64_t count = 0;
    for (int i = 2; i <= 1000000 ; i++)
    {
        count += TotientCount(i);
    }
    return count;
}

int main()
{
    time_t t1 = clock();
    int solution = solve();
    time_t t2 = clock();
    int ms = (int)(t2 - t1) * 1000 / CLOCKS_PER_SEC;

    std::cout << "solution: " << solve() << std::endl << "duration: " <<  ms << " ms" << std::endl;
}
