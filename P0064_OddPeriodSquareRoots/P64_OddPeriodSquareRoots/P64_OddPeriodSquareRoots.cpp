// P64_OddPeriodSquareRoots.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

int ggt(int a, int b)
{
	int _a = a, _b = b;
	int r;
	do {
		r = _a % _b;
		if (r == 0)
		{
			// std::cout << "ggt(" << a << "," << b << ") = " << _b << std::endl;
			return _b;
		}

		_a = _b;
		_b = r;
	} while (true);

}

int kgv(int a, int b)
{
	return (a / ggt(a, b)) * b;
}

class term {
public:
	term(int sq) {
		_ax = (int)sqrt((double)sq);
		_sq = sq;
		_a = _ax;
		_q = sq - (_a * _a);
		_f = 1;
	}
	term(int ax, int sq, int a, int f, int q)
	{
		_ax = ax;
		_sq = sq;
		_a = a;
		_f = f;
		_q = q;
	}
	term next()
	{
		int n_ax = (int) floor((double)_f * (sqrt(_sq) + _a) / (double)_q);
		int aa = _a - _q * n_ax;
		int n_a = (-aa);
		int den = _sq - (aa * aa);
		int _ggt = ggt(_q, den);
		int f = _q / _ggt;
		int q = den / _ggt;
		return term(n_ax, _sq, n_a, f, q);
	}
	int getAx()
	{
		return _ax;
	}
	bool operator==(const term &other)
	{
		if (
			this->_ax == other._ax
			&&
			this->_a == other._a
			&&
			this->_f == other._f
			&&
			this->_q == other._q
			&&
			this->_sq == other._sq
			)
			return true;
		else
			return false;
	}
protected:
	int _ax;
	int _sq;
	int _a;
	int _f;
	int _q;
};

int periodCount(int n, bool verbose = false)
{

	int tst = sqrt(n);
	if (tst * tst == n)
		return 0;	// integers as root do not work.

	term t(n);
	term t1 = t.next();
	term tn = t1;
	int i = 1;

	if (verbose)
		std::cout << "sqrt(" << n << ") = [" << t.getAx() << "; (";

	while (true)
	{
		if (verbose)
			std::cout << tn.getAx();
		tn = tn.next();
		if (tn == t1)
		{
			break;
		}
		if (verbose)
			std::cout << ",";
		i += 1;
	}
	if (verbose)
		std::cout << ")], period = " << i << std::endl;
	return i;
}

void(step(int sq, int f, int q, int a))
{
	int ax = (int) floor((double)f * (sqrt(sq) + a) / (double)q);

}

void solve()
{
	int OddPeriodCount = 0;
	for (int i = 2; i <= 10000; i++)
	{
		int c = periodCount(i, false);
		if (c % 2 == 1)
			OddPeriodCount += 1;
	}
	std::cout << OddPeriodCount << " have odd period." << std::endl;
}

int main()
{

	clock_t t = clock();
	solve();
	printf("\nTime taken: %.4fs\n", (float)(clock() - t) / CLOCKS_PER_SEC);
}
