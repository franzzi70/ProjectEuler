
#include <iostream>
#include <chrono>

const uint32_t LINES_COUNT = 5000;
struct line {
	uint32_t x1, y1, x2, y2;
};
line lines[LINES_COUNT];

struct point {
	double x, y;
};

uint32_t blum_blum_shub(uint32_t seed = 290797)
{
	const uint32_t m = 50515093; // modulus
	uint64_t snplus1 = ((uint64_t)seed * seed) % m;
	return snplus1;
}

void init_lines()
{
	uint32_t t_n = blum_blum_shub();
	for (uint32_t i = 0; i < LINES_COUNT; ++i)
	{
		uint32_t x1 = t_n % 500;
		t_n = blum_blum_shub(t_n);
		uint32_t y1 = t_n % 500;
		t_n = blum_blum_shub(t_n);
		uint32_t x2 = t_n % 500;
		t_n = blum_blum_shub(t_n);
		uint32_t y2 = t_n % 500;
		t_n = blum_blum_shub(t_n);
		if (x1 > x2) {
			std::swap(x1, x2);
			std::swap(y1, y2);
			}
		lines[i] = { x1, y1, x2, y2 };
	}
}

point* intersect_true(line& l1, line& l2 )
{
	// This function should calculate the intersection point of two lines l1 and l2
	// If they intersect, return a pointer to a point containing the intersection coordinates
	// If they do not intersect, return nullptr
	// For now, we will just return nullptr as a placeholder

	// return nullptr for parallel sections
	int32_t lx1 = l1.x2 - l1.x1;
	int32_t ly1 = l1.y2 - l1.y1;
	int32_t lx2 = l2.x2 - l2.x1;
	int32_t ly2 = l2.y2 - l2.y1;

	int32_t det = lx1 * ly2 - ly1 * lx2;
	if (det == 0) {
		return nullptr; // parallel lines
	}

	if (lx1 == 0)
	{
		if (lx2 == 0) {
			return nullptr; // both lines are vertical
		}
		else {
			// l1 is vertical, l2 is not
			double y = ((double)(l2.y1 - l1.x1 * ly2 / lx2)) / (ly2 / lx2);
			return new point{ (double)l1.x1, y };
		}
	}
	if (lx2 == 0)
	{
		// l2 is vertical, l1 is not
		double y = ((double)(l1.y1 - l2.x1 * ly1 / lx1)) / (ly1 / lx1);
		return new point{ (double)l2.x1, y };
	}
	else
	{
		// both lines are not vertical
		double a1 = (double)ly1 / lx1; // slope of l1
		double b1 = (double)l1.y1 - a1 * l1.x1; // y-intercept of l1
		double a2 = (double)ly2 / lx2; // slope of l2
		double b2 = (double)l2.y1 - a2 * l2.x1; // y-intercept of l2
		if (a1 == a2) {
			return nullptr; // parallel lines
		}
		double x = (b2 - b1) / (a1 - a2);
		double y = a1 * x + b1;
		return new point{ x, y };
	}

	return nullptr;
}

void test()
{
	// test detection of parallel sections
	line l1 = { 1, 1, 4, 3 };
	line l2 = { 2, 2, 5, 4 };
	point* p = intersect_true(l1, l2);
	if (p != nullptr) {
		std::cout << "Intersection at: (" << p->x << ", " << p->y << ")" << std::endl;
	}
	else {
		std::cout << "No intersection." << std::endl;
	}


}

__int64 solve()
{
	init_lines();
	test();
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
