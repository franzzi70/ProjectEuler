#include <iostream>
#include <vector>
#include <list>
#include <bitset>
#include <ctime>

// #define USEINCLUSIONEXCLUSION

struct factorInfo {
    int factor;
    int count;
    // std::bitset<168> factorbitset;
};

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
        for (int i=0 ; i<primesCount ; i++)
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

void createFactors(int num)
{

}

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


bool next_combination(int comb[], int elementCount, int combinationSize)
{
    bool moved = false;
    for (int i = 0 ; i<combinationSize ; i++)
    {
        int ix = comb[combinationSize - 1 - i];
        if (ix < elementCount -1 - i)
        {
            int newIx = ix + 1;
            comb[combinationSize - 1 - i] = newIx;
            for (int j = 1 ; j <= i ; j++)
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
            for (int i=0; i < combinationSize; i++)
                factor *= NumberPrimes[comb[i]];
            sum += (n - 1) / factor;
        }
        while
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
int CalculatePhi2(int n)
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
    if (vecSize == 0)
    {
        NumberPrimes[vecSize] = n;
        vecSize += 1;
    }

    int possibleNumbers = calcPossibleNumbers(n, NumberPrimes, vecSize, vecSize);
    int Phi = n - 1 - possibleNumbers;

    return Phi;
}

int CalculatePhi(int n)
{
#ifdef USEINCLUSIONEXCLUSION
    return CalculatePhi2(n);
#endif
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
    if (vecSize == 0)
    {
        NumberPrimes[vecSize] = n;
        vecSize += 1;
    }

    double Phi = n;
    for (int i = 0; i<vecSize ; i++)
    {
        Phi *= 1 - (((double)1) / NumberPrimes[i]);
    }
    return (int)round(Phi);
}


int solve()
{
    int primeMax = (int)ceil(sqrt(MAXNUM));
    createPrimes(primeMax);
    double HighestNOverPhi = 0;
    int NforHighestValue = 0;

    for (int i = 2; i <= MAXNUM; i++)
    {
        int Phi = CalculatePhi(i);
        double NOverPhi = ((double)i) / Phi;

        if (NOverPhi > HighestNOverPhi)
        {
            HighestNOverPhi = NOverPhi;
            NforHighestValue = i;
        }
    }

    return NforHighestValue;
}

int main(int argc, char** argv)
{
    time_t t1 = clock();
    std::cout << "solution: " << solve() << std::endl;
    time_t t2 = clock();
    std::cout << "duration: " << ((double)(t2 - t1)) * 1000 / CLOCKS_PER_SEC << "ms." << std::endl;
    std::cout << "CLOCKS_PER_SEC: " << CLOCKS_PER_SEC << std::endl;
}


