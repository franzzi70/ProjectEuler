// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <iostream>
#include <chrono>
#include <string>
#include <vector>
#include <set>

class Move {
public:
    int value;
    std::string name;
};

class MoveList {
public:
    MoveList() : value(0), moveCount(0), name("") {}
    void add(Move& move) {
	    moves.push_back(move);
	    value += move.value;
	    name += move.name;
        moveCount += 1;
    }
    int value;
    int moveCount;
	std::vector<Move> moves;
    std::string name;
};

std::vector<Move> moves;
std::vector<MoveList> moveLists;

int solve()
{
	moves.push_back({ 25, "SB" });
    moves.push_back({ 50, "DB" });
    for (int i = 1; i <= 20; i++)
    {
        moves.push_back(Move{ i, "S" + std::to_string(i) });
        moves.push_back(Move{ i * 2, "D" + std::to_string(i) });
        moves.push_back(Move{ i * 3, "T" + std::to_string(i) });
    }

    for (auto it = moves.begin(); it != moves.end(); it++)
	{
        if (it->name[0] == 'D')
        {
            MoveList m;
            m.add(*it);
            moveLists.push_back(m);
        }
	}
 
    int moveListsSize = (int)moveLists.size();
    for (int i=0;i<moveListsSize;i++)
	{
        for (auto it2 = moves.begin(); it2 != moves.end(); it2++)
		{
            MoveList ml;
            ml.add(*it2);
            ml.add(moveLists[i].moves[0]);
            moveLists.push_back(ml);
		}
	}


    std::set <std::string> listNames;

    moveListsSize = (int)moveLists.size();
    for (int i=0;i<moveListsSize;i++)
    {
        if (moveLists[i].moveCount == 2)
        { 
            for (auto it2 = moves.begin(); it2 != moves.end(); it2++)
            {
                std::string probeString = it2->name + moveLists[i].moves[0].name + moveLists[i].moves[1].name;
                std::string probeString2 = moveLists[i].moves[0].name + it2->name + moveLists[i].moves[1].name;
                if (listNames.find(probeString) != listNames.end() || listNames.find(probeString2) != listNames.end())
					continue;

                MoveList ml;
                ml.add(*it2);
                ml.add(moveLists[i].moves[0]);
                ml.add(moveLists[i].moves[1]);
                moveLists.push_back(ml);
                listNames.insert(probeString);
            }
        }
    }

    // created all possible sequences.
    // now just count the relevant for the problem: points to check out < 100
    moveListsSize = (int)moveLists.size();
    int count = 0;
    for (int i = 0; i < moveListsSize; i++)
    {
        if (moveLists[i].value < 100)
		{
            count += 1;
		}
    }

    return count;
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
