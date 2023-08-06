// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <iostream>
#include <time.h>
#include <chrono>
#include <list>
#include "Board.h"
#include <fstream>
#include <string>

const std::string fileName = "projecteuler.net_project_resources_p096_sudoku.txt";

std::list<std::string> readFile()
{
    std::ifstream f(fileName);
    std::string line;
    std::list<std::string> frames;
    int frameLineNo = 0;
    std::string frame = "";

    while (std::getline(f, line))
    {
        if (line.substr(0, 4).compare("Grid") == 0)
        {
            if (frame.size() > 0)
                frames.push_back(frame);
            frame = "";
            frameLineNo = 0;
        }
        else
        {
            frame.append(line);
            if (frameLineNo < 8)
            {
                frame.append(1, ',');
            }
            frameLineNo += 1;
        }

        //std::istringstream iss(line);
        //int a, b;
        //if (!(iss >> a >> b)) { break; } // error

        // std::cout << line << std::endl;
        // process pair (a,b)
    }
    if (frame.size() > 0)
        frames.push_back(frame);
    f.close();

    //std::cout << "done." << std::endl;
    return frames;
}

int solve()
{

    std::list<std::string> f = readFile();

    auto tm_start = std::chrono::high_resolution_clock::now();

    int sum = 0;

    for (auto it = f.begin(); it != f.end(); it++)
    {
        std::string frame = *it;
        Board b;
        b.SetBoard(frame);
        b.Solve();
        int val = b.GetFieldValue(0, 0) * 100
            + b.GetFieldValue(0, 1) * 10
            + b.GetFieldValue(0, 2);
        sum += val;

    }
    //std::string TestBoardString = "006095200,000720010,040000973,074000090,030610000,500039807,180006000,007000120,069072040"; // easy
    //std::string TestBoardString = "076000000,500000004,804060009,610800000,200001300,000302500,058000190,000200006,900708000";  // medium
    //std::string TestBoardString = "400390000,203000008,000016900,000080090,900164500,070000004,007000430,080903001,010040002";  // medium
    //std::string TestBoardString = "270406000,084030070,000007800,000060000,005002960,030008502,003690008,902050001,000703605";  // medium
    //std::string TestBoardString = "020010080,006000030,400500601,070400000,000000010,008050902,000000300,800009000,009020506";  // evil
    //std::string TestBoardString = "500060000,004507090,030000800,800206900,000040060,002010000,005902070,000100009,400000005";  // evil

    //Board b;
    //b.SetBoard(TestBoardString);
    //auto tm_afterSetup = std::chrono::high_resolution_clock::now();
    //b.Solve();
    //auto tm_stop = std::chrono::high_resolution_clock::now();
    //auto setupMic = std::chrono::duration_cast<std::chrono::microseconds>(tm_afterSetup - tm_start).count();
    //auto solveMic = std::chrono::duration_cast<std::chrono::microseconds>(tm_stop - tm_afterSetup).count();
    //std::cout << "setup time: " << setupMic << " microSec, solution time: " << solveMic << " microSec." << std::endl;

    return sum;
}

int main()
{
    time_t t1 = clock();
    int solution = solve();
    time_t t2 = clock();
    int ms = (int)(t2 - t1) * 1000 / CLOCKS_PER_SEC;

    std::cout << "solution: " << solution << std::endl << "duration: " << ms << " ms" << std::endl;
}
