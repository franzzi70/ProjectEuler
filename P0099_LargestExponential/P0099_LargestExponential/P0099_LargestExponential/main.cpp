// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <iostream>
#include <chrono>
#include <string>
#include <fstream>
#include <list>


int solve()
{
    std::string fileName = "0099_base_exp.txt";
    std::ifstream f;    // (fileName);

    double highestValue = 0;
    int lineNoOfHighestValue = 0;

    f.open(fileName);
    int lineNo = 0;
    while (!f.eof())
    {
        std::string line;
        std::getline(f, line);
        if (line.length() > 0)
        {
            lineNo += 1;
            size_t commaPos = line.find_first_of(',');
            std::string baseStr = line.substr(0, commaPos);
            std::string expStr = line.substr(commaPos + 1, line.length() - commaPos - 1);
            int base = std::stoi(baseStr);
            int exp = std::stoi(expStr);
            double logNo = (double)exp * log((double)base);
            if (logNo > highestValue)
            {
				highestValue = logNo;
                lineNoOfHighestValue = lineNo;
			}

        }
    }
    f.close();

    return lineNoOfHighestValue;
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
