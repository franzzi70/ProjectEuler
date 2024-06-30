#pragma once

#include <bitset>
#include <vector>

namespace primes
{
	class PrimesSieve
	{
	public:
		int m_maxNumber;

		PrimesSieve(int maxNumber)
		{
			initSieve(maxNumber);
		}

		bool isPrime(int num)
		{
			return !getBitVal(num);
		}

		bool isPrimeExt(__int64 num)
		{
			if (num <= m_maxNumber)
				return isPrime((int)num);
			if ((__int64)m_maxNumber * m_maxNumber < num)
			{
				int upper = (int)sqrt(num) + 1;
				if (num % 2 == 0)
					return false;
				for (int i = 3; i <= upper; i += 2)
				{
					if (num % i == 0)
						return false;
				}
			}
			//return !getBitVal(num);
			return false;
		}

		int nextPrime(int num)
		{
			int _ix = num + 1;
			while (_ix <= m_maxNumber)
			{
				if (isPrime(_ix))
					return _ix;
				_ix += 1;
			}
			// use isPrimeExt to find primes beyond the sieve
			_ix = num + 1;
			while (true)
			{
				if (isPrimeExt(_ix))
					return _ix;
				_ix += 1;
			}
		}

	private:
		std::vector<std::bitset<64>> primesBits;

		bool getBitVal(int num)
		{
			if (num > m_maxNumber || num < 0)
				return false;
			int _vectorIx = num / 64;
			int _bitIx = num % 64;
			return primesBits[_vectorIx][_bitIx];
		}

		void setBitVal(int num, bool val)
		{
			if (num > m_maxNumber || num < 0)
				return;
			int _vectorIx = num / 64;
			int _bitIx = num % 64;
			primesBits[_vectorIx].set(_bitIx, val);
		}

		int initSieve(int maxNumber)
		{
			m_maxNumber = maxNumber;
			int _vectorSize = (maxNumber + 65) / 64;
			primesBits = std::vector<std::bitset<64>>(_vectorSize);

			int _maxI = (int)sqrt(m_maxNumber) + 1;
			setBitVal(0, true);
			setBitVal(1, true);
			for (int k = 2; k <= _maxI; k++)
			{
				if (!getBitVal(k))
				{
					int ix = k+k;
					while (ix <= m_maxNumber)
					{
						setBitVal(ix, true);
						ix += k;
					}
				}
			}
			return 0;
		}

	};
}