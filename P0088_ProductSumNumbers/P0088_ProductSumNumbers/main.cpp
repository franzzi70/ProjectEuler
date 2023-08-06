#include <iostream>
#include <time.h>
#include <set>
#include <vector>
#include <list>
#include <set>
#include <algorithm>

#include "primes.h"
//#include "main.h"


#define MAX_K 12000
//#define MAX_K 1000
#define MIN_K 2

#define MAX_TEST_N 32000
#define FACTOR_VECSIZE 20

#define VERBOSE
#undef VERBOSE
#define MAXSEQLEN 10000

#define MAXPRIME 20000
#define PRIMEARRSIZE (MAXPRIME/2)


primes::PrimesSieve g_sieve(MAXPRIME);
int g_primes[PRIMEARRSIZE];
int g_primesCount = 0;


class NumberSequence;
class SequencesForNumber;

//  list of solutions:
std::vector <NumberSequence*> g_ProdSumArray(MAX_K+1);
std::vector <SequencesForNumber*> g_N_Array(MAX_TEST_N + 1);
int g_prodSumsAssignedCount = 0;

class NumberSequence {
public:
    std::vector<__int16> m_numbers;
    __int8 m_sequenceLen;
    __int16 m_product;
    __int16 m_sumProd_len;

    NumberSequence(const NumberSequence& source)
    {
        m_numbers = std::vector<__int16>(source.m_numbers);
        m_sequenceLen = source.m_sequenceLen;
        m_product = source.m_product;
        m_sumProd_len = source.m_sumProd_len;
    }

    NumberSequence(__int16 n1, __int16 n2)
    {
        m_numbers = std::vector<__int16>(2);
        m_numbers[0] = (__int16)n1;
        m_numbers[1] = (__int16)n2;
        m_sequenceLen = 2;
        m_product = (__int16(n1 * n2));
        m_sumProd_len = m_product + 2 - (n1 + n2);
    }
    
    NumberSequence(__int16 n)
    {
        m_numbers = std::vector<__int16>(1);
        m_numbers[0] = (__int16)n;
        m_sequenceLen = 1;
        m_product = (__int16(n));
        m_sumProd_len = m_product + 1 - (n);
    }
    NumberSequence(NumberSequence& source, __int16 n)
    {
        __int16 _sourceLen = source.m_sequenceLen;
        m_numbers = std::vector<__int16>(_sourceLen + 1);
        __int16 sum = 0;
        for (int i = 0; i < _sourceLen; i++)
        {
            __int16 _val = source.m_numbers[i];
            m_numbers[i] = _val;
            sum += _val;
        }
        m_numbers[_sourceLen] = n;
        m_sequenceLen = _sourceLen + 1;
        m_product = (__int16)source.m_product * n;
        m_sumProd_len = m_product + m_sequenceLen - (sum + n);
    }

    NumberSequence(NumberSequence& source, __int16 n, __int8 index)
    {
        m_numbers = std::vector<__int16>(source.m_numbers);
        __int16 _n_ix = m_numbers[index];
        m_numbers[index] *= n;
        m_sequenceLen = source.m_sequenceLen;
        m_product = source.m_product * n;
        m_sumProd_len = source.m_sumProd_len - source.m_product + m_product + _n_ix - m_numbers[index];
    }

    NumberSequence(std::vector<__int16> v, __int16 vectorLen)
    {
        m_numbers = std::vector<__int16>(vectorLen);
        for (int i = 0; i < vectorLen; i++)
        {
            m_numbers[i] = v[i];
        }
        m_sequenceLen = (__int8)vectorLen;
        __int16 _prod = 1;
        __int16 _sum = 0;
        for (int i = 0; i < vectorLen; i++)
        {
            _prod *= v[i];
            _sum += v[i];
        }
        m_product = _prod;
        m_sumProd_len = _prod + vectorLen - _sum;
    }

};

bool isPrime(int num)
{
    return g_sieve.isPrime(num);
}

class PrimeFactors {
public:
    __int16 m_number;
    __int8 m_factorCount;
    std::vector<__int16> m_factors;

    PrimeFactors(__int16 n)
    {
        m_number = n;
        m_factors = std::vector<__int16>(FACTOR_VECSIZE);
        calculateFactors(n);
    }
private:
    void calculateFactors(__int16 n)
    {
        int primeIx = 0;
        int factorCount = 0;
        int testNum = n;
        while (!isPrime(testNum))
        {
            while (testNum % g_primes[primeIx] != 0) {
                primeIx += 1;
            }
            int _prime = g_primes[primeIx];
            testNum /= _prime;
            m_factors[factorCount] = _prime;
            factorCount += 1;
        }
        m_factors[factorCount] = testNum;
        factorCount += 1;
        m_factorCount = factorCount;
    }
};



class SequencesForNumber {
public:
    __int16 m_number;
    std::list<NumberSequence> m_sequences;
    SequencesForNumber(__int16 N)
    {
        m_number = N;
        m_sequences = std::list<NumberSequence>();

        PrimeFactors f(N);
        int _factorCount = f.m_factorCount;
        if (_factorCount == 2)
        {
            NumberSequence seq1 = NumberSequence(f.m_factors[0], f.m_factors[1]);
            m_sequences.push_back(seq1);
            NumberSequence seq2 = NumberSequence(f.m_factors[0] * f.m_factors[1]);
            m_sequences.push_back(seq2);
        }
        if (_factorCount > 2)
        {
            if (f.m_factors[0] == f.m_factors[_factorCount - 1])
            {
                // create less redundant sequences for equal factors:
                
                NumberSequence seq1(f.m_number);
                m_sequences.push_back(seq1);

                std::vector<__int16> _templateVector(20);
                __int16 _factor = f.m_factors[0];
                __int16 _factorCount = f.m_factorCount;
                _templateVector[0] = 1;
                for (int i = 1; i <= _factorCount; i++)
                {
                    _templateVector[0] *= _factor;
                    generateEqualsSequences(_templateVector, 1, _factor, _factorCount - i, i);
                }

            }
            else
            {
                __int16 _factorLast = f.m_factors[_factorCount - 1];
                __int16 _factorRest = 1;
                for (int i = 0; i < _factorCount - 1; i++)
                {
                    _factorRest *= f.m_factors[i];
                }
                SequencesForNumber* _source = g_N_Array[_factorRest];

                for (auto it = g_N_Array[_factorRest]->m_sequences.begin()
                    ; it != g_N_Array[_factorRest]->m_sequences.end()
                    ; it++
                    )
                {
                    NumberSequence seq1(*it, _factorLast);
                    int _seqLen1 = seq1.m_sumProd_len;
                    m_sequences.push_back(seq1);

                    int _sequenceLen = it->m_sequenceLen;
                    for (int _ix = 0; _ix < _sequenceLen; _ix++)
                    {
                        NumberSequence seq2(*it, _factorLast, _ix);
                        int _seqLen2 = seq2.m_sumProd_len;
                        m_sequences.push_back(seq2);
                    }
                }
            }
        }
    }
private:
    void generateEqualsSequences(std::vector<__int16>& templateVector, int startIndex, __int16 factor, int factorCount, __int16 factorLimit)
    {
        //generateEqualsSequences(_templateVector, 1, _factor, _factorCount - i, i);
        if (factorCount < 0)
            return;

        if (factorCount == 0)
        {
            m_sequences.push_back( NumberSequence(templateVector, startIndex));
        }
        else
        {
            templateVector[startIndex] = 1;
            for (int i = 1; i <= factorLimit; i++)
            {
                templateVector[startIndex] *= factor;
                generateEqualsSequences(templateVector, startIndex + 1, factor, factorCount - i, i);
            }
        }
    }
};



bool probeSequence(NumberSequence& seq)
{
    //std::vector <NumberSequence*> g_ProdSumArray(MAX_K + 1);
    __int16 k = seq.m_sumProd_len;
    if (k < 2 || k > MAX_K)
        return false;

    NumberSequence* _currentSeq = g_ProdSumArray[k];

    if (_currentSeq == NULL)
    {
        g_ProdSumArray[k] = new NumberSequence(seq);
        g_prodSumsAssignedCount += 1;
        if (g_prodSumsAssignedCount >= MAX_K - 1)
        {
            return true;
            //return false;
        }
    }
    else
    {
        if (seq.m_product < _currentSeq->m_product)
        {
            delete _currentSeq;
            g_ProdSumArray[k] = new NumberSequence(seq);
        }
    }
    return false;
}


int solve()
{
    int _foundCount = 0;
    int _toFind = MAX_K - 2;

    for (__int16 N = 4; N < MAX_TEST_N; N++)
    {
        if (isPrime(N))
            continue;
#ifdef VERBOSE
        std::cout << N << std::endl;
#endif

        SequencesForNumber* seqN = new SequencesForNumber(N);
        g_N_Array[N] = seqN;
        
        for (auto it = seqN->m_sequences.begin()
            ; it != seqN->m_sequences.end()
            ; it++)
        {
            if (probeSequence(*it))
            {
                // FOUND ALL NUMBERS !!
                // now sum up sor ProjectEuler solution number:
                int sum = 0;
                std::set<__int16> countedNumbers;
                for (int i = 2; i <= MAX_K; i++)
                {
                    __int16 n = g_ProdSumArray[i]->m_product;
                    if (countedNumbers.count(n) ==0)
                    {
                        sum += n;
                        countedNumbers.insert(n);
                    }
                }
                return sum;
            }
        }
    }
    return 0;
}



void init()
{
    // init prime array:
    int _maxPrime = g_sieve.m_maxNumber;
    int _primeIx = 0;
    for (int i = 0; i <= _maxPrime; i++)
    {
        if (isPrime(i))
        {
            g_primes[_primeIx] = i;
            _primeIx += 1;
        }
    }
    g_primesCount = _primeIx;
}

int main()
{

    time_t t1 = clock();
    init();
    int solution = solve();
    time_t t2 = clock();
    int ms = (int)(t2 - t1) * 1000 / CLOCKS_PER_SEC;

    std::cout << "solution: " << solution << std::endl << "duration: " <<  ms << " ms" << std::endl;
}
