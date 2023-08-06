#include <iostream>
#include <time.h>
#include <array>
#include <stack>
#include <vector>

typedef struct {
    int rest;
    int groupSize;
    } paramFrame;
std::stack<paramFrame> paramStack;


const int SEARCHLIMIT = 10000;
std::array<int64_t, (SEARCHLIMIT+1) * (SEARCHLIMIT+1)> memArray;
int64_t callCount = 0;
int64_t countTerms(int n)
{
    int64_t count = 0;
    paramStack.push(paramFrame{n,n});
    while (!paramStack.empty())
    {
            
        paramFrame currentParams = paramStack.top();
        int rest = currentParams.rest;
        int GroupSize = currentParams.groupSize;
        paramStack.pop();
        bool allSuccessorsHaveMemValue = true;
        int64_t sucessorSum = 0;

        int memIx = (GroupSize-1) * (SEARCHLIMIT+1)+ rest;
        int64_t memVal = memArray[memIx];
        if (memVal != 0)
            count += memVal;
        else
        {
            if (GroupSize == 1)
            {
                count += 1;
                memArray[memIx] = 1;
                continue;
            }

            for (int i = rest; i >= 0; i -= GroupSize)
            {
                int64_t succVal = memArray[(GroupSize - 2) * (SEARCHLIMIT + 1) + i];
                if (succVal == 0)
                {
                    allSuccessorsHaveMemValue = false;
                    break;
                }
                sucessorSum += succVal;
            }
            if (allSuccessorsHaveMemValue)
            {
                memArray[memIx] = sucessorSum;
                count += sucessorSum;
            }
            else
            {
                for (int i = rest; i >= 0; i -= GroupSize)
                {
                    paramStack.push(paramFrame{ i, GroupSize - 1});
                }
            }
        }

    }
    callCount += 1;

    return count;
}

int64_t check(int n)
{
    int64_t TermCount = countTerms(n);
    return TermCount;
}

void init()
{
    memArray.fill(0);
}

void test()
{
    int64_t solutionCount = check(40);
    std::cout << callCount << std::endl;
    return;
}



const int MAXNUM = 100000;
std::vector<int> memVec;

int GetIndex(int n)
{
    // using Eulers ruler shown in https://en.wikipedia.org/wiki/Partition_function_(number_theory)
    // calculated (and simplified terms) the jumpOffsets for even and odd cases, maybe more elegant way possible.
    int jumpSize;
    if (n % 2 == 0)
    {
        jumpSize = n * (3 * n + 2) / 8;
    }
    else
    {
        jumpSize = (n * (3 * n + 4) + 1) / 8;
    }
    return jumpSize;
}

static int PModE6(int n)
{
    if (n < 2)
    {
        int val = 1;
        if (memVec.size() == n)
            memVec.push_back (val);
        return val;
    }

    if (memVec.size() > n)
    {
        return memVec[n];
    }

    int sum = 0;
    int ix = 0;

    for (int i = 1; (ix = GetIndex(i)) <= n; i += 1)
    {
        int p = PModE6(n - ix) % 1000000;
        if ((i - 1) % 4 < 2)
        {
            sum += p;
        }
        else
        {
            sum -= p;
        }
    }

    sum = sum % 1000000;

    if (memVec.size() == n)
        memVec.push_back(sum);    // modulo 1000000 can be used !

    return sum;
}


int solve()
{
    init();

    int TermCount;
    int solution = 0;
    // test();

    for (int i = 1; i < 100000; i++)
    {
        int p = PModE6(i);
        if (p % 1000000 == 0)
        {
            std::cout << p << std::endl;;
            return i;
        }
        if (i % 10000 == 0)
            std::cout << i << std::endl;
    }
    return 0;


    return solution;
}

int main()
{
    time_t t1 = clock();
    int solution = solve();
    time_t t2 = clock();
    int ms = (int)(t2 - t1) * 1000 / CLOCKS_PER_SEC;

    std::cout << "solution: " << solution << std::endl << "duration: " <<  ms << " ms" << std::endl;
}
