
#include <set>


#pragma once
class primes
{
public:
	primes();
	primes(int numPrimes);
	~primes();

	bool isPrime(int number);
	bool isPrime(__int64 number);
	void addPrimes(int numPrimes);
	void addPrimesUpTo(int prime);
	int getHighStoredPrime();

private:

	std::set<int> m_primes;

};

