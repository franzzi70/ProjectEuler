
#include <iostream>
#include <chrono>
#include "primes.h"

// #define VERBOSE

static const int MAX_PRIME = 1000000;

primes::PrimesSieve g_primes(MAX_PRIME);


const double SQRT3OVER2 = sqrt(3) / 2;
const double MY_PI = acos(-1);

class Point {
public:
	Point() : x(0), y(0) {}
    Point(const Point& other) : x(other.x), y(other.y) {}
    Point(double x, double y) : x(x), y(y) {}
	double distance(const Point& p) const
	{
		double dx = p.x - this->x;
		double dy = p.y - this->y;
		double d = sqrt(dx * dx + dy * dy);
		return d;
	}

	static double angle(const Point& a, const Point& center, const Point& b)
	{
		double ax = a.x - center.x;
		double ay = a.y - center.y;
		double bx = b.x - center.x;
		double by = b.y - center.y;
		double dot = ax * bx + ay * by;
		double det = ax * by - ay * bx;
		double angle = atan2(det, dot);
		return angle;
	}

	Point& operator *= (double factor)
	{
		x *= factor;
		y *= factor;
		return *this;
	}

	double x;
    double y;
};


class Line {
public:
	Line() : p1(Point()), p2(Point()) {}
    Line(const Line& other) : p1(other.p1), p2(other.p2) {}
    Line(Point p1, Point p2) : p1(p1), p2(p2) {}
	Line(double x1, double y1, double x2, double y2) : p1(Point(x1, y1)), p2(Point(x2, y2)) {}
	double len() const
	{
		return p1.distance(p2);
	}
	Point mid() const
	{
		double x = (p1.x + p2.x) / 2;
		double y = (p1.y + p2.y) / 2;
		return Point(x, y);
	}
	void mid(Point& m)
	{
		m.x = (p1.x + p2.x) / 2;
		m.y = (p1.y + p2.y) / 2;
	}

	Point intersect(const Line& line) const
	{
		double x1 = p1.x;
		double y1 = p1.y;
		double x2 = p2.x;
		double y2 = p2.y;
		double x3 = line.p1.x;
		double y3 = line.p1.y;
		double x4 = line.p2.x;
		double y4 = line.p2.y;

		double x = ((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4)) /
			((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4));
		double y = ((x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4)) /
			((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4));

		return Point(x, y);
	}

	Line& operator *= (double factor)
	{
		p1 *= factor;
		p2 *= factor;
		return *this;
	}

	Point p1;
	Point p2;

};

class Triangle {
public:
	Triangle() : a(Line()), b(Line()), c(Line()) {}
	Triangle(const Triangle& other) : a(other.a), b(other.b), c(other.c) {}
	Triangle(const Line a, const Line b, const Line c) : a(a), b(b), c(c) {}
	Triangle(const Point& A, const Point& B, const Point& C) : a(Line(B, C)), b(Line(C, A)), c(Line(A, B)) {}
    Triangle(const double lenA, const double lenB, const double lenC)
    {
		c = Line(0, 0, lenC, 0);
		double cb = (lenC * lenC + lenB * lenB - lenA * lenA) / (2 * lenC);
		double hc = sqrt(lenB * lenB - cb * cb);
		a = Line(lenC, 0, cb, hc);
		b = Line(cb, hc, 0, 0);
    }
	const Point& A() const
	{
		return c.p1;
	}
	const Point& B() const
	{
		return a.p1;
	}
	const Point& C() const
	{
		return b.p1;
	}

	Triangle& operator *=(double factor)
	{
		a *= factor;
		b *= factor;
		c *= factor;
		return *this;
	}
		
	double inner_len() const
	{
		double len_c = c.len();
		Point P1(len_c / 2, -SQRT3OVER2 * len_c);
		return P1.distance(C());
	}

	Point fermat_point() const
	{
		double len_c = c.len();
		Point P1(len_c / 2, -SQRT3OVER2 * len_c);

#ifdef VERBOSE
		std::cout << "check triangle: " << std::endl;
		std::cout << P1.distance(c.p1) << std::endl;
		std::cout << P1.distance(c.p2) << std::endl;
		std::cout << c.len() << std::endl;
#endif


		Line l1 = Line(P1, b.p1);
		Point a_mid = a.mid();
		double xdiff = a.p2.x - a.p1.x;
		double ydiff = a.p2.y - a.p1.y;

		Point P2 = Point(a_mid.x + SQRT3OVER2 * ydiff, a_mid.y - SQRT3OVER2 * xdiff);
		Line l2(c.p1, P2);

		Point PL = l1.intersect(l2);
#ifdef VERBOSE
		std::cout << "PL: " << PL.x << ", " << PL.y << std::endl;
#endif
		//double distSum = PL.distance(a.p1) + PL.distance(b.p1) + PL.distance(c.p1);

#ifdef VERBOSE
		std::cout << PL.distance(a.p1) << std::endl;
		std::cout << PL.distance(b.p1) << std::endl;
		std::cout << PL.distance(c.p1) << std::endl;
		std::cout << distSum << std::endl;

		std::cout << "angles: " << std::endl;
		std::cout << Point::angle(a.p1, PL, b.p1) << " , in degrees: " << Point::angle(a.p1, PL, b.p1) * 180 / MY_PI << std::endl;
		std::cout << Point::angle(b.p1, PL, c.p1) << " , in degrees: " << Point::angle(b.p1, PL, c.p1) * 180 / MY_PI << std::endl;
		std::cout << Point::angle(c.p1, PL, a.p1) << " , in degrees: " << Point::angle(c.p1, PL, a.p1) * 180 / MY_PI << std::endl;
#endif

		return PL;
	}

	Line a;
	Line b;
	Line c;
};

bool check_integer(double d)
{
	double intpart;
	double fractpart = modf(d, &intpart);
	if (fractpart < 0.0000001)
	{
		return true;
	}
	return false;
}

void small_test_search()
{
	int count = 0;
	for (double c = 3; c <= 2000; c += 1)
	{
		for (double a = c ; a >= 1; a -= 1)
		{
			for (double b = a; b >= 1 ; b -= 1)
			{
				if (a + b == c)
					continue;

				if (c == 511 && a == 455 && b == 399)
				{
					std::cout << "found: " << a << ", " << b << ", " << c << std::endl;
				}
				Triangle t1(a, b, c);
				double fermat_len = t1.inner_len();
				if (check_integer(fermat_len))
				{
					//if (((int)c) % 2 == 0)
					//{
					//	if (((int)a) % 2 == 1 || ((int)b) % 2 == 1)
					//	std::cout << "even/odd found: " << a << ", " << b << ", " << c << std::endl;
					//}
					if (
						!g_primes.isPrime(c)
						&&
						g_primes.isPrime(a)
						&&
						g_primes.isPrime(b)
						)
						continue;
					count += 1;
					if (count %20 == 0)
					{
						// print counter
						std::cout << "count:" << count;
						// print quotiient of inner len over circumference
					}
					std::cout << "found: " << a << ", " << b << ", " << c;
					std::cout << ", inner len: " << fermat_len;
					std::cout << "; smallSumDiff: " << fermat_len - (a + b) << std::endl;
					//std::cout << "; quotient: " << fermat_len / (a + b + c) << std::endl;
				}
			}
		}
	}
}


void testPi()
{
	//const double MY_PI_ALT = asin(0.5) * 6;

	//std::cout << "PI: " << MY_PI << std::endl;
	//std::cout << "PI_ALT: " << MY_PI_ALT << std::endl;
	//// difference between the two PI values
	//std::cout << "PI diff: " << MY_PI - MY_PI_ALT << std::endl;

	//// compare sin(pi/6) with 0.5 using the two Pi values	
	//std::cout << "sin(pi/6): " << sin(MY_PI / 6) << std::endl;
	//std::cout << "sin(pi/6) - 0.5: " << sin(MY_PI / 6) - 0.5 << std::endl;
	//std::cout << "sin(pi/6) - 0.5: " << sin(MY_PI_ALT / 6) - 0.5 << std::endl;

	//// compare sin(pi/6) with 1 using the two Pi values	
	//std::cout << "sin(pi/2): " << sin(MY_PI / 2) << std::endl;
	//std::cout << "sin(pi/2) - 1: " << sin(MY_PI / 2) - 1 << std::endl;
	//std::cout << "sin(pi/2) - 1: " << sin(MY_PI_ALT / 2) - 1 << std::endl;

	//// compare sin(pi/6) with sqrt(2)/2 using the two Pi values	
	//std::cout << "sin(pi/4): " << sin(MY_PI / 4) << std::endl;
	//std::cout << "sin(pi/4) - sqrt(2)/2: " << sin(MY_PI / 4) - sqrt(2) / 2 << std::endl;
	//std::cout << "sin(pi/4) - sqrt(2)/2: " << sin(MY_PI_ALT / 4) - sqrt(2) / 2 << std::endl;
}

void playground()
{
	//testPi();
	small_test_search();

	//Triangle t1(399, 455, 511);
	//Triangle t1(511, 455, 399);
	Triangle t1(Triangle(511, 455, 399)*=2);
	//Triangle t1(399, 455, 516);
	//Triangle t1(5, 7, 8);
	//Triangle t1(Line(4, 0, 2, 2*sqrt(3)), Line(2, 2*sqrt(3), 0, 0), Line(0,0,4,0));
	//Triangle t1(Point(0,0), Point(4,0), Point(2,2*sqrt(3)));
	//Triangle t1(Point(0,0), Point(4,0), Point(0,3));
	//Triangle t1(Point(0, 0), Point(6, 0), Point(3, 4));

	std::cout << "inner len: " << t1.inner_len() << std::endl;

#ifdef VERBOSE
	std::cout << t1.a.len() << std::endl;
	std::cout << t1.b.len() << std::endl;
	std::cout << t1.c.len() << std::endl;
	std::cout << "distance points of A: " << t1.a.p1.distance(t1.a.p2) << std::endl;
	std::cout << "distance points of B: " << t1.b.p1.distance(t1.b.p2) << std::endl;
	std::cout << "distance points of C: " << t1.c.p1.distance(t1.c.p2) << std::endl;

	std::cout << SQRT3OVER2 << std::endl;
	std::cout << SQRT3OVER2 - sqrt(3) / 2 << std::endl;

	//double my_pi = asin(0.5) * 6;
	std::cout << MY_PI << std::endl;
	std::cout << SQRT3OVER2 - sin(2 * MY_PI / 3) << std::endl;
#endif

	// length of vertexes
	std::cout << "length of vertexes: " << t1.a.len() << ", " << t1.b.len() << ", " << t1.c.len() << std::endl;

	double sum = 0;
	Point P = t1.fermat_point();
	std::cout << "P: " << P.x << ", " << P.y << std::endl;
	double a_dist = P.distance(t1.A());
	double b_dist = P.distance(t1.B());
	double c_dist = P.distance(t1.C());

	std::cout << "distance to A: " << P.distance(t1.A()) << std::endl;
	std::cout << "distance to B: " << P.distance(t1.B()) << std::endl;
	std::cout << "distance to C: " << P.distance(t1.C()) << std::endl;

	std::cout << "sum: " << a_dist + b_dist + c_dist << std::endl;
	double angle_a = Point::angle(t1.B(), t1.A(), t1.C());
	double angle_a_neg = Point::angle(t1.C(), t1.A(), t1.B());
	double angle_b = Point::angle(t1.C(), t1.B(), t1.A());
	double angle_c = Point::angle(t1.A(), t1.C(), t1.B());

	std::cout << "triangle angles in degrees" << std::endl;
	std::cout << angle_a * 180 / MY_PI << std::endl;
	std::cout << angle_a_neg * 180 / MY_PI << std::endl;
	std::cout << angle_b * 180 / MY_PI << std::endl;
	std::cout << angle_c * 180 / MY_PI << std::endl;
	std::cout << " sum of angles: " << angle_a * 180 / MY_PI + angle_b * 180 / MY_PI + angle_c * 180 / MY_PI << std::endl;

	
	//std::cout << "angles: " << std::endl;
	//std::cout << Point::angle(t1.a.p1, P, t1.b.p1) << " , in degrees: " << Point::angle(t1.a.p1, P, t1.b.p1) * 180 / MY_PI << std::endl;
	//std::cout << Point::angle(t1.b.p1, P, t1.c.p1) << " , in degrees: " << Point::angle(t1.b.p1, P, t1.c.p1) * 180 / MY_PI << std::endl;
	//std::cout << Point::angle(t1.c.p1, P, t1.a.p1) << " , in degrees: " << Point::angle(t1.c.p1, P, t1.a.p1) * 180 / MY_PI << std::endl;


	//for (int i = 0; i < 100000000; i++)
	//{
	//	//t1.c.p2.x = i*0.000002;
	//	P = t1.fermat_point();
	//	sum += P.y;
	//}

	std::cout << "sum: " << sum << std::endl;

#ifdef VERBOSE
	Point A = t1.A();
	std::cout << "A: " << A.x << ", " << A.y << std::endl;
	Point B = t1.B();
	std::cout << "B: " << B.x << ", " << B.y << std::endl;
	Point C = t1.C();
	std::cout << "C: " << C.x << ", " << C.y << std::endl;
#endif

}

int solve()
{
    playground();
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
}
