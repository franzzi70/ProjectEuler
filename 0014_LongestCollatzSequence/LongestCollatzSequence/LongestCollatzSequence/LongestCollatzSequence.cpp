// LongestCollatzSequence.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

//#define ARRAYSIZE 1000000
#define ARRAYSIZE 1000000
//#define VERBOSE

class CollatzProblem
{
public:
	CollatzProblem() {
	}

	__int64 Solve()
	{
		__int64 HighestSequenceLen = 0;
		for (__int64 i = ARRAYSIZE -1; i > 0; i--)
		{
			if (InfMap[i].nextNum == 0)
			{

				__int64 SeqLen = RecursiveSolve(i);
				//if (SeqLen > InfMap[HighestFound].sequenceLen)
				if (SeqLen > HighestSequenceLen)
				{
					HighestFound = i;
					HighestSequenceLen = SeqLen;
#ifdef VERBOSE
					// printf("\nHighestFound: starting with %d , length: %d\n", i, SeqLen);
					printf("\nHighestFound: starting with %d , length: %d\n", HighestFound, InfMap[HighestFound].sequenceLen);
#endif
				}

			}
		}
		
		printf("\nHighestFound: starting with %d , length: %d\n", (int)HighestFound, (int)(InfMap[HighestFound].sequenceLen));
		return HighestFound;
	}

private:
	class cInf {
	public:
		cInf() {}
		__int64 nextNum = 0;
		__int64 sequenceLen = 0;
	};

	
	__int64 RecursiveSolve(__int64 index)
	{
#ifdef VERBOSE
		printf("%d ", index);
#endif
		__int64 nextIndex;
		// termination:
		if (index == 1)
		{
			return 1;
		}

		// determine next index:
		if (index % 2 == 0)
			nextIndex = index / 2;
		else
			nextIndex = index * 3 + 1;

		__int64 SeqLen;
		//__int64 SeqLen = RecursiveSolve(nextIndex);

		if (index >= ARRAYSIZE)
		{
			SeqLen = RecursiveSolve(nextIndex) + 1;
			return SeqLen;
		}
		else {
			cInf entry = InfMap[index];
			if (entry.nextNum != 0)
			{
#ifdef VERBOSE
				printf("\nCHACHE HIT\n");
#endif
				return entry.sequenceLen;
			}
			else
			{
				SeqLen = RecursiveSolve(nextIndex) + 1;
				InfMap[index].nextNum = nextIndex;
				InfMap[index].sequenceLen = SeqLen;
				
				return SeqLen;
			}
		}
	}

	cInf *InfMap = new cInf[ARRAYSIZE * 2];
	__int64 HighestFound = 0;
};




int main()
{

	CollatzProblem cp;
	cp.Solve();

	return 0;
}

