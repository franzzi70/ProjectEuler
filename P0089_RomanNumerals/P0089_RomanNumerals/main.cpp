// P0089_RomanNumerals.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <iostream>
#include <time.h>
#include <string>
#include <vector>

std::string g_RomanSymbols = "IVXLCDM";
std::vector<int> g_ValueTab{ 1,5,10,50,100,500,1000 };

extern std::vector<std::string>input_strings;

struct codeRec {
    std::string code;
    int value;
    int len;
};

std::vector<codeRec> codeTab{
    {"I", 1, 1},
    {"V", 5, 1},
    {"X", 10, 1},
    {"L", 50, 1},
    {"C", 100, 1},
    {"D", 500, 1},
    {"M", 1000, 1},
    {"IV", 4, 2},
    {"IX", 9, 2},
    {"XL", 40, 2},
    {"XC", 90, 2},
    {"CD", 400, 2},
    {"CM", 900, 2}
};

std::string buf("xx");
static const std::string shiftValues("IXC");

int getCodeValue(std::string &str)
{
    int tabSize = codeTab.size();
    for (int i = 0; i < tabSize; i++)
    {
        if (str.compare(codeTab[i].code) == 0)
        {
            return codeTab[i].value;
        }
    }
    return 0;
}

bool bufChar(char c)
{
    if (shiftValues.find(c) != std::string::npos)
    {
        buf[0] = c;
        return true;
    }
    else
    {
        buf[0] = 0;
        return false;
    }
}

int decode(char curChar, bool isLastChar)
{
    std::string bufCharString = "x";
    int returnVal = 0;
    if (buf[0] != 0)
    {
        buf[1] = curChar;
        int codeVal = getCodeValue(buf);
        if (codeVal != 0)
        {
            buf[0] = 0;
            buf[1] = 0;
            return codeVal;
        }
        else
        {
            bufCharString[0] = buf[0];
            returnVal += getCodeValue(bufCharString);
            buf[0] = 0;
            buf[1] = 0;
        }
    }
    if (!isLastChar)
    {
        if (!bufChar(curChar))
        {
            bufCharString[0] = curChar;
            returnVal += getCodeValue(bufCharString);
        }
    }
    else
    {
        bufCharString[0] = curChar;
        returnVal += getCodeValue(bufCharString);

    }
    return returnVal;
}

int parseRomanNumber(std::string& romanNumber)
{
    int len = (int)romanNumber.length();
    int parsedNumber = 0;
    char curChar;

    // init global state variables:
    buf[0] = 0;
    buf[1] = 0;

    int cursor = 0;
    while (cursor < len)
    {
        curChar = romanNumber[cursor];
        parsedNumber += decode(curChar, cursor >= len-1);
        cursor += 1;
    }
    return parsedNumber;
}

std::string generateRomanNumber(int n)
{
    std::string romanNumber("");
    int rest = n;
    int d;
    
    d = rest / 1000;
    romanNumber.append(d,'M');
    rest = rest % 1000;

    d = rest / 100;
    if (d>0)
    {
        if (d == 9)
            romanNumber.append("CM");
        else
            if (d == 4)
                romanNumber.append("CD");
            else
            {
                if (d >= 5)
                {
                    romanNumber.append(1, 'D');
                }
                romanNumber.append(d % 5, 'C');
            }
    }
    rest = rest % 100;

    d = rest / 10;
    if (d > 0)
    {
        if (d == 9)
            romanNumber.append("XC");
        else
            if (d == 4)
                romanNumber.append("XL");
            else
            {
                if (d >= 5)
                {
                    romanNumber.append(1, 'L');
                }
                romanNumber.append(d % 5, 'X');
            }
    }
    rest = rest % 10;

    d = rest;
    if (d > 0)
    {
        if (d == 9)
            romanNumber.append("IX");
        else
            if (d == 4)
                romanNumber.append("IV");
            else
            {
                if (d >= 5)
                {
                    romanNumber.append(1, 'V');
                }
                romanNumber.append(d % 5, 'I');
            }
    }

    return romanNumber;
}

int solve()
{
    int diffSum = 0;
    int numberCount = input_strings.size();
    for (int i = 0; i < numberCount; i++)
    {
        std::string input_number = input_strings[i];
        int n = parseRomanNumber(input_number);
        std::string rn = generateRomanNumber(n);
        diffSum += input_number.length() - rn.length();
    }
    return diffSum;
}

int main()
{
    time_t t1 = clock();
    int solution = solve();
    time_t t2 = clock();
    int ms = (int)(t2 - t1) * 1000 / CLOCKS_PER_SEC;

    std::cout << "solution: " << solution << std::endl << "duration: " << ms << " ms" << std::endl;
}
