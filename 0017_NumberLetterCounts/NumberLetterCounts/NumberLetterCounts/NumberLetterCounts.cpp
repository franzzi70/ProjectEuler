// NumberLetterCounts.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string.h>
#include <string>

const char* digits[20] = {
		"", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine",
		"ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen", "seventeen", "eighteen", "nineteen"};

const char* tens[10] = {"", "ten", "twenty", "thirty", "forty", "fifty", "sixty", "seventy", "eighty", "ninety" };

const char* hundred = "hundred";
const char* thousand = "onethousand";

void PrintStrings()
{

	int letterCount = 0;
	for (int num = 1; num <= 999; num++)
	{
		std::string strNumber = "";

		if (num >= 100)
		{
			strNumber.append(digits[num / 100]);
			strNumber.append(hundred);
			if ((num % 100) != 0)
				strNumber.append("and");
		}

		int numIn100 = num % 100;
		if (numIn100 < 20 && numIn100 > 0)
		{
			strNumber.append(digits[numIn100]);
		}
		if (numIn100 >= 20)
		{
			strNumber.append(tens[numIn100 / 10]);
			strNumber.append(digits[num % 10]);
		}

		printf("%s (%d)\n", strNumber.c_str(), strNumber.length());
		letterCount += strNumber.length();
	}
	printf (thousand);
	letterCount += strlen(thousand);

	printf("number of letter in numbers from 1 to 1000 (from strings): %d\n", letterCount);

	printf("\n");

}



int main()
{

	//printf("number of letter in numbers from 1 to 1000: %d\n", CountLetters());

	printf("strings of numbers from 1 to 1000:\n");
	PrintStrings();

	return 0;
}

