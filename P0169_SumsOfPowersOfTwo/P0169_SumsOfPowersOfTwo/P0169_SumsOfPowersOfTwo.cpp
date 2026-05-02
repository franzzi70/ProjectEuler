

#include <iostream>
#include <chrono>
#include <vector>

std::vector<int64_t> rmem(100 * 100, -1);

// (oneCount,zeroCount) : variations -> (x,y) : z
// r(n,0) : 1
// r(1,n) : n + 1
// r(n,1) : 1 + (n-1,1)
// r(n,2) : (n,1) + (n-1,2)
// r(n,3) : (n,2) + (n-1,3)
int64_t r(int n, int m)
{
	if (n>=100 || m>= 100)
	{
		std::cerr << "n or m too big: " << n << "," << m << std::endl;
		return 0;
	}

	if (m == 0)
		return 1;
	if (n == 1)
		return n + 1;
	int64_t memVal = rmem[n*1000+m];
	if (memVal != -1)
		return memVal;
	else
	{
		int64_t val = r(n, m - 1) + r(n - 1, 1);
		rmem[n * 1000 + m] = val;
		return val;
	}
}

//int64_t comb(int n, int k) {
//	if (k > n)
//		return 0;
//	if (k == 0 || k == n)
//		return 1;
//	int64_t sum = 1;
//	for (int i = 1; i <= k; i++) {
//		sum *= n - (k - i);
//		sum /= i;
//	}
//	return sum;
//}

class cluster {
public:
	cluster(int oneCount, int zeroCount) : oneCount(oneCount), zeroCount(zeroCount) {}
	int oneCount;
	int zeroCount;
	cluster* next = nullptr;

	void print() const {
		for (int i=0;i<oneCount;i++) {
			std::cout << "1";
		}
		for (int i=0;i<zeroCount;i++) {
			std::cout << "0";
		}
		if (next != nullptr) {
			next->print();
		}
	}

	int64_t variations(bool pushed) const {
		// 1st: within cluster: 1 + variations of the rest of the clusters 1 in the current cluster and m less zero in the current cluster (if there are enough zeros)

		// (oneCount,zeroCount) : variations -> (x,y) : z
		// (n,0) : 1
		// (1,n) : n + 1		<1000> <0200> <0120> <0112>
		// (n,1) : 1 + (n-1,1)
		//	=> n=2: (<110> <102> <022>)
		//	=> n=3: (<1110>) + (<1102> <1022> <0222>)
		// (n,2) : (n,1) + (n-1,1lit)
		//	=> n=2:	(<1100> <1020> <0220>) + (<1012> <0212>)
		//  => n=3:	(<11100> <11020> <10220> <02220>) + (<11012> <10212> <02212>)
		// (n,3) : (n,2) + (n-1,1lit)
		//	=> n=2: ((<11000> <10200> <02200>) + (<10120> <02120>))  + (<10112> <02112>)
		// ----------------------------
		// (n,m) : (n,m-1) + (n-1,1)
		// ============================
		int64_t count = 0;
		int64_t next_variations = 1;
		int64_t next_variations_push = 0;
		bool canPush = false;
		if (next != nullptr) {
			 next_variations = next->variations(false);
			 if (next->zeroCount > 0)
			 {
				 canPush = true;
				 next_variations_push = next->variations(true);
			 }
		}
		
		if (!pushed)
		{
			count += r(oneCount, zeroCount) * next_variations;
			count += (oneCount) * next_variations_push;
		}
		else
		{
			// push: <11111000> -> <22222200>	-> equiv to <100>  ( <22222200> <22222120> <22222112> )
			count += r(1, zeroCount-1) * next_variations;
			count += (oneCount +1) * next_variations_push;
		}

		return count;
	}
};


class binVec {
public:
	
	binVec() : v(1, 0), highPos(0) {}
	binVec(const binVec& other) : v(other.v), highPos(other.highPos) {
	}
	binVec(int64_t num) {
		int64_t n = num;
		while (n > 0) {
			v.push_back(n % 2);
			n /= 2;
		}
		highPos = v.size() - 1;
	}

	binVec(std::vector<int8_t>& v, int highPos) : v(v), highPos(highPos) {}

	~binVec() {
		cluster* c = clusterChain;
		while (c != nullptr) {
			cluster* next = c->next;
			delete c;
			c = next;
		}
	}

	void pow10(int pow10) {

		int maxdigits = (int) ceil(pow10 * log(10) / log(2));
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

	int64_t solve() {
		createClusterChain();
		return clusterChain->variations(false);
	}

	void printClustersChain() {
		createClusterChain();
		if (clusterChain != nullptr)
			clusterChain->print();
	}

	std::vector<int8_t> v;
	int highPos = 0;

private:
	cluster* clusterChain = nullptr;

private:
	inline cluster* getCluster(int& ix) {
		int oneCount = 0;
		int zeroCount = 0;
		while (ix >= 0 && v[ix] == 1) {
			oneCount++;
			ix--;
		}
		while (ix >= 0 && v[ix] == 0) {
			zeroCount++;
			ix--;
		}
		return new cluster(oneCount, zeroCount);
	}
	
	void createClusterChain()
	{
		if (clusterChain != nullptr) {
			return;
		}
		int ix = highPos;
		cluster* prevCluster = nullptr;
		while (ix > 0) {
			cluster* c = getCluster(ix);
			if (prevCluster != nullptr) {
				prevCluster->next = c;
			}
			else
			{
				clusterChain = c;
			}
			prevCluster = c;
		}
	}

};

void test()
{

	//binVec v25 = binVec();
	//v25.pow10(25);
	//v25.printClustersChain();
	//std::cout << "\n------------------\n";
	//v25.print();

	//binVec v10 = binVec(10);
	//v10.print();
	//std::cout << v10.solve() << std::endl;

	//binVec v20 = binVec(20);
	//v20.print();
	//std::cout << v20.solve() << std::endl;

	//binVec v28 = binVec(28);
	//v28.print();
	//std::cout << v28.solve() << std::endl;

	//binVec v29 = binVec(29);
	//v29.print();
	//std::cout << v29.solve() << std::endl;

	//binVec v25 = binVec();
	//v25.pow10(25);
	//v25.print();
	//std::cout << v25.solve() << std::endl;


}


int64_t solve()
{

	test();

	//std::cout << comb(5, 2) << std::endl;
	//std::cout << comb(4, 2) << std::endl;

	binVec v10pow25 = binVec(10);
	v10pow25.pow10(10);
	//v10pow25.print();
	//std::cout << "------------------" << std::endl;
	//v10pow25.printClustersChain();
	//std::cout << v10pow25.solve() << std::endl;

	return v10pow25.solve();
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
