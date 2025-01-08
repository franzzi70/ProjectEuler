
#include <iostream>
#include <chrono>
#include <vector>
#include "main.h"

// #define VERBOSE


// white cell:
//	ellipse: 4x^2 + y^2 = 100
//  opening on top (-0.01 <= x <= 0.01)
// 
// laser beam: y = mx + b
//	initial beam: (0,10.1) -> (1.4,-9.6)

// dx = 1.4
// dy = -9.6 - 10.1 = -19.7
// eq: y = (-19.7/1.4) * x + 10.1

// a^2 * x^2 + b^2 * y^2 = a^2 * b^2
// 25 * x^2 + 100 * y^2 = 2500
// x^2 + 4 * y^2 = 100

// 25 * k^2 + 100 = d^2

//y = kx+d
//d = y-kx
//d = y-(p/q)x
// q*d = q*y - p*x


typedef std::pair<double, double> point;
typedef std::pair<point, point> segment;
typedef std::pair<segment, point> bounce_result;

//class point {
//public:
//	double first;
//	double second;
//};
//
//class segment {
//public:
//	segment(point p1, point p2) : first(p1), second(p2) {}
//	point first;
//	point second;
//};


point mirror(point Q, segment S)
{
	double Q0 = Q.first;
	double Q1 = Q.second;

	double P0 = S.first.first;
	double P1 = S.first.second;

	double R0 = S.second.first;
	double R1 = S.second.second;

	double vx = R0 - P0;
	double vy = R1 - P1;

	double x = P0 - Q0;
	double y = P1 - Q1;

	double r = 1 / (vx * vx + vy * vy);
	return {
		Q0 + 2 * (x - x * vx * vx * r - y * vx * vy * r),
		Q1 + 2 * (y - y * vy * vy * r - x * vx * vy * r)
	};
}

point rotatePoint(point& p, double angle)
{
	double x = p.first;
	double y = p.second;
	double x_ = x * cos(angle) - y * sin(angle);
	double y_ = x * sin(angle) + y * cos(angle);
	return { x_, y_ };
}

double orthogonalSlope(double slope)
{
	return -1 / slope;
}

point getFarPoint(const segment& s, const point& p)
{
	double dx1 = p.first - s.first.first;
	double dy1 = p.second - s.first.second;
	double D1S = dx1 * dx1 + dy1 * dy1;

	double dx2 = p.first - s.second.first;
	double dy2 = p.second - s.second.second;
	double D2S = dx2 * dx2 + dy2 * dy2;

	if (D1S > D2S)
		return s.first;
	return s.second;
}


class line {
public:
	line(double a, double b, double c) : A(a), B(b), C(c) { normalize(); }
	line(const segment& seg)
	{
		double x1 = seg.first.first;
		double y1 = seg.first.second;
		double x2 = seg.second.first;
		double y2 = seg.second.second;
		A = y2 - y1;
		B = x1 - x2;
		C = -A * x1 - B * y1;
		normalize();
	}
	line(double k, double d) // y = kx + d
	{
		A = -k;
		B = 1;
		C = -d;
		normalize();
	}
	point mirror(const point& p)
	{
		double D = A_ * p.first + B_ * p.second + C_;
		return {
			p.first - 2 * A_ * D,
			p.second - 2 * B_ * D
		};
	}

	void normalize()
		// normalize: A^2 + B^2 = 1
	{
		double len = sqrt(A * A + B * B);
		A_ = A / len;
		B_ = B / len;
		C_ = C / len;

	}

	double getYSlope()
	{
		return (-1) * A_ / B_;
	}

	double getXSlope()
	{
		return (-1) * B_ / A_;
	}

	point getNormalVector()
	{
		return { A_, B_ };
	}

	// normal form of line: Ax + By + C = 0
	double A;
	double B;
	double C;
	double A_;
	double B_;
	double C_;
};


class Ellipse
{
public:
	Ellipse(double a, double b) : a(a), b(b) {}
	segment intersectBeam(const segment& s)
	{
		double x1 = s.first.first;
		double y1 = s.first.second;
		double x2 = s.second.first;
		double y2 = s.second.second;
		double dx = x2 - x1;
		double dy = y2 - y1;
		double k = dy / dx;
		double A = a * a;
		double B = b * b;
		double K = k * k;
		double d = y1 - x1 * k;
		double D = d * d;

		double x1_ = ((-2) * sqrt(B * K + A - D) - d * k) / (K + 4);
		double x2_ = (2 * sqrt(B * K + A - D) - d * k) / (K + 4);
		double y1_ = 2 * (2 * d - k * sqrt(A + B * K - D)) / (K + 4);
		double y2_ = 2 * (k * sqrt(A + B * K - D) + 2 * d) / (K + 4);

		return { {x1_, y1_}, {x2_, y2_} };
	}

	double tangent(const point& p)
	{
		return (-1) * (a * a / (b * b)) * p.first / p.second;
	}

	segment orthLine(const point& p)
	{
		return {
			p,
			{p.first + 1, p.second + orthogonalSlope(tangent(p))}
		};
	}

	bounce_result bounce(const segment& s, const point& bouncePoint)
	{
		segment orthSeg = orthLine(bouncePoint);
		point mirrorPoint = getFarPoint(s, bouncePoint);
		// mirror first point on orth to surface:
		point pmirr1 = mirror(mirrorPoint, orthSeg);
		// find new mirror intersection point:
		segment i = intersectBeam({ bouncePoint, pmirr1 });
		point far_point = getFarPoint(i, bouncePoint);
		return { i, far_point };
	}

	double a;
	double b;
};


void test()
{
	Ellipse e(10, 5);

	line l1(segment{ {-4.0 / 3,0}, {0,4} });
	line l2(3, 4);

	// point p1 = l2.mirror({ 1, 1 });
	//point p2 = l2.mirror({ 0, 0 });
	//point p2m = l2.mirror({ 0, 1 });

	//point p3 = mirror({ 0,0 }, { {-4.0 / 3,0}, {0,4} });
	//point p4m = mirror({ 0,1 }, { {-4.0 / 3,0}, {0,4} });

	//double s1 = orthogonalSlope(0.5);
	//double s2 = orthogonalSlope(0.1);

	//std::vector<double> testHeights{ 0.5,1,9.5,9.9,9.99 /* , 11 */ };

	//bounce_result b1 = e.bounce(s2, getFarPoint(s2, start_point));
	//bounce_result b2 = e.bounce(b1.first, b1.second);



	//segment s3 = { { 1, 10.1 },{ 1.4, -9.6 } };
	//segment s4 = e.intersectBeam(s3);

	//for (auto h : testHeights)
	//{
	//	segment s5 = { { -100, h },{ 100, h } };
	//	segment s6 = e.intersectBeam(s5);

	//	double t61 = e.tangent(s6.first);
	//	double t62 = e.tangent(s6.second);

	//	continue;
	//}

	return;
}

int64_t solve()
{

	Ellipse e(10, 5);

	point start_point{ 0, 10.1 };
	point p_init{ 1.4, -9.6 };
	segment s_init = { start_point ,p_init };
	segment s2 = e.intersectBeam(s_init);

	segment bounce_seg = s2;
	point bounce_point = getFarPoint(s2, start_point);

	int hit_count = 0;
	do
	{
		// print bounce_point
#ifdef VERBOSE
		std::cout << "x: " << bounce_point.first << " y: " << bounce_point.second;
#endif	
		if (abs(bounce_point.first) <= 0.01 && bounce_point.second > 0)
		{
#ifdef VERBOSE
			std::cout << "\nreached hole." << std::endl;
#endif		
			break;
		}
		bounce_result result = e.bounce(bounce_seg, bounce_point);
		bounce_seg = result.first;
		bounce_point = result.second;
		hit_count += 1;
#ifdef VERBOSE
		std::cout << " , hitcount: " << hit_count << std::endl;
#endif
	} while (true);

	return hit_count;
}

int main()
{

	auto t1 = std::chrono::high_resolution_clock::now();
	int64_t solution = solve();
	auto t2 = std::chrono::high_resolution_clock::now();
	auto microSec = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
	std::cout << "solution: " << solution << std::endl << "duration: " << microSec << " micro seconds (" << ms << " ms)" << std::endl;
	if (microSec > 300'000'000)
	{
		std::cout << "(" << ((float)microSec) / 60'000'000 << " minutes )" << std::endl;
	}
}

