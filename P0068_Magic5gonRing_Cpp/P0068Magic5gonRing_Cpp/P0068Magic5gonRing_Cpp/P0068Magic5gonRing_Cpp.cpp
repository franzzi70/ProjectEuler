#include <iostream>
#include <algorithm>
#include <list>
#include <string>

/*
		
       5
         0     6
	 4       1
   9    3   2   7
	      8

*/

int LinesMap[15] = {5,0,1,6,1,2,7,2,3,8,3,4,9,4,0};

struct solutionInfo {
	std::string string;
	int digitCount;
	std::string digitString;
};

void generateSolutions(int* arr, std::list<solutionInfo>& solutions)
{
	int linesValues[15];
	for (int i=0; i < 15; i++)
		linesValues[i] = arr[LinesMap[i]];
	// check if all lines have same sum:
	int LinesSum = 0;
	for (int i = 0; i < 3; i++)
		LinesSum += linesValues[i];
	for (int i = 1; i < 5; i++)
	{
		int LineSum = 0;
		for (int j = 0; j < 3; j++)
		{
			LineSum += linesValues[i * 3 + j];
		}
		if (LineSum != LinesSum)
			return;
	}
	// all lines have same sum, now order them
	int LowestNum = linesValues[0];
	int LowestLine = 0;
	for (int i = 1; i < 5; i++)
	{
		if (linesValues[i * 3] < LowestNum)
		{
			LowestNum = linesValues[i * 3];
			LowestLine = i;
		}
	}
	// now generate solution string and value
	std::string solutionString = "";
	std::string digitString = "";
	int digitCount =0;
	for (int i = 0; i < 5; i++)
	{
		int lineIx = (i + LowestLine) % 5;
		for (int j = 0; j < 3; j++)
		{
			int val = linesValues[lineIx * 3 + j];
			std::string stringVal = std::to_string(val);
			solutionString += stringVal;
			digitString += stringVal;
			if (j<2)
				solutionString += ",";
		}
		if (i < 4)
			solutionString += ";";
	}
	digitCount = digitString.length();

	// add solution if not found already
	for (std::list<solutionInfo>::iterator it = solutions.begin(); it != solutions.end(); it++)
	{
		if ((((solutionInfo)*it).string).compare(solutionString) == 0)
			return;
	}
	solutionInfo info = { solutionString, digitCount, digitString};
	solutions.insert (solutions.end(), info);
	return;
}

std::string getHighest16DigitString(std::list<solutionInfo> &solutions)
{
	std::string highestString = "";

	for (std::list<solutionInfo>::iterator it = solutions.begin(); it != solutions.end(); it++)
	{
		solutionInfo inf = (*it);
		if (inf.digitCount == 16)
		{
			if (highestString.compare("") == 0)
			{
				highestString = inf.digitString;
			}
			else
			{
				if (highestString.compare(inf.digitString) < 0)
				{
					highestString = inf.digitString;
				}
			}
		}
	}
	return highestString;
}

std::string solve()
{

	std::list<solutionInfo> solutions;

	int a[10];
	for (int i = 0; i < 10; i++)
		a[i] = i + 1;
	do {
		generateSolutions(a, solutions);
	} while (std::next_permutation(a, a + 10));

	std::string solution = getHighest16DigitString(solutions);

	return solution;
}

int main(int argc, char** argv)
{
	std::cout << "solution:" << solve() << std::endl;
	return 0;
}
