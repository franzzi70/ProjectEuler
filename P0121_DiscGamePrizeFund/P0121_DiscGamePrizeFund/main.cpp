
#include <iostream>
#include <chrono>
#include <vector>
#include <algorithm>

double getProbability(std::vector<int>& v, std::vector<double>& p)
{
	double prob = 1;
	int len  = (int)v.size();
	for (int i = 0; i < len; i++)
	{
		if (v[i]==0)
		{
			prob *= p[i];
		}
		else
		{
			prob *= (1 - p[i]);
		}
	}
	return prob;
}

double getProbability(int drawCount)
{
	int minBlue = (drawCount +2) / 2;
	std::vector<double> probs(drawCount, 0.0);
	for (int i = 0; i < drawCount; i++)
	{
		probs[i] = (double)(1/double(i+2));
	}

	double probSum = 0;
	for (int i = minBlue; i <= drawCount; i++)
	{
		std::vector<int> draws(drawCount, 1);
		for (int j = 0; j < i; ++j)
			draws[j] = 0;
		do {
			//for (auto i : draws)
			//	std::cout << i << " ";
			//std::cout << std::endl;
			double prob = getProbability(draws, probs);
			probSum += prob;
			//std::cout << prob << std::endl;
		} while (std::next_permutation(draws.begin(), draws.end()));

	}
	return probSum;
}

int solve()
{
	double prob = getProbability(15);
	double f = 1.0 / prob;
	int backupFund = ((int)f);

	//std::cout << prob << std::endl;
	//std::cout << f << std::endl;
	//std::cout << backupFund << std::endl;

    return 	backupFund;
}



int main()
{
    auto t1 = std::chrono::high_resolution_clock::now();
    __int64 solution = solve();
    auto t2 = std::chrono::high_resolution_clock::now();
    auto microSec = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
    std::cout << "solution: " << solution << std::endl << "duration: " << microSec << " micro seconds (" << ms << " ms)" << std::endl;
}
