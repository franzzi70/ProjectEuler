
#include <iostream>
#include <time.h>
#include <vector>
#include <bitset>
#include <set>
#include "primes.h"

const int MAX_NUM = 1000000;
const int SIEVESIZE = MAX_NUM;

primes::PrimesSieve sieve(SIEVESIZE);
const int PRIMEARRAYSIZE = SIEVESIZE / 10;
int primeArray[PRIMEARRAYSIZE];


class FactorInfo {
public:
    int factor;
    int count;
};


void getPrimeFactors(int n, std::vector<int>& factors, int& factorCount)
{
    int rest = n;
    int fCount = 0;
    int primeArrIx = 0;

    while (rest > 1)
    {
        int currentPrime = primeArray[primeArrIx];
        if (rest % currentPrime == 0)
        {
            rest /= currentPrime;
            factors[fCount] = currentPrime;
            fCount += 1;
        }
        else
        {
            primeArrIx += 1;
        }
    }
    factorCount = fCount;
}

void getPrimeFactorsInfo(int n, std::vector<FactorInfo>& factorInfos, int& infCount)
{
    static std::vector<int> primeFactors(30);
    int factorCount;

    getPrimeFactors(n, primeFactors, factorCount);

    int nextIx = 0;
    int infoIx = 0;
    while (nextIx < factorCount)
    {
        int f = primeFactors[nextIx];
        int groupSize = 1;
        while (nextIx < factorCount - 1)
        {
            if (primeFactors[nextIx + 1] == f)
            {
                groupSize += 1;
                nextIx += 1;
            }
            else
                break;
        }
        factorInfos[infoIx] = FactorInfo{ f, groupSize };
        infoIx += 1;
        nextIx += 1;
    }
    infCount = infoIx;
}


int getFactorSum(std::vector<FactorInfo>& factorInfos, int len)
{
    int f = factorInfos[len - 1].factor;
    int count = factorInfos[len - 1].count;

    int testFactor = 1;
    int sum = 0;
    int nextFactorSum = 1;
    if (len > 1)
    {
        nextFactorSum = getFactorSum(factorInfos, len - 1);
    }

    for (int i = 0; i <= count; i++)
    {
        sum += nextFactorSum * testFactor;
        testFactor *= f;
    }
    return sum;
}

int getDivisorSum(int n)
{

    std::vector<FactorInfo> factorInfos(30);
    int infCount = 0;

    std::set<int> foundDivisors;


    if (sieve.isPrime(n))
    {
        return n;
    }

    getPrimeFactorsInfo(n, factorInfos, infCount);
    int factorSum = getFactorSum(factorInfos, infCount);

    return factorSum - n;   // subtract the generated full multiplication, because it is not counted as divisor.
}

void init()
{
    int primeArrIx = 0;
    for (int i = 2; i < SIEVESIZE; i++)
    {
        if (primeArrIx >= PRIMEARRAYSIZE)
            break;

        if (sieve.isPrime(i))
        {
            primeArray[primeArrIx] = i;
            primeArrIx += 1;
        }
    }
}


int solve()
{
    init();

    /*
    int s1 = getDivisorSum(28);
    int s2 = getDivisorSum(220);
    int s3 = getDivisorSum(284);
    int s4 = getDivisorSum(12496);
    int s5 = getDivisorSum(14288);
    int s6 = getDivisorSum(15472);
    int s7 = getDivisorSum(14536);
    int s8 = getDivisorSum(14264);
    */


    int minChainLen = 0;
    int minChainEl = MAX_NUM;
    static std::set <int> visitedNumbers;

    for (int n = 2; n <= MAX_NUM; n++)
    {
        //std::cout << n << std::endl;
        if (n % 10000 == 0)
        {
            std::cout << n << std::endl;
        }

        //if (n == 12496)
        //{
        //    std::cout << n << std::endl;
        //}
        if (sieve.isPrime(n))
            continue;
        int nextChainNum = 0;
        int chainLen = 0;
        int linkStart = n;

        visitedNumbers.clear();
        while (true)
        {
            nextChainNum = getDivisorSum(linkStart);
            //if (nextChainNum == 12496)
            //{
            //    std::cout << n << std::endl;
            //}

            if (nextChainNum > MAX_NUM)
                break;
            chainLen += 1;
            if (nextChainNum == n)
            {
                if (chainLen > minChainLen)
                {
                    minChainLen = chainLen;
                    minChainEl = n;
                }
                break;
            }
            else
            {
                if (visitedNumbers.count(nextChainNum) > 0)
                    break;
                visitedNumbers.insert(nextChainNum);
                linkStart = nextChainNum;
            }
        }
    }

    return minChainEl;
}


int main()
{
    time_t t1 = clock();
    int solution = solve();
    time_t t2 = clock();
    int ms = (int)(t2 - t1) * 1000 / CLOCKS_PER_SEC;

    std::cout << "solution: " << solution << std::endl << "duration: " << ms << " ms" << std::endl;
}
