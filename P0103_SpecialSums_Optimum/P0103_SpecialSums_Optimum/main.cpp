// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <iostream>
#include <chrono>
#include <vector>
#include <set>
#include <sstream>
#include <algorithm>


static int g_SeqSumsCheck = 0;
static int g_MonotonyCheck = 0;
static int g_EqLenDiffCheck = 0;



class Sequence
{
public:

    int m_len;
    std::vector<int> m_sequence;


    Sequence(int len) : m_len(len) {
        m_sequence = std::vector<int>((size_t)len, 1);
        for (int i = 1; i < len; i++)
            m_sequence[i] = i + 1;
    }
    Sequence(std::vector<int>& sequence) : m_sequence(sequence), m_len((int)sequence.size()) {}
    Sequence(const Sequence& other) : m_sequence(other.m_sequence), m_len(other.m_len) {}
    bool checkSequenceSums()
    {
        return _checkSequenceSums(m_sequence, m_len);
	}

    bool checkMonotony()
    {
		return _checkMonotony(m_sequence, m_len);
	}

    bool checkEqualLenDifferentSums()
    {
		return _checkEqualLenDifferentSums(m_sequence, m_len);
	}

    bool checkAll()
    {
        return _checkAll(m_sequence, m_len);
    }



    static bool _checkSequenceSums(std::vector<int>& sequence, int len)
    {
        if (len < 3)
			return true;
        int lenLongMax = (len+1) / 2;

        int sumLong = sequence[0];
        int sumShort = 0;

        for (int lenLong = 2; lenLong <= lenLongMax; lenLong++)
        {
            sumLong += sequence[lenLong - 1];
            sumShort += sequence[len + 1 - lenLong];
            if (sumLong <= sumShort)
            {
                g_SeqSumsCheck += 1;
                return false;
            }
        }
        return true;
    }

    static bool _checkMonotony(std::vector<int>& sequence, int len)
    {
		if (len < 2)
			return true;
		for (int i = 1; i < len; i++)
            if (sequence[i - 1] >= sequence[i])
            {
                g_MonotonyCheck += 1;
                return false;
            }
        return true;
    }

    static bool _checkEqualLenDifferentSums(std::vector<int>& sequence, int len)
    {
        int maxLen = len / 2;
        std::set<int> sums;
        for (int l = 2; l <= maxLen; l++)
        {
            std::vector<bool> selection1(len,false);
            std::vector<bool>::iterator it11 = selection1.begin();
            std::vector<bool>::iterator it12 = selection1.end();

            for (int i=0;i<l;i++)
                selection1[len-i-1] = true;
            do {
                int sum = 0;
                for (int i = 0; i < len; i++)
					if (selection1[i])
						sum += sequence[i];
                if (sums.count(sum))
                {
                    g_EqLenDiffCheck += 1;
                    return false;
                }
                sums.insert(sum);
            }
            while (std::next_permutation(it11, it12));

        }
        return true;
    }

    static bool _checkAll(std::vector<int>& sequence, int len)
    {
		return _checkSequenceSums(sequence, len) && _checkMonotony(sequence, len) && _checkEqualLenDifferentSums(sequence, len);
	}
};

static bool _nextDistribution(std::vector<int>& distribution, int len, int value, int maxValue)
{
    if (len == 0)
        return false;

    if (value == 0)
		return false;

    if (len == 1)
    {
        if (value <= maxValue)
        {
            if (distribution[0] != value)
            {
                distribution[0] = value;
                return true;
            }
			else
				return false;
        }
        else
            return false;
    }

    int allocated = distribution[len - 1];


    if (_nextDistribution(distribution, len - 1, value - allocated, allocated))
    {
        return true;
    }
    else
    {
        if (allocated == 0)
            return false;

        int newAllocated = allocated - 1;
        if (newAllocated * len < value)
            return false;

        distribution[len - 1] = newAllocated;

        if (allocated > maxValue)
			return false;
    
        distribution[len - 1] = newAllocated;
        int rest = value - newAllocated;

        //if (maxValue * (len - 1) < rest)
        //    return false;

        for (int i = len - 2; i >= 0; i--)
        {
            if (rest > newAllocated)
            {
                distribution[i] = newAllocated;
                rest -= newAllocated;
            }
            else
            {
				distribution[i] = rest;
                rest = 0;
			}
        }
        if (rest != 0)
			return false;

        return true;
    }

}

static void _initDistribution(std::vector<int>& distribution, int len, int value)
{
    if (len < 7)
    {
        for (int i = 0; i < len - 2; i++)
            distribution[i] = 0;

        int halfValue = (value + 2) / 2;
        distribution[len - 1] = halfValue;
        distribution[len - 2] = value - halfValue;
    }
    else
    {
        int lastValue = (2 * value - len + 1) / (len + 1);
        distribution[len - 1] = lastValue;
        int rest = value - lastValue;
        for (int i = len - 2; i >= 0; i--)
        {
            if (rest > lastValue)
            {
				distribution[i] = lastValue;
				rest -= lastValue;
			}
            else
            {
				distribution[i] = rest;
				rest = 0;
			}

        }
	}
}

bool checkSequence(std::vector<int>& sequence)
{

	return true;
}

std::string solve()
{
    int vectorSize = 7;
    Sequence baseSequence(vectorSize);
    std::stringstream ss;

    int headroom = 227;
    bool found = false;
    while (!found)
    {
        std::vector<int> distribution(vectorSize, 0);
        _initDistribution(distribution, vectorSize, headroom);
        do {
            std::vector<int> probeVector(baseSequence.m_sequence);

            for (int i = 0; i < vectorSize; i++)
                probeVector[i] += distribution[i];

            if (Sequence::_checkAll(probeVector, vectorSize))
            {
                found = true;
                for (int i = 0; i < vectorSize; i++)
                    ss << probeVector[i];
                break;
            }

        } while (_nextDistribution(distribution, vectorSize, headroom, headroom));
        headroom += 1;
        std::cout << headroom << std::endl;
        std::cout << "g_SeqSumsCheck:" << g_SeqSumsCheck << " g_MonotonyCheck:" << g_MonotonyCheck << " g_EqLenDiffCheck:" << g_EqLenDiffCheck << std::endl;
        g_SeqSumsCheck = 0;
        g_MonotonyCheck = 0;
        g_EqLenDiffCheck = 0;
    }

    std::string solution = ss.str();
    return solution;
}

int main()
{

    auto t1 = std::chrono::high_resolution_clock::now();
    std::string solution = solve();
    auto t2 = std::chrono::high_resolution_clock::now();
    auto microSec = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
    std::cout << "solution: " << solution << std::endl << "duration: " << microSec << " micro seconds (" << ms << " ms)" << std::endl;
}
