#pragma once

#include <iostream>
#include <bitset>
#include "Field.h"

class Board
{
public:
	Board()
	{
		if (!groupsInitialized)
			InitGroups();
		emptyCount = 81;
	}

	Board(Board& b)
	{

		for (int i=0; i < 81; i++)
			fields[i].value = b.fields[i].value;
		emptyCount = b.emptyCount;
	}

	~Board() {};

	bool Solve(bool verbose = false)
	{
		return Solve(this, verbose);
	}

	bool Solve(Board* rootBoard, bool verbose = false)
	{
		while (emptyCount != 0)
		{
			//std::cout << "scanning:" << std::endl;

			int foundCount = ScanBoardDirect(true);
			if (foundCount == 0)
			{
				//std::cout << "No more numbers found with scanning." << std::endl;
				break;
			}
		}


		if (emptyCount == 0)
		{
			if (rootBoard != NULL)
			{
				if (rootBoard != this)
					rootBoard->SetBoard(*this);
			}
			if (verbose)
			{
				std::cout << "problem solved:" << std::endl;
				Print();
			}
			return true;
		}
		else
		{
			// find field with least possible numbers and try them recursively:
			std::bitset<9> options;
			int fieldIx = ScanBoardForLeastOptions(options);
			if (fieldIx == -1)	// no options found => not solvable.
			{
				return false;
			}
			for (int i = 0; i < 9; i++)
			{
				if (options.test(i))
				{
					int testNumber = i + 1;
					Board testBoard(*this);
					testBoard.SetValue(fieldIx / 9, fieldIx % 9, testNumber);
					bool solved = testBoard.Solve(rootBoard);
					if (solved)
					{
						return true;
					}
				}
			}
		}
		return false;
	}

	void ScanGroup(char (&group)[9], std::bitset<9>& result)
	{
		for (int i = 0; i < 9; i++)
		{
			char c = fields[group[i]].value;
			if (c != 0)
				result.reset(c-1);
		}
	}

	int getEmptyCount()
	{
		return emptyCount;
	}

	void ScanField(int line, int column, std::bitset<9> &result)
	{
		for (int i = 0; i < 9; i++)
			result.set(i);
		int ix = line * 9 + column;
		ScanGroup(squares[groupMembership[ix][0]], result);
		ScanGroup(lines[groupMembership[ix][1]], result);
		ScanGroup(columns[groupMembership[ix][2]], result);
	}

	int ScanBoardForLeastOptions(std::bitset<9>& options)
	{
		int foundPos = -1;
		size_t minOptionsCount = 10;
		std::bitset<9> foundResult;
		int foundCount = 0;

		for (int i = 0; i < 81; i++)
		{
			if (fields[i].IsEmpty())
			{
				std::bitset<9> result;
				ScanField(i/9, i%9, result);
				size_t optionCount = result.count();
				if (optionCount > 1)
				{
					if (optionCount < minOptionsCount)
					{
						foundCount += 1;
						foundPos = i;
						foundResult = result;
						minOptionsCount = optionCount;
					}
				}
			}
		}
		if (foundCount > 0)
		{
			options = foundResult;
		}
		return foundPos;
	}

	int ScanBoardDirect(bool setNumbers)
	{
		std::bitset<9> result;
		int foundCount = 0;

		for (int i=0;i<9;i++)
			for (int j = 0; j < 9; j++)
			{
				if (fields[i * 9 + j].IsEmpty())
				{
					ScanField(i, j, result);
					if (result.count() == 1)
					{
						foundCount += 1;
						int foundNumber = 0;
						for (int k = 0; k < 9; k++)
						{
							if (result.test(k))
							{
								foundNumber = k + 1;
								break;
							}
						}
						if (setNumbers)
						{
							this->SetValue(i, j, (char)foundNumber);
						}
						//std::cout << "line " << i + 1 << ", column " << j + 1 << " => value " << (char)(foundNumber + 48) << std::endl;
					}
				}
			}
		return foundCount;
	}

	void SetBoard(Board& other)
	{
		for (int i = 0; i < 9; i++)
			for (int j = 0; j < 9; j++)
			{
				fields[i * 9 + j].value = other.fields[i * 9 + j].value;
			}
	}

	void SetBoard(std::string &boardString)
	{
		int strIx = 0;
		for (int i=0;i<9;i++)
			for (int j = 0; j < 9; j++)
			{
				char boardStringChar = boardString[strIx];
				while (boardStringChar < 48 || boardStringChar > 48 + 9)
				{
					strIx += 1;
					boardStringChar = boardString[strIx];
				}
				if (strIx >= boardString.size())
				{
					throw new std::exception("invalid input string.");
				}
				SetValue(i, j, boardString[strIx] - 48);
				// fields[i * 9 + j].value = boardString[strIx] - 48;
				strIx++;
			}
	}

	void SetValue(int row, int column, char value)
	{
		fields[row * 9 + column].value = value;
		if (value != 0)
			emptyCount--;
	}

	int GetFieldValue(int i, int j)
	{
		return (int)fields[i * 9 + j].value;
	}

	void Print()
	{
		for (int i = 0; i < 9; i++)
		{
			if (i % 3 == 0 && i > 0)
			{
				std::cout << "---+---+---" << std::endl;
			}
			for (int j = 0; j < 9; j++)
			{
				if (j % 3 == 0 && j > 0)
					std::cout << "|";
				std::cout << (char)(fields[i * 9 + j].value + 48);

			}
			std::cout << std::endl;
		}
	}

private:
	Field fields[91];
	int emptyCount;
	static bool groupsInitialized;
	static char squares[9][9];
	static char lines[9][9];
	static char columns[9][9];
	static int groupMembership[81][3];

	void InitGroups()
	{
		for (int i = 0; i < 9; i++)
		{
			// init squares
			int startIxSquares = (i%3) * 3 + 27*(i/3);
			for (int j=0;j<3;j++)
				for (int k = 0; k < 3; k++)
				{
					squares[i][j * 3 + k] = startIxSquares + j * 9 + k;
				}

			// init lines and columns
			int startIxLines = i * 9;
			int startIxColumns = i;
			for (int j = 0; j < 9; j++)
			{
				lines[i][j] = startIxLines + j;
				columns[i][j] = startIxColumns + 9 * j;
			}

		}
		for (int i=0;i<9;i++)
			for (int j = 0; j < 9; j++)
			{
				//// squares
				//groupMembership[i * 9 + j][0] = j/3 + 3*(i/3);
				//// lines
				//groupMembership[i * 9 + j][1] = i;
				//// columns
				//groupMembership[i * 9 + j][2] = j;
				
				groupMembership[squares[i][j]][0] = i;
				groupMembership[lines[i][j]][1] = i;
				groupMembership[columns[i][j]][2] = i;

			}

		groupsInitialized = true;
	}
};

