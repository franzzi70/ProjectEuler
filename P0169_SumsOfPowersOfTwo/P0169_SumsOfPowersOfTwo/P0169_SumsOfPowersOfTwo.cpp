

#include <iostream>
#include <chrono>
#include <vector>

int comb(int n, int k) {
	if (k > n)
		return 0;
	if (k == 0 || k == n)
		return 1;
	int64_t sum = 1;
	for (int i = 1; i <= k; i++) {
		sum *= n - (k - i);
		sum /= i;
	}
	return sum;
}



class binVec {
public:
	
	binVec() : v(1, 0), highPos(0) {}

	binVec(const binVec& other) : v(other.v), highPos(other.highPos) {}

	binVec(int64_t num) {
		int64_t n = num;
		while (n > 0) {
			v.push_back(n % 2);
			n /= 2;
		}
		highPos = v.size() - 1;
	}

	binVec(std::vector<int8_t>& v, int highPos) : v(v), highPos(highPos) {}

	void pow10(int pow10) {

		int maxdigits = ceil(pow10 * log(10) / log(2));
		v = std::vector<int8_t>(maxdigits, 0);
		v[0] = 1;
		for (int i = 1; i <= pow10; i++) {
			*this = this->leftshift(3) + this->leftshift(1);
		}
	}

	void print() const {
		for (int i = highPos; i >= 0; i--) {
			std::cout << (int)v[i];
		}
		if (highPos < 40)
		{
			// print decimal:
			std::cout << " (";
			int64_t sum = 0;
			int64_t pow2 = 1;
			for (int i = 0; i <= highPos; i++) {
				sum += v[i] * pow2;
				pow2 *= 2;
			}
			std::cout << sum << ")";
		}
		std::cout << std::endl;
	}

	binVec _plus(const binVec longVec, const binVec& shortVec) const {
		binVec result(*this);
		int highPos = longVec.highPos;
		int carry = 0;
		for (int i = 0; i <= shortVec.highPos; i++)
		{
			int8_t sum = longVec.v[i] + shortVec.v[i] + carry;
			result.v[i] = sum % 2;
			carry = sum / 2;
		}
		for (int i = shortVec.highPos + 1; i <= highPos; i++)
		{
			int8_t sum = longVec.v[i] + carry;
			result.v[i] = sum % 2;
			carry = sum / 2;
		}
		if (carry > 0) {
			result.v[highPos + 1] = carry;
			result.highPos = highPos + 1;
		}
		return result;
	}

	binVec operator+(const binVec& other) const {
		if (other.highPos > highPos) {
			return _plus(other, *this);
		}
		else
		{
			return _plus(*this, other);
		}
	}

	binVec leftshift (int shift) const {
		binVec result(*this);
		for (size_t i = result.highPos + shift; i >= shift; --i) {
			result.v[i] = result.v[i - shift];
		}
		for (size_t i = 0; i < shift; ++i) {
			result.v[i] = 0;
		}
		result.highPos += shift;
		return result;
	}

	int64_t variations_bak (int n) const {
		if (n == 0)
			return 1;
		int64_t sum = 1;
		int i = n-1;
		//if (v[i] == 1) {
		//	return 1 + variations(i);
		//}
		while (i >= 0 && v[i] == 0) {
			sum += 1;
			i--;
		}
		if (i < 0)
			return sum;
		else
		{
			sum *= variations(i);
			sum += variations(i-1);
			//while (i >= 0 && v[i] == 1) {
			//	i--;
			//}
			//sum += variations(i);
		}
	}

	int64_t variations(int n) const {
		if (n <= 0)
			return 1;
		int64_t sum = 0;
		if (v[n] == 1) {
			int ix1 = n;
			while (ix1 >= 0 && v[ix1] != 0) {
				ix1--;
			}
			int oneCount = n - ix1;
			int ix0 = ix1;
			while (ix0 >= 0 && v[ix0] == 0) {
				ix0--;
			}
			int zeroCount = ix1 - ix0;
			int clusterComb = oneCount * zeroCount;// comb(oneCount + zeroCount, oneCount);
			sum = clusterComb * variations(ix0);
		}
		// TODO: add the cases where the last 1 is pushed onto low one of higher cluster:


		// NOT IMPLEMENTED YET.


	}

	int64_t solve() const {
		return variations(highPos);
	}

	std::vector<int8_t> v;
	int highPos = 0;


};

void test10()
{

	binVec v10 = binVec(10);
	v10.print();
	std::cout << v10.solve() << std::endl;

	binVec v20 = binVec(20);
	v20.print();
	std::cout << v20.solve() << std::endl;

	binVec v28 = binVec(28);
	v28.print();
	std::cout << v28.solve() << std::endl;

	//binVec v25 = binVec();
	//v25.pow10(25);
	//v25.print();
	//std::cout << v25.solve() << std::endl;


}


int64_t solve()
{

	test10();

	std::cout << comb(5, 2) << std::endl;
	std::cout << comb(4, 2) << std::endl;

	binVec v10pow25 = binVec(10);
	v10pow25.pow10(10);
	v10pow25.print();
	//std::cout << v10pow25.solve() << std::endl;

	//return v10pow25.solve();
	return 0;
}

int main()
{
	auto t1 = std::chrono::high_resolution_clock::now();
	volatile int64_t solution = solve();
	auto t2 = std::chrono::high_resolution_clock::now();
	auto microSec = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
	std::cout << "solution: " << solution << std::endl << "duration: " << microSec << " micro seconds (" << ms << " ms)" << std::endl;
	if (microSec > 300'000'000)
	{
		std::cout << "(" << ((float)microSec) / 60'000'000 << " minutes )" << std::endl;
	}
}
