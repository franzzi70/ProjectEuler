

#include <iostream>
#include <format>
#include <chrono>
#include <cmath>
#include <set>
#include <map>
#include <cassert>


const int32_t LINES_COUNT = 5000;

typedef std::pair<int64_t, int64_t> fraction;
typedef std::pair<fraction, fraction> qpoint;


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

std::map<double, point> g_points;


int32_t blum_blum_shub(int32_t seed = 290797)
{
	const int32_t m = 50515093; // modulus
	int32_t snplus1 = (int32_t)(((int64_t)seed * seed) % m);
	assert(snplus1 >= 0 && snplus1 < m);
	return snplus1;
}

int64_t gcd(int64_t a, int64_t b)
{
	// Calculate the greatest common divisor using the Euclidean algorithm
	while (b != 0) {
		int64_t temp = b;
		b = a % b;
		a = temp;
	}
	return a;
}

fraction reduce_fraction(fraction f)
{
	// Reduce the fraction to its simplest form
	int64_t gcd_value = gcd(std::abs(f.first), std::abs(f.second));
	if (f.second < 0)
	{
		return { -f.first / gcd_value, -f.second / gcd_value };
	}
	return {f.first / gcd_value, f.second / gcd_value };
}

qpoint test_intersection(line& l1, line& l2, bool& is_parallel)
{
	// This function returns true if two line segments l1 and l2 intersect, false otherwise
	// assume all coordinates are postive integers and normalized so that x1 < x2, and vertical lines (x1 == x2) do not exist

	//assert(!(l1.x1 == l1.x2 || l2.x1 == l2.x2)); // vertical lines are not allowed

	// Check if the segments are parallel
	// Calculate the direction vectors of the lines
	// and check if they are proportional
	// Calculate the determinant to check if they are parallel
	int64_t x1 = l1.x1;
	int64_t x2 = l1.x2;
	int64_t x3 = l2.x1;
	int64_t x4 = l2.x2;
	int64_t y1 = l1.y1;
	int64_t y2 = l1.y2;
	int64_t y3 = l2.y1;
	int64_t y4 = l2.y2;

	int64_t det_l1 = x1 * y2 - x2 * y1;
	int64_t det_l2 = x3 * y4 - x4 * y3;

	int64_t det = (x1 - x2) * (y3 - y4) - (x3 - x4) * (y1 - y2);

	if (det == 0) {
		is_parallel = true;
		return { {0,0}, {0,0} }; // parallel lines
	}
	else
		is_parallel = false;

	fraction x = reduce_fraction({ det_l1 * (x3 - x4) - det_l2 * (x1 - x2), det });
	fraction y = reduce_fraction({ det_l1 * (y3 - y4) - det_l2 * (y1 - y2), det });

	return { x, y };
}

void init_lines()
{
	int32_t t_n = blum_blum_shub();
	for (int32_t i = 0; i < LINES_COUNT; ++i)
	{
		int32_t x1 = t_n % 500;
		t_n = blum_blum_shub(t_n);
		int32_t y1 = t_n % 500;
		t_n = blum_blum_shub(t_n);
		int32_t x2 = t_n % 500;
		t_n = blum_blum_shub(t_n);
		int32_t y2 = t_n % 500;
		t_n = blum_blum_shub(t_n);
		//if (x1 > x2) {
		//	std::swap(x1, x2);
		//	std::swap(y1, y2);
		//	}
		lines[i] = { x1, y1, x2, y2 };
	}
}


bool point_in_line(line& line, qpoint& q_intersect)
{
	// test if point is within both segments:
	int64_t x1_scaled = line.x1 * q_intersect.first.second;
	int64_t x2_scaled = line.x2 * q_intersect.first.second;
	int64_t x_scaled = q_intersect.first.first;
	if (x1_scaled == x2_scaled)
	{
		if (x_scaled != x1_scaled)
			return false;
	}
	else
	{
		if (x1_scaled > x2_scaled)
			std::swap(x1_scaled, x2_scaled);
		if (x_scaled <= x1_scaled || x_scaled >= x2_scaled)
			return false;
	}

	int64_t y1_scaled = line.y1 * q_intersect.second.second;
	int64_t y2_scaled = line.y2 * q_intersect.second.second;
	int64_t y_scaled = q_intersect.second.first;
	if (y1_scaled == y2_scaled)
	{
		if (y_scaled != y1_scaled)
			return false;
	}
	else
	{
		if (y1_scaled > y2_scaled)
			std::swap(y1_scaled, y2_scaled);
		if (y_scaled <= y1_scaled || y_scaled >= y2_scaled)
			return false;
	}
	return true;
}

bool test_intersect_true(line& l1, line& l2, qpoint& p)
{
	// hitbox test first:
	if (
		std::max(l1.x1, l1.x2) <= std::min(l2.x1, l2.x2)
		|| std::max(l2.x1, l2.x2) <= std::min(l1.x1, l1.x2)
		|| std::max(l1.y1, l1.y2) <= std::min(l2.y1, l2.y2)
		|| std::max(l2.y1, l2.y2) <= std::min(l1.y1, l1.y2)
		)
		return false;

	p = { {0,0}, {0,0} };
	bool is_parallel = false;
	qpoint q_intersect = test_intersection(l1, l2, is_parallel);
	if (is_parallel)
		return false;
	if (!point_in_line(l1, q_intersect))
		return false;
	if (!point_in_line(l2, q_intersect))
		return false;

	p = q_intersect;
	return true;
}



__int64 solve()
{
	init_lines();
	// test();

	std::set<qpoint> qpoints;
	int32_t repeat_count = 0;
	int32_t unique_count = 0;
	int32_t intersect_count = 0;
	int32_t non_intersect_count = 0;

	qpoint q_intersect = { {0,0}, {0,0} };

	for (int32_t i = 0; i < LINES_COUNT; ++i)
	{
		// std::cout << i << " / " << LINES_COUNT << "\r";
		for (int32_t j = i + 1; j < LINES_COUNT; ++j)
		{
			if (test_intersect_true(lines[i], lines[j], q_intersect))
			{
				intersect_count += 1;
				// register point
				if (qpoints.count(q_intersect) > 0)
				{
					repeat_count += 1;
					//std::cout << "point already registered: " << q_intersect.first.first << "/" << q_intersect.first.second << ", "
					//	<< q_intersect.second.first << "/" << q_intersect.second.second << std::endl;
				}
				else
				{
					unique_count += 1;
					qpoints.insert(q_intersect);
					//std::string str = std::format("({},{})",x_intersect_fraction, y_intersect_fraction);
					//std::cout << "(" << x_intersect_fraction.first << "/" << x_intersect_fraction.second << "," << y_intersect_fraction.first << "/" << y_intersect_fraction.second << ")\n";
				}
			}
			else
			{
				non_intersect_count += 1;
			}
		}
	}

	std::cout << intersect_count + non_intersect_count << " total pairs of lines were checked." << std::endl;
	std::cout << non_intersect_count << " pairs of lines did not intersect." << std::endl;

	std::cout << intersect_count << " total intersection points were found." << std::endl;
	std::cout << repeat_count << " points were repeated." << std::endl;
	std::cout << unique_count << " distinct intersection points were found." << std::endl;

    return unique_count;
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
