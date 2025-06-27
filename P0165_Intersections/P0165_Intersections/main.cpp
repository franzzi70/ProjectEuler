
#include <iostream>
#include <chrono>
#include <cmath>
#include <set>
#include <map>

const uint32_t LINES_COUNT = 5000;
struct line {
	int32_t x1, y1, x2, y2;
};
line lines[LINES_COUNT];

class point {
public:
	double x, y;
	bool operator<(const point& other) const {
		if (x < other.x) return true;
		if (x > other.x) return false;
		return y < other.y; // if x's are equal, compare y's
	}
};

std::map<double,point> g_points;

uint32_t blum_blum_shub(uint32_t seed = 290797)
{
	const uint32_t m = 50515093; // modulus
	uint64_t snplus1 = ((uint64_t)seed * seed) % m;
	return (uint32_t)snplus1;
}

//bool point_on_section(line& section, point& p)
//{
//	// Check if point p is on the line segment defined by section
//	// assume line normalized so that x1 < x2, and vertical lines (x1 == x2) to not exist
//	if (section.x1 <= p.x && p.x <= section.x2) {
//		if (section.x1 == section.x2) { // vertical line
//			return (p.y >= section.y1 && p.y <= section.y2);
//		}
//		else {
//			double slope = (double)(section.y2 - section.y1) / (section.x2 - section.x1);
//			double expected_y = section.y1 + slope * (p.x - section.x1);
//			return (p.y == expected_y);
//		}
//	}
//}

void init_lines()
{
	uint32_t t_n = blum_blum_shub();
	for (uint32_t i = 0; i < LINES_COUNT; ++i)
	{
		int32_t x1 = t_n % 500;
		t_n = blum_blum_shub(t_n);
		int32_t y1 = t_n % 500;
		t_n = blum_blum_shub(t_n);
		int32_t x2 = t_n % 500;
		t_n = blum_blum_shub(t_n);
		int32_t y2 = t_n % 500;
		t_n = blum_blum_shub(t_n);
		if (x1 > x2) {
			std::swap(x1, x2);
			std::swap(y1, y2);
			}
		lines[i] = { x1, y1, x2, y2 };
	}
}

//point* p_intersect_true(line& l1, line& l2 )
//{
//	// This function should calculate the intersection point of two lines l1 and l2
//	// If they intersect, return a pointer to a point containing the intersection coordinates
//	// If they do not intersect, return nullptr
//	// For now, we will just return nullptr as a placeholder
//
//	// return nullptr for parallel sections
//	int32_t lx1 = l1.x2 - l1.x1;
//	int32_t ly1 = l1.y2 - l1.y1;
//	int32_t lx2 = l2.x2 - l2.x1;
//	int32_t ly2 = l2.y2 - l2.y1;
//
//	int32_t det = lx1 * ly2 - ly1 * lx2;
//	if (det == 0) {
//		return nullptr; // parallel lines
//	}
//
//	if (lx1 == 0)
//	{
//		if (lx2 == 0) {
//			return nullptr; // both lines are vertical
//		}
//		else {
//			// l1 is vertical, l2 is not
//			double y = ((double)(l2.y1 - l1.x1 * ly2 / lx2)) / (ly2 / lx2);
//			return new point{ (double)l1.x1, y };
//		}
//	}
//	if (lx2 == 0)
//	{
//		// l2 is vertical, l1 is not
//		double y = ((double)(l1.y1 - l2.x1 * ly1 / lx1)) / (ly1 / lx1);
//		return new point{ (double)l2.x1, y };
//	}
//	else
//	{
//		// both lines are not vertical
//		double a1 = (double)ly1 / lx1; // slope of l1
//		double b1 = (double)l1.y1 - a1 * l1.x1; // y-intercept of l1
//		double a2 = (double)ly2 / lx2; // slope of l2
//		double b2 = (double)l2.y1 - a2 * l2.x1; // y-intercept of l2
//		if (a1 == a2) {
//			return nullptr; // parallel lines
//		}
//		double x = (b2 - b1) / (a1 - a2);
//		double y = a1 * x + b1;
//		return new point{ x, y };
//	}
//
//	return nullptr;
//}

bool intersect_true(line& l1, line& l2)
{
	// This function returns true if two line segments l1 and l2 intersect, false otherwise
	// assume all coordinates are postive integers and normalized so that x1 < x2, and vertical lines (x1 == x2) do not exist
	if (l1.x1 == l1.x2 || l2.x1 == l2.x2) {
		return false; // vertical lines are not allowed
	}
	// Check if the segments are parallel
	// Calculate the direction vectors of the lines
	// and check if they are proportional
	// Calculate the determinant to check if they are parallel
	int32_t lx1 = l1.x2 - l1.x1;
	int32_t ly1 = l1.y2 - l1.y1;
	int32_t lx2 = l2.x2 - l2.x1;
	int32_t ly2 = l2.y2 - l2.y1;
	int32_t det = lx1 * ly2 - ly1 * lx2;
	if (det == 0) {
		return false; // parallel lines
	}
	//double s1 = // slope of l1
	//	(double)(ly1) / (lx1);
	//double s2 = // slope of l2
	//	(double)(ly2) / (lx2);
	double s1 = (double)(l1.y2 - l1.y1) / (l1.x2-l1.x1); // slope of l1
	double s2 = (double)(l2.y2 - l2.y1) / (l2.x2 - l2.x1); // slope of l2
	// y1 = l1.y1 + s1 * x1;
	// y2 = l2.y1 + s2 * x2;
	// x1 = x2
	// y2 = y2
	// solve these 4 equations to find the intersection point
	// If the slopes are equal, the lines are parallel
	if (s1 == s2) {
		return false; // parallel lines
	}
	// Calculate the intersection point using the determinant method
	// The intersection point (x, y) can be calculated as follows:
	// x = (l2.y1 - l1.y1 + s1 * l1.x1 - s2 * l2.x1) / (s1 - s2)
	// y = s1 * (x - l1.x1) + l1.y1

	// l1.y1 + s1 * x1 - l2.y1 - s2 * x2 = 0
	// l1.y1 + s1 * x - l2.y1 - s2 * x = 0
	// Rearranging gives us:
	// x * (x1 - s2) + l1.y1 - l2.y1 = 0
	// x = (l2.y1-l1.y1)/(s1 - s2)
	// expand s1 and s2:
	// x = (l2.y1 - l1.y1 + (double)(ly1) / (lx1) * l1.x1 - (double)(ly2) / (lx2) * l2.x1) / ((double)(ly1) / (lx1) - (double)(ly2) / (lx2))
	// simplify:
	// x = (l2.y1 - l1.y1 + (double)(ly1 * l1.x1) / (lx1) - (double)(ly2 * l2.x1) / (lx2)) / ((double)(ly1 * lx2 - ly2 * lx1) / (lx1 * lx2))
	//  simplify further:
	// x = (l2.y1 - l1.y1 + (double)(ly1 * l1.x1) * lx2 - (double)(ly2 * l2.x1) * lx1) / (ly1 * lx2 - ly2 * lx1)
	// expand ly1 and ly2:
	//  x = (l2.y1 - l1.y1 + (double)(l1.y2 - l1.y1) * l1.x1 * lx2 - (double)(l2.y2 - l2.y1) * l2.x1 * lx1)
	// / (ly1 * lx2 - ly2 * lx1)
	// expand ly1 and ly2:
	//  x = (l2.y1 - l1.y1 + (double)(l1.y2 - l1.y1) * l1.x1 * lx2 - (double)(l2.y2 - l2.y1) * l2.x1 * lx1) / (ly1 * lx2 - ly2 * lx1)
	// 
	// ächts
	// 
	// 
	// 
	// ächts
	// 
	// Calculate the intersection point
	double t = (double)(lx2 * (l1.y1 - l2.y1) - ly2 * (l1.x1 - l2.x1)) / det;
	double u = (double)(lx1 * (l1.y1 - l2.y1) - ly1 * (l1.x1 - l2.x1)) / det;

	// Check if the intersection point is within both segments,
	// but only inner points and not end points of the semgents are counted as true intersection.

	if ((t > 0 && t < 1) && (u > 0 && u < 1))
	{
		// Calculate the intersection point coordinates
		double x_intersect = l1.x1 + t * (l1.x2 - l1.x1);
		double y_intersect = l1.y1 + t * (l1.y2 - l1.y1);
		point p_intersect = { x_intersect, y_intersect };
		g_points.insert(std::make_pair(x_intersect, p_intersect));
		return true;
	}
	return false; // The segments do not intersect
}

void test()
{
	// test detection of parallel sections
	line l1 = { 1, 1, 4, 3 };
	line l2 = { 2, 2, 5, 4 };
	bool b_intersect = intersect_true(l1, l2);
	if (b_intersect) 
		{
		std::cout << "Lines intersect." << std::endl;
	}
	else {
		std::cout << "Lines do not intersect." << std::endl;
	}
}

__int64 solve()
{
	init_lines();
	test();
	
	uint32_t point_count = 0;
	for (uint32_t i = 0; i < LINES_COUNT; ++i)
		for (uint32_t j = 0; j < i; ++j)
		{
			if (intersect_true(lines[i], lines[j]))
			{
				point_count++;
				//std::cout << "Lines " << i << " and " << j << " intersect." << std::endl;
			}
			else
			{
				//std::cout << "Lines " << i << " and " << j << " do not intersect." << std::endl;
			}
		}

	point_count = 0;
	double last_x = -1;
	double last_y = -1;
	for (auto& p : g_points)
	{
		bool b_close = false;
		if (p.first - last_x < 0.0001) // avoid duplicates
		{
			if (p.second.y - last_y < 0.0001) // avoid duplicates
			{
				double distance = std::sqrt(std::pow(p.second.x - last_x, 2) + std::pow(p.second.y - last_y, 2));
				if (distance < 1E-9) // close enough to last point
				{
					//b_close = true;
					std::cout << "Point: (" << p.second.x << ", " << p.second.y << ") is close to last point; Distance: " << distance << " , skipping." << std::endl;
				}
				//b_close = true;
				//std::cout << "Point: (" << p.second.x << ", " << p.second.y << ") is close to last point " << last_x << ", " << last_y << " , skipping." << std::endl;

			}
			//std::cout << "Point: (" << p.second.x << ", " << p.second.y << ")" << std::endl;
		}
		last_x = p.second.x;
		last_y = p.second.y;
		if (!b_close)
			point_count += 1;
	}
    return point_count;
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
