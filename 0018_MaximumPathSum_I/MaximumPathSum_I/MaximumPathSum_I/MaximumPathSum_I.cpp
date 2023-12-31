// MaximumPathSum_I.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>

const char *raw_input = "\
75 \
95 64 \
17 47 82 \
18 35 87 10 \
20 04 82 47 65 \
19 01 23 75 03 34 \
88 02 77 73 07 63 67 \
99 65 04 28 06 16 70 92 \
41 41 26 56 83 40 80 70 33 \
41 48 72 33 47 32 37 16 94 29 \
53 71 44 65 25 43 91 52 97 51 14 \
70 11 33 28 77 73 17 78 39 68 17 57 \
91 71 52 38 17 14 91 43 58 50 27 29 48 \
63 66 04 68 89 53 67 30 73 16 69 87 40 31 \
04 62 98 27 23 09 70 98 73 93 38 53 60 04 23";


class Node {
public:
	Node()
	{
		m_value = 0;
		m_weight = 0;
	}
	Node(int value, int weight) {
		m_value = value;
		m_weight = weight;
	}
	int m_value;
	int m_weight;
};

Node nodeArr[15 * 15];

void Solve()
{
	std::string str(raw_input);
	int rowPos = 14;
	int rowNum = 14;
	for (int i = str.size() / 3; i >= 0; i--)
	{

		int num = atoi(str.substr(i * 3, 2).c_str());
		int weight;
		if (rowNum == 14)
			weight = num;
		else
		{
			Node a = nodeArr[(rowNum + 1) * 15 + rowPos];
			Node b = nodeArr[(rowNum + 1) * 15 + rowPos + 1];
			if (a.m_weight > b.m_weight)
				weight = num + a.m_weight;
			else
				weight = num + b.m_weight;
		}
		nodeArr[rowNum * 15 + rowPos] = Node(num, weight);
		rowPos -= 1;
		if (rowPos < 0)
		{
			rowNum -= 1;
			rowPos = rowNum;
		}
		if (i == 0)
			printf("\n%d\n", weight);
	}

}


int main()
{
	Solve();

    return 0;
}

