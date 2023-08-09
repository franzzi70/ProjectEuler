
#include <iostream>
#include <chrono>
#include <list>
#include <vector>
#include <map>
#include <algorithm>
#include <fstream>

#include "AnagramWordsProvider.h"


class AnagramInfo {
public:
    std::string orderedLetters;
    int wordCount;
};



bool isPalindrome(std::string& str1, std::string& str2)
{
    int len1 = (int)str1.length();
    int len2 = (int)str2.length();
    if (len1 != len2)
    {
        return false;
    }
    for (int i = 0; i < len1; i++)
    {
        if (str1[i] != str2[len2 - i - 1])
        {
            return false;
        }
    }
    return true;
}



std::list<std::pair<std::string, std::string>> pairList;
void parseWordsFile()
{
    
    IAnagramWordsProvider* awprov = AnagramWordsProviderFactory::createProvider();
    std::list<std::string> wordsList = awprov->getWordsList();

    std::map<std::string, std::list<std::string>> anagramInfoMap;


    for (auto it = wordsList.begin(); it != wordsList.end() ; it++)
    {
        std::string orderedString = (*it);
        std::sort(orderedString.begin(), orderedString.end());
        if (orderedString.compare("OPST") == 0)
        {
            std::cout << *it << std::endl;
        }
        if (anagramInfoMap.count( orderedString) == 0)
        {
            std::list<std::string> l;
            l.push_back(*it);
            anagramInfoMap.insert(std::pair<std::string, std::list<std::string>> {orderedString, l});
        }
        else
        {
            //if (orderedString.compare("OPST") == 0)
            //{
            //    std::cout << *it << std::endl;
            //}
            auto p = anagramInfoMap.find(orderedString);
            for (auto it2 = p->second.begin(); it2 != p->second.end(); it2++)
            {
                if (!isPalindrome(*it, *it2))
                {
                    pairList.push_back(std::pair<std::string, std::string>{*it, * it2 });
                }
            }
            p->second.push_back(*it);
        }

    }


}

__int64 getAnagramicSquare(std::string& str1, std::string& str2, __int64 n)
{
    char digitMap[10]{ 0,0,0,0,0,0,0,0,0,0 };
    char charMap[26]{
            -1,-1,-1,-1,-1,-1,-1,-1,
            -1,-1,-1,-1,-1,-1,-1,-1,
            -1,-1,-1,-1,-1,-1,-1,-1,
            -1,-1 };

   // if (str1.compare("RAISE") == 0 && (n * n) == 11025)
   // {
			//std::cout << str1 << std::endl;
   // }

    __int64 prod = n * n;
    __int64 rest = prod;
    int len = (int)str1.size();
    int ixstr1 = len-1;
    while (rest > 0)
    {
        int digit = rest % 10;
        char string1Char = str1[ixstr1];
        char currentDigitString = digitMap[digit];

        if (currentDigitString != 0)
        {
            if (currentDigitString != string1Char)
                return -1;
        }
        else
        {
            digitMap[digit] = string1Char;
            charMap[string1Char - 65] = digit;
        }

        rest /= 10;
        ixstr1 -= 1;
    }

    // build number with 2nd word and check if it is square
    __int64 num2 = 0;

    if (charMap[str2[0] - 65] == 0) // leading 0 digit not allowed
    {
        return -1;
    }

    for (int i = 0; i < len; i++)
    {
        __int64 digit2 = charMap[str2[i] - 65];
        num2 += (__int64) digit2;
        if (i < len - 1)
        {
            num2 *= 10;
        }

    }
    __int64 root2 = (__int64)round(sqrt(num2));
    if (root2 * root2 == num2)
    {
        //if (num2 > prod)
        //{
        //    std::cout << prod << "," << num2 << ", max: " << std::max(prod, num2) << std::endl;
        //}
        return std::max(prod, num2);
    }

    return -1;
}

int solvePair(std::string& str1, std::string& str2)
{
    int len = (int)str1.length();
    __int64 highExp10 = 1;
    for (int i = 0; i < len; i++)
    {
        highExp10 *= 10;
    }
    __int64 lowExp10 = highExp10 / 10;

    int highTestNum = (int)(sqrt(highExp10) +1);

    for (__int64 i = highTestNum; i >0 ;  i--)
    {
        __int64 prod = i * i;
        if (prod >= highExp10)
        {
            continue;
        }
        if (prod < lowExp10)
        {
            break;
        }

        __int64 sq = sq = getAnagramicSquare(str1, str2, i);
        if (sq != -1)
        {
            __int64 sq2 = getAnagramicSquare(str2, str1, i);
            if (sq2 > sq)
            {
                std::cout << str1 << "," << str2 << ": " << sq << "," << sq2 << ", max: " << std::max(sq, sq2) << std::endl;
            }
            else
            {
                std::cout << str1 << "," << str2 << ": " << sq << "," << sq2 << ", max: " << std::max(sq, sq2) << std::endl;
            }
            return std::max(sq, sq2);
        }
    }

    return 0;
}

void init()
{
    parseWordsFile();
}

int solve()
{
    init();

    // pairList contains all anagramic word pairs
    int highestValue = 0;

    for (auto it = pairList.begin(); it != pairList.end(); it++)
    {
        std::string str1 = it->first;
        std::string str2 = it->second;
        int val = solvePair(str1, str2);
        if (val > highestValue)
            highestValue = val;

        // std::cout << it->first << " - " << it->second << std::endl;
    }
    
    return highestValue;
}


int main()
{
    auto t1 = std::chrono::high_resolution_clock::now();
    __int64 solution = solve();
    auto t2 = std::chrono::high_resolution_clock::now();
    auto microSec = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
    std::cout << "solution: " << solution << std::endl << "duration: " << microSec << " micro seconds (" << ms << " ms)" << std::endl;
}
