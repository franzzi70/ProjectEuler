// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <iostream>
#include <chrono>
#include <vector>
#include <string>
#include <algorithm>
#include <set>

extern std::vector<std::string> data;

static int g_SeqSumsCheck = 0;
static int g_MonotonyCheck = 0;
static int g_EqLenDiffCheck = 0; 

bool _checkSequenceSums(std::vector<int>& sequence, int len)
{
    if (len < 3)
        return true;
    int lenLongMax = (len + 1) / 2;

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

bool _checkMonotony(std::vector<int>& sequence, int len)
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

bool _checkEqualLenDifferentSums(std::vector<int>& sequence, int len)
{
    int maxLen = len / 2;
    std::set<int> sums;
    for (int l = 2; l <= maxLen; l++)
    {
        std::vector<bool> selection1(len, false);
        std::vector<bool>::iterator it11 = selection1.begin();
        std::vector<bool>::iterator it12 = selection1.end();

        for (int i = 0; i < l; i++)
            selection1[len - i - 1] = true;
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
        } while (std::next_permutation(it11, it12));

    }
    return true;
}

bool _checkAll(std::vector<int>& sequence, int len)
{
    return _checkSequenceSums(sequence, len) && _checkMonotony(sequence, len) && _checkEqualLenDifferentSums(sequence, len);
}

std::vector<int> parseLine(std::string str)
{
    std::vector<int> resultSequence;

    int len = str.length();

    int start = 0;
    do {
        int end = str.find_first_of(',', start);
        if (end == std::string::npos)
        {
            if (start < str.size())
            {
				std::string number = str.substr(start, str.size() - start);
				resultSequence.push_back(std::stoi(number));
			}
            break;
		}
		std::string number = str.substr(start, end - start);
		resultSequence.push_back(std::stoi(number));
		start = end + 1;
	} while (true);
    
    std::sort(resultSequence.begin(), resultSequence.end());

	return resultSequence;
}

int solve()
{
    int sum = 0;

    for (int i = 0; i < data.size(); i++)
    {
        std::vector<int> l = parseLine(data[i]);
        if (_checkAll(l, l.size()))
        {
            for (int j=0; j < l.size(); j++)
			    sum += l[j];
		}
	}

    return sum;
}

int main()
{

    auto t1 = std::chrono::high_resolution_clock::now();
    int solution = solve();
    auto t2 = std::chrono::high_resolution_clock::now();
    auto microSec = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
    std::cout << "solution: " << solution << std::endl << "duration: " << microSec << " micro seconds (" << ms << " ms)" << std::endl;
}
