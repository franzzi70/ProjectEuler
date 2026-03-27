

#include <iostream>
#include <format>
#include <chrono>
#include <cmath>
#include <set>
#include <map>
#include <cassert>
#include <mutex>
#include <algorithm>
#include <queue>

#define NOMINMAX
#include <windows.h>



#define THREADCOUNT 16

const int32_t LINES_COUNT = 5000;

typedef std::pair<int64_t, int64_t> fraction;
typedef std::pair<fraction, fraction> qpoint;

struct rec_shared{
	int32_t tread_count;
	std::mutex& mtx;
	int32_t thread_mod;
	std::set<qpoint>& qpoints;
	std::vector <qpoint> *candidates[THREADCOUNT];
};

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


int64_t solve_single_t()
{
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

/*
12497500 total pairs of lines were checked.
9628503 pairs of lines did not intersect.
2868997 total intersection points were found.
129 points were repeated.
2868868 distinct intersection points were found.
solution: 2868868
duration: 2256846 micro seconds (2256 ms)
*/

    return unique_count;
}

void thread_body(rec_shared* shared)
{
	std::vector <qpoint>* candidates = new std::vector<qpoint>();

	// not implemented
	std::cout << "thread body started" << std::endl;

	shared->mtx.lock();
	int32_t thread_count = shared->tread_count;
	int32_t thread_mod = shared->thread_mod;
	shared->thread_mod += 1;
	shared->candidates[thread_mod] = candidates;
	shared->mtx.unlock();

	candidates->reserve(((LINES_COUNT * LINES_COUNT) / 2) / thread_count);

	int32_t i = 0;
	int32_t j = thread_mod + 1;

	while (i < LINES_COUNT)
	{
		//std::cout << "testing lines " << i << " and " << j << std::endl;

		qpoint q_intersect = { {0,0}, {0,0} };
		if (test_intersect_true(lines[i], lines[j], q_intersect))
		{
			//std::cout << "thread " << std::this_thread::get_id() << " found new point: " << q_intersect.first.first << "/" << q_intersect.first.second << ", "
			//	<< q_intersect.second.first << "/" << q_intersect.second.second << std::endl;
			// register point
			candidates->push_back(q_intersect);
		}

		int32_t offs = thread_count;
		j += offs;
		while (j >= LINES_COUNT)
		{
			//offs -= (j - LINES_COUNT);
			j = i + 1 + thread_mod;
			i += 1;
			if (i >= LINES_COUNT)
				break;
		}
		if (i >= LINES_COUNT)
			break;
	}


	std::sort(candidates->begin(), candidates->end());


	shared->mtx.lock();
	std::cout << "thread " << std::this_thread::get_id() << " (offset " << thread_mod << ") found " << candidates->size() << " candidate points." << std::endl;
	shared->mtx.unlock();


	return;
}


int64_t solve_multi_t(int32_t thread_count)
{
	// not implemented
	std::set<qpoint> qpoints;
	std::mutex mtx;


	//int32_t used_thread_count = std::min(thread_count, (int32_t)std::thread::hardware_concurrency());
	int32_t used_thread_count = thread_count;
	std::cout << "Using " << used_thread_count << " threads." << std::endl;
	std::vector<std::thread*> threads(used_thread_count);

	rec_shared shared{
		.tread_count = used_thread_count,
		.mtx = mtx,
		.thread_mod = 0,
		.qpoints = qpoints,
	};


	for (int i = 0; i < used_thread_count; ++i)
	{
		std::cout << std::thread::hardware_concurrency() << " hardware threads supported." << std::endl;
		threads[i] = new std::thread(thread_body, &shared);
	}
	
	for (int i = 0; i < used_thread_count; ++i)
	{
		if (threads[i]->joinable())
			threads[i]->join();
	}

	// get unique points count from candidates:
// get unique points count from candidates:

// Method 1: K-way merge using priority queue (most efficient for sorted lists)
	struct QPointIterator {
		std::vector<qpoint>::iterator current;
		std::vector<qpoint>::iterator end;
		int thread_id;

		bool operator>(const QPointIterator& other) const {
			return *current > *other.current; // Min-heap
		}
	};

	std::priority_queue<QPointIterator, std::vector<QPointIterator>, std::greater<QPointIterator>> pq;

	// Initialize priority queue with first element from each list
	for (int i = 0; i < used_thread_count; ++i) {
		if (shared.candidates[i] && !shared.candidates[i]->empty()) {
			pq.push({ shared.candidates[i]->begin(), shared.candidates[i]->end(), i });
		}
	}

	int unique_count_from_merge = 0;
	qpoint last_point = { {0, 0}, {0, 0} };
	bool first = true;

	while (!pq.empty()) {
		QPointIterator top = pq.top();
		pq.pop();

		// Count unique (skip duplicates)
		if (first || *top.current != last_point) {
			unique_count_from_merge++;
			last_point = *top.current;
			first = false;
		}

		// Advance iterator and re-insert if more elements remain
		++top.current;
		if (top.current != top.end) {
			pq.push(top);
		}
	}

	std::cout << "Unique points from K-way merge: " << unique_count_from_merge << std::endl;


	for (int i = 0; i < used_thread_count; ++i)
	{
		delete shared.candidates[i];
		delete threads[i];
	}

	return unique_count_from_merge;
}


int64_t solve()
{
	init_lines();
	if (THREADCOUNT == 1) {
		return solve_single_t();
	}
	else {
		return solve_multi_t(THREADCOUNT);
	}
}

//bool SetMultiCoreAffinity(int coreCount) {
//	// Create mask for first N cores: 0xFF for 8 cores, 0x0F for 4 cores, etc.
//	DWORD_PTR mask = (1ULL << coreCount) - 1;
//
//	if (!SetProcessAffinityMask(GetCurrentProcess(), mask)) {
//		std::cerr << "Failed to set process affinity. Error: " << GetLastError() << std::endl;
//		return false;
//	}
//	std::cout << "Process affinity set to " << coreCount << " cores" << std::endl;
//	return true;
//}

// Usage:


int main()
{
	// SetProcessAffinityMask(GetCurrentProcess(), 0x00000001); // use only first core for testing


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
