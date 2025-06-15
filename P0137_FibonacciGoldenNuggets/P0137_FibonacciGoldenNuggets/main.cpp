
#include <iostream>
#include <chrono>

const __int64 FIBCOUNT = 500;	// not more to fit into __int64
double g_fib[FIBCOUNT + 1];
bool g_fibIsSquare[FIBCOUNT + 1];

void init()
{
	//inaialize g_fib with first 100 fibonacci numbers
	g_fib[1] = 1;
	g_fib[2] = 1;
	for (int i = 3; i <= FIBCOUNT; i++) {
		g_fib[i] = g_fib[i - 1] + g_fib[i - 2];
	}
	for (int i = 3; i <= FIBCOUNT; i++) {
		// find out if number is a square number
		__int64 root = (__int64)sqrt(g_fib[i]);
		if (root * root == g_fib[i])
			g_fibIsSquare[i] = true;
		else
			g_fibIsSquare[i] = false;
	}
	// print square fibonacci numbers
	for (int i = 1; i <= FIBCOUNT; i++) {
		if (g_fibIsSquare[i])
			printf("%lf\n", g_fib[i]);
	}
}

double calcSeries(double x)
{
	double xFactor = x;
	double sum = 0;
	for (int i = 1; i <= FIBCOUNT; i++) {
		sum += g_fib[i] * xFactor;
		xFactor *= x;
	}
	return sum;
}

void testValue(double x)
{
	double v = calcSeries(x);
	printf("x=%.12lf, v=%.12lf, 1/v=%.12lf, ", x, v, 1 / v);
	std::cout << abs(round(v) - v) << std::endl;
	if (abs(round(v) - v) < 1E-8)
		printf("v is integer\n");
	else
		printf("v is not integer\n");
}

__int64 solve()
{
	init();

	std::cout << sizeof(long double) << std::endl;
	std::cout << sizeof(double) << std::endl;

	testValue(0.5);
	testValue((sqrt(34) - 3) / 5);
	testValue(sqrt(2) - 1);
	testValue((sqrt(144) - 8) / 13);
	testValue(((double)1) / 3);
	testValue(((double)1) / 4);
	testValue(((double)1) / 5);
	testValue((double)0.21);
	testValue((double)0.18);
	testValue((double)0.191);
	testValue((double)0.1911);
	testValue((double)0.1925);
	testValue((sqrt(89) - 5) / 8);
	testValue((double)2 / 3);	// golden nugget !
	testValue((double)3/4);
	testValue((double)1);
	testValue((double)0.99);

	return 0;
}


int main()
{

	auto t1 = std::chrono::high_resolution_clock::now();
	__int64 solution = solve();
	auto t2 = std::chrono::high_resolution_clock::now();
	auto microSec = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
	std::cout << "solution: " << solution << std::endl << "duration: " << microSec << " micro seconds (" << ms << " ms)" << std::endl;
	if (microSec > 300'000'000)
	{
		std::cout << "(" << ((float)microSec) / 60'000'000 << " minutes )" << std::endl;
	}
}
