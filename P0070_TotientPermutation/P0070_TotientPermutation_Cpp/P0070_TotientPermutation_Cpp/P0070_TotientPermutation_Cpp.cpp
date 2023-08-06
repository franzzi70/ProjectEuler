
#include <iostream>
#include <string>
#include <algorithm>

int MAXNUM = 10000000;
int primes[10000];
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

int NumberPrimes[100];
int CalculatePhi(int n)
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

    double Phi = n;
    for (int i = 0; i < vecSize; i++)
    {
        Phi *= 1 - (((double)1) / NumberPrimes[i]);
    }
    return (int)round(Phi);
}



int toDigits(int n, char* buf)
{
    int digitCount = 0;
    int r = n;
    while (r > 0)
    {
        char d = r % 10;
        buf[digitCount] = d;
        r /= 10;
        digitCount += 1;
    }
    //memset(buf + digitCount, ' ', 10 - digitCount);
    for (int i = digitCount; i < 10; i++)
    {
        buf[i] = ' ';
    }
    return digitCount;
}

bool consumeDigit(char* digits, char digit, int digitCount)
{
    for (int i = 0; i < digitCount; i++)
    {
        if (digits[i] == digit)
        {
            digits[i] = ' ';
            return true;
        }
    }
    return false;
}
char digits1[10];
char digits2[10];
bool checkPerm2(int a, int b)
{
    int digitCount1 = toDigits(a, digits1);
    int digitCount2 = toDigits(b, digits2);
    if (digitCount1 != digitCount2)
        return false;
    for (int i = 0; i < digitCount1; i++)
    {
        if (!consumeDigit(digits2, digits1[i], digitCount2))
            return false;

    }
    return true;
}

bool checkPerm(int a, int b)
{
    std::string dstr1 = std::to_string(a);
    std::string dstr2 = std::to_string(b);
    if (dstr1.length() != dstr2.length())
        return false;
    std::sort(dstr1.begin(), dstr1.end());
    std::sort(dstr2.begin(), dstr2.end());
    if (dstr1.compare(dstr2) == 0)
        return true;
    return false;
}

int solve()
{
    int primeMax = (int)ceil(sqrt(MAXNUM));
    createPrimes(primeMax);
    double LowestNOverPhi = 100;
    int NforLowestValue = 0;

    for (int i = 2; i <= MAXNUM; i++)
    {
        if (i % 100000 == 0)
        {
            std::cout << i << std::endl;
        }
        int Phi = CalculatePhi(i);

        if (checkPerm2(i, Phi))
        {
            double NOverPhi = ((double)i) / Phi;

            if (NOverPhi <= LowestNOverPhi)
            {
                std::cout << NOverPhi << std::endl;
                LowestNOverPhi = NOverPhi;
                NforLowestValue = i;
            }
        }
    }

    return NforLowestValue;
}

int main()
{
    time_t t1 = clock();
    std::cout << "solution: " << solve() << std::endl;
    time_t t2 = clock();
    std::cout << "duration: " << ((double)(t2 - t1)) * 1000 / CLOCKS_PER_SEC << "ms." << std::endl;
    std::cout << "CLOCKS_PER_SEC: " << CLOCKS_PER_SEC << std::endl;
}


