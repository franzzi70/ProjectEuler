#include <iostream>
#include <fstream>
#include <time.h>
#include <vector>
#include <map>
#include <algorithm>
#include <sstream>

bool readFile(std::vector<std::string>& logCodes)
{
    const std::string fileName = "../../p079_keylog.txt";
    std::fstream inputFile;
    inputFile.open(fileName, std::fstream::in);
    if (!inputFile.is_open())
    {
        std::cout << "file not opened." << std::endl;
        return false;
    }
    char str[10];
    
    do
    {
        inputFile.getline(str, 10);
        if (inputFile.good())
            logCodes.push_back(std::string(str));
    } while (inputFile.good());
    (void)inputFile.close();
    return true;
}

void createUniqueList(std::vector<std::string>& logCodes, std::vector<std::string>& uniqueCodes)
{
    for (unsigned int i = 0; i < logCodes.size(); i++)
    {
        std::string str = logCodes[i];
        bool found = false;
        for (unsigned int j = 0; j < uniqueCodes.size(); j++)
        {
            if (str.compare(uniqueCodes[j]) == 0)
            {
                found = true;
                break;
            }
        }
        if (!found)
            uniqueCodes.push_back(str);
    }
}

void createOrderMap(std::vector<std::string>& uniqueCodes, std::map<std::string, std::string>& orderMap)
{
    for (unsigned int i = 0; i < uniqueCodes.size(); i++)
    {
        for (int j = 0; j < 3; j++)
        {
            std::string mapStr("  ");
            std::string strCode = uniqueCodes[i];

            switch (j) {
            case 0:
                mapStr[0] = strCode[0];
                mapStr[1] = strCode[1];
                break;
            case 1:
                mapStr[0] = strCode[0];
                mapStr[1] = strCode[2];
                break;
            case 2:
                mapStr[0] = strCode[1];
                mapStr[1] = strCode[2];
                break;
            default:
                break;
            }
            std::string reverseKey(mapStr);
            std::reverse(reverseKey.begin(), reverseKey.end());
            if (orderMap.count(mapStr) == 0)
            {
                auto inverseIt = orderMap.find(reverseKey);
                //if (inverseIt != orderMap.end())
                //{
                //    std::cout << reverseKey << " found." << std::endl;
                //}

                if (orderMap.count(reverseKey) != 0)
                {
                    std::cout << reverseKey << " found." << std::endl;
                }

                //if (orderMap.count(std::reverse(mapStr.begin(), mapStr.end()) != 0)
                //{

                //}
                if (mapStr.length() < 2)
                {
                    return;
                }
                orderMap.insert({ mapStr, mapStr });
            }
        }
    }
}

std::string createStartSequence(std::vector<std::string>& uniqueCodes)
{
    std::string digitString = "";
    for (unsigned int i = 0; i < uniqueCodes.size(); i++)
    {
        std::string str = uniqueCodes[i];
        for (unsigned int j = 0; j < str.length(); j++)
        {
            char c = str[j];
            if (std::string::npos == digitString.find(c))
            {
                digitString += c;
            }
        }
    }
    return digitString;
}

void reorder(std::string& digitSequence, std::map<std::string, std::string>& orderMap)
{
    bool ordered;

    while (true)
    {
        ordered = true;

        for (auto it = orderMap.begin(); it != orderMap.end(); it++)
        {
            std::string strOrder = it->first;
            char c1 = strOrder[0];
            char c2 = strOrder[1];
            int ix1 = digitSequence.find(c1);
            int ix2 = digitSequence.find(c2);
            if (ix1 > ix2)
            {
                ordered = false;
                digitSequence[ix1] = c2;
                digitSequence[ix2] = c1;
            }
        }
        if (ordered)
            return;
    }
}

int solve()
{
    std::vector<std::string> logCodes;
    std::vector<std::string> uniqueCodes;
    std::map<std::string,std::string> orderMap;
    if (!readFile(logCodes))
        return 0;
    createUniqueList(logCodes, uniqueCodes);
    createOrderMap(uniqueCodes, orderMap);
    std::string strSequence = createStartSequence(uniqueCodes);
    reorder(strSequence, orderMap);
    int solution;
    std::stringstream(strSequence) >> solution;
    
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
