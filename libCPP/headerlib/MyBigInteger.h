#pragma once

#include <vector>
#include <string>
#include <stack>

#define LOG_CALCULATIONS
#undef LOG_CALCULATIONS

class MyBigInteger
{
public:
	MyBigInteger() : MyBigInteger(0) {};
	MyBigInteger(const MyBigInteger& src) // copy constructor
	{
		this->m_base = src.m_base;
		this->m_digits = src.m_digits;
		this->m_size = src.m_size;
		this->m_negative = src.m_negative;
	}
	MyBigInteger& operator = (const MyBigInteger& src) // copy assignment
	{
		this->m_base = src.m_base;
		this->m_digits = src.m_digits;
		this->m_size = src.m_size;
		this->m_negative = src.m_negative;
		return *this;
	};

	MyBigInteger(__int32 val, int base = DEFAULT_BASE) {
		m_base = base;
		if (val == 0)
		{
			m_digits.push_back(0);
			m_size = 1;
			m_negative = false;
			return;
		}
		__int32 tmpVal = std::abs(val);
		int size = 0;
		m_negative = val < 0;
		while (tmpVal > 0)
		{
			m_digits.push_back(tmpVal % m_base);
			tmpVal /= m_base;
			size += 1;
		}
		m_size = size;
	};

	MyBigInteger(__int64 val, int base = DEFAULT_BASE) {
		m_base = base;
		if (val == 0)
		{
			m_digits.push_back(0);
			m_size = 1;
			m_negative = false;
			return;
		}
		__int64 tmpVal = std::abs(val);
		int size = 0;
		m_negative = val < 0;
		while (tmpVal > 0)
		{
			__int32 digit = (__int32)(tmpVal % m_base);
			tmpVal /= m_base;
			m_digits.push_back(digit);
			size += 1;
		}
		m_size = size;
	};

	~MyBigInteger() {};
	MyBigInteger operator + (const MyBigInteger& other) {
		MyBigInteger result = *this;
		result += other;
		return result;
	};

	MyBigInteger operator += (const MyBigInteger& other) {
#ifdef LOG_CALCULATIONS
		MyBigInteger bak_this(*this);
#endif
		if (other.m_negative == this->m_negative)
			return _unsignedAdd(other);
		else
		{
			if (_isAbsGreaterThan(other))
			{
				this->_unsignedSub(other);
#ifdef LOG_CALCULATIONS
				std::cout << "operator +=: " << (std::string)(bak_this) << " + " << (std::string)other << " = " << (std::string)(*this) << std::endl;
#endif
				_normalize();
				return *this;
			}
			else
			{
				MyBigInteger tmp = other;
				tmp._unsignedSub(*this);
				*this = tmp;
#ifdef LOG_CALCULATIONS
				std::cout << "operator +=: " << (std::string)(bak_this) << " + " << (std::string)other << " = " << (std::string)(*this) << std::endl;
#endif
				_normalize();
				return *this;
			}
		}

	};

	MyBigInteger operator - (const MyBigInteger& other) {
		MyBigInteger result = *this;
		result -= other;
		return result;
	};

	MyBigInteger operator -= (const MyBigInteger& other)
	{
#ifdef LOG_CALCULATIONS
		MyBigInteger bak_this(*this);
#endif
		if (m_negative != other.m_negative)
		{
			return _unsignedAdd(other);
		}
		else
		{
			if (_isAbsGreaterThan(other))
			{
				this->_unsignedSub(other);
#ifdef LOG_CALCULATIONS
				std::cout << "operator -=: " << (std::string)(bak_this) << " - " << (std::string)other << " = " << (std::string)(*this) << std::endl;
#endif
				_normalize();
				return *this;
			}
			else
			{
				MyBigInteger tmp = other;
				tmp._unsignedSub(*this);
				tmp.m_negative = !tmp.m_negative;
				*this = tmp;
#ifdef LOG_CALCULATIONS
				std::cout << "operator -=: " << (std::string)(bak_this) << " - " << (std::string)other << " = " << (std::string)(*this) << std::endl;
#endif
				_normalize();
				return *this;
			}
		}
	};

	MyBigInteger operator - () {
		MyBigInteger result = *this;
		result.m_negative = !result.m_negative;
		return result;
	};

	MyBigInteger operator * (const MyBigInteger& other) {
		MyBigInteger result = *this;
		result *= other;
		return result;
	};

	MyBigInteger operator *= (const MyBigInteger& other) {
#ifdef LOG_CALCULATIONS
		MyBigInteger bak_this(*this);
#endif
		if (isZero())
			return *this;
		if (other.isZero())
		{
			*this = other;
			return *this;
		}
		bool isNegative = m_negative != other.m_negative;
		_unsignedMul(other);
		m_negative = isNegative;
#ifdef LOG_CALCULATIONS
		std::cout << "operator *=: " << (std::string)(bak_this) << " * " << (std::string)other << " = " << (std::string)(*this) << std::endl;
#endif
		return *this;
	};

	MyBigInteger operator * (__int32 factor) {
		MyBigInteger result = *this;
		result *= factor;
		return result;
	};

	MyBigInteger operator *= (__int32 factor) {
#ifdef LOG_CALCULATIONS
		MyBigInteger bak_this(*this);
#endif
		if (factor == 0)
		{
			*this = 0;
			return *this;
		}
		if (factor < 0)
		{
			m_negative = !m_negative;
			factor = std::abs(factor);
		}
		__int32 carry = 0;
		for (int i = 0; i < m_size; i++)
		{
			__int64 tmp = (__int64)m_digits[i] * (__int64)factor + (__int64)carry;
			m_digits[i] = tmp % m_base;
			carry = (__int32)(tmp / m_base);
		}
		if (carry > 0)
		{
			m_digits.push_back(carry);
			m_size += 1;
		}
#ifdef LOG_CALCULATIONS
		std::cout << "operator *=: " << (std::string)(bak_this) << " * " << factor << " = " << (std::string)(*this) << std::endl;
#endif
		return *this;
	};

	MyBigInteger operator * (unsigned __int32 factor) {
		MyBigInteger result = *this;
		result *= factor;
		return result;
	};

	MyBigInteger operator *= (unsigned __int32 factor) {
#ifdef LOG_CALCULATIONS
		MyBigInteger bak_this(*this);
#endif
		if (factor == 0)
		{
			*this = 0;
#ifdef LOG_CALCULATIONS
			std::cout << "operator *=: " << (std::string)(bak_this) << " * " << factor << " = " << (std::string)(*this) << std::endl;
#endif
			return *this;
		}
		__int32 carry = 0;
		for (int i = 0; i < m_size; i++)
		{
			__int64 tmp = (__int64)m_digits[i] * (__int64)factor + (__int64)carry;
			m_digits[i] = tmp % m_base;
			carry = (__int32)(tmp / m_base);
		}
		if (carry > 0)
		{
			m_digits.push_back(carry);
			m_size += 1;
		}
#ifdef LOG_CALCULATIONS
		std::cout << "operator *=: " << (std::string)(bak_this) << " * " << factor << " = " << (std::string)(*this) << std::endl;
#endif
		return *this;
	};

	MyBigInteger operator / (__int32 divisor) {
		MyBigInteger result = *this;
		result /= divisor;
		return result;
	};

	MyBigInteger operator /= (__int32 divisor) {
#ifdef LOG_CALCULATIONS
		MyBigInteger bak_this(*this);
#endif
		if (divisor == 0)
			throw std::exception("Divide by zero exception");
		if (divisor < 0)
		{
			m_negative = !m_negative;
			divisor = std::abs(divisor);
		}
		__int32 carry = 0;
		for (int i = m_size - 1; i >= 0; i--)
		{
			__int64 tmp = (__int64)m_digits[i] + (__int64)carry * (__int64)m_base;
			m_digits[i] = (__int32)(tmp / divisor);
			carry = tmp % divisor;
		}
		_normalize();
#ifdef LOG_CALCULATIONS
		std::cout << "operator /= (__int32): " << (std::string)(bak_this) << " / " << divisor << " = " << (std::string)(*this) << std::endl;
#endif
		return *this;
	};


	MyBigInteger operator / (unsigned __int32 divisor) {
		MyBigInteger result = *this;
		result /= divisor;
		return result;
	};

	MyBigInteger operator /= (unsigned __int32 divisor) {
#ifdef LOG_CALCULATIONS
		MyBigInteger bak_this(*this);
#endif
		if (divisor == 0)
			throw std::exception("Divide by zero exception");
		__int32 carry = 0;
		for (int i = m_size - 1; i >= 0; i--)
		{
			__int64 tmp = (__int64)m_digits[i] + (__int64)carry * (__int64)m_base;
			m_digits[i] = (__int32)(tmp / divisor);
			carry = tmp % divisor;
		}
		_normalize();
#ifdef LOG_CALCULATIONS
		std::cout << "operator /= (unsigned __int32): " << (std::string)(bak_this) << " / " << divisor << " = " << (std::string)(*this) << std::endl;
#endif
		return *this;
	};

	MyBigInteger operator / (const MyBigInteger& other) {
		MyBigInteger result = *this;
		result /= other;
		return result;
	};

	MyBigInteger operator /= (const MyBigInteger& other) {
#ifdef LOG_CALCULATIONS
		MyBigInteger bak_this(*this);
#endif
		if (other.isZero())
			throw std::exception("Divide by zero exception");
		if (other.m_size == 1)
		{
			__int32 f = other.m_digits[0];
			*this /= f;
#ifdef LOG_CALCULATIONS
			std::cout << "operator /= (bigint shortcut): " << (std::string)(bak_this) << " / " << (std::string)other << " = " << (std::string)(*this) << std::endl;
#endif
			return *this;
		}
		bool sign = false;
		bool changeSign = true;
		if (other._isAbsGreaterThan(*this))	// do not change sign of 0 outcome
			changeSign = false;
		sign = !(m_negative == other.m_negative);
		*this = _unsignedDiv(other);
		if (changeSign)
		{
			m_negative = sign;
		}
#ifdef LOG_CALCULATIONS
		std::cout << "operator /= (bigint): " << (std::string)(bak_this) << " / " << (std::string)other << " = " << (std::string)(*this) << std::endl;
#endif
		return *this;
	};

	MyBigInteger operator % (const MyBigInteger& other) {
		MyBigInteger result = *this;
		result %= other;
		return result;
	};

	MyBigInteger operator %= (const MyBigInteger& other) {
		if (other.isZero())
			throw std::exception("Divide by zero exception");
		if (other.m_size == 1)
		{
			__int32 f = other.m_digits[0];
			__int32 m = _unsignedModuloDigit(f);
			if (m_negative)
				m = -m;
			*this = MyBigInteger(m);
			return *this;
		}
		if (other.m_negative == this->m_negative)
		{
			*this = _unsignedMod(other);
			return *this;
		}
		else
		{
			if (!other._isAbsGreaterThan(*this))
			{
				m_negative = !(m_negative == other.m_negative);
				std::pair<MyBigInteger, MyBigInteger> divMod = _unsignedDivMod(other);
				*this = divMod.second;
			}
			return *this;
		}
	};


	operator std::string() const {
		std::stack<__int32> tmp;
		std::string result;
		char buf[101];
		bool first = true;
		if (m_negative)
			result.append("-");

		for (int i = m_size - 1; i >= 0; i--)
		{
			if (first)
			{
				snprintf(buf, 100, "%d", m_digits[i]);
				first = false;
			}
			else
			{
				snprintf(buf, 100, "%06d", m_digits[i]);
			}
			result.append(std::string(buf));
		}
		return result;
	};

	bool operator < (const MyBigInteger& other) const {
		return other > (*this);
	};

	bool operator > (const MyBigInteger& other) const {
		if (m_negative != other.m_negative)
		{
			return (!m_negative);
		}
		else
		{
			if (m_negative)
			{
				return other._isAbsGreaterThan(*this);
			}
			else
			{
				return _isAbsGreaterThan(other);
			}
		}
	};

	bool operator < (const __int32 val) const {
		if (val == 0)
			return m_negative;
		return *this < (MyBigInteger(val));
	};

	bool operator == (const MyBigInteger& other) const {
		if (m_negative != other.m_negative)
			return false;
		if (m_size != other.m_size)
			return false;
		for (int i = 0; i < m_size; i++)
		{
			if (m_digits[i] != other.m_digits[i])
				return false;
		}
		return true;
	};

	bool operator != (const MyBigInteger& other) const {
		return !(*this == other);
	};

	bool operator <= (const MyBigInteger& other) const {
		return (!(*this > other));
	};

	bool operator >= (const MyBigInteger& other) const {
		return (!(*this < other));
	};

	int sign() const
	{
		if (m_negative)
			return -1;
		else
			return 1;
	};

	void DebugPrintNumber()
	{
		std::stack<__int32> tmp;

		for (std::vector<__int32>::iterator it = m_digits.begin(); it != m_digits.end(); it++)
			tmp.push(*it);
		while (!tmp.empty())
		{
			printf("%06d ", tmp.top());
			tmp.pop();
		}
		printf("\n");
	}

	int GetDigitSum() const
	{
		int sz = (int)m_digits.size();
		int digitSum = 0;
		for (int i = 0; i < sz; i++)
		{
			__int32 rest = m_digits[i];
			while (rest != 0)
			{
				digitSum += rest % 10;
				rest /= 10;
			}
		}
		return digitSum;
	}

	bool inline isZero() const
	{
		if (m_digits[0] == 0)
		{
			return m_size == 1;
		}
		else
			return false;
	};

private:

	std::vector<__int32> m_digits;
	bool m_negative;
	int m_base;
	int m_size;

	const static int DEFAULT_BASE = 1000000;

	//friend class MyBigIntegerComperator;

	void _setSign(bool negative)
	{
		m_negative = negative;
	};

	__int32 _getDigit(int index) const
	{
		if (index >= m_size)
			return 0;
		else
			return m_digits[index];
	};

	bool _isAbsGreaterThan(const MyBigInteger& other) const
	{
		int ix1 = m_size;
		int ix2 = other.m_size;
		if (ix1 > ix2)
			return true;
		if (ix1 < ix2)
			return false;
		for (int i = ix1 - 1; i >= 0; i--)
		{
			if (m_digits[i] > other.m_digits[i])
				return true;
			if (m_digits[i] < other.m_digits[i])
				return false;
		}
		return false;
	};

	void _normalize() {
		while (m_size > 1 && m_digits[m_size - 1] == 0)
			m_size -= 1;
		if (m_size == 1 && m_digits[0] == 0)
			m_negative = false;
	};


	MyBigInteger _unsignedAdd(const MyBigInteger& other) {
		int ix1 = m_size;
		int ix2 = other.m_size;
		int maxIx = ix1 > ix2 ? ix1 : ix2;
		int carry = 0;
		int i = 0;
		for (i = 0; i < maxIx; i++)
		{
			__int32 otherVal = 0;
			__int32 thisVal = 0;
			if (i < ix2)
				otherVal = other.m_digits[i];
			if (i < ix1)
				thisVal = m_digits[i];
			int sum = thisVal + otherVal + carry;
			if (sum >= m_base)
			{
				carry = 1;
				sum -= m_base;
			}
			else
			{
				carry = 0;
			}
			if (i < ix1)
			{
				m_digits[i] = sum;
			}
			else
			{
				m_digits.push_back(sum);
				m_size += 1;
			}
		}
		if (carry != 0)
		{
			if (i < ix1)
			{
				m_digits[i] = carry;
			}
			else
			{
				m_digits.push_back(carry);
				m_size += 1;
			}
		}
		return *this;
	};

	MyBigInteger _unsignedSub(const MyBigInteger& other)
	{
		// precondition: abs(*this) >= abs(other)
		// ignore m_negative

		int ix1 = m_size;
		int ix2 = other.m_size;
		int maxIx = ix1 > ix2 ? ix1 : ix2;
		bool borrow = false;
		int i = 0;
		for (i = 0; i < maxIx; i++)
		{
			__int32 otherVal = 0;
			__int32 thisVal = 0;
			if (i < ix2)
				otherVal = other.m_digits[i];
			if (i < ix1)
				thisVal = m_digits[i];

			if (borrow)
			{
				if (otherVal + 1 == m_base)
				{
					otherVal = 0;
				}
				else
				{
					otherVal += 1;
					borrow = false;
				}

			}
			if (thisVal < otherVal)
			{
				borrow = true;
				thisVal += m_base;
			}

			int diff = thisVal - otherVal;
			m_digits[i] = diff;
		}

		_normalize();
		return *this;
	};

	MyBigInteger _unsignedMul(const MyBigInteger& other) {
		int ix1 = m_size;
		int ix2 = other.m_size;
		int maxIx = ix1 + ix2;
		std::vector<__int32> result;
		result.resize(maxIx);
		int i = 0;
		int j = 0;
		for (i = 0; i < ix1; i++)
		{
			__int32 carry = 0;
			__int32 thisVal = m_digits[i];
			for (j = 0; j < ix2; j++)
			{
				__int32 otherVal = other.m_digits[j];
				__int64 tmp = (__int64)thisVal * (__int64)otherVal + (__int64)result[i + j] + (__int64)carry;
				carry = (__int32)(tmp / m_base);
				__int32 newVal = tmp % m_base;
				result[i + j] = newVal;
			}
			for (int c = 0; carry != 0; c++)	// handle overflows
			{
				__int64 tmp = (__int64)(result[i + j + c]) + carry;
				carry = (__int32)(tmp / m_base);
				__int32 newVal = tmp % m_base;
				result[i + j + c] = newVal;
			}
		}
		m_digits = result;
		m_size = maxIx;
		while (m_size > 1 && m_digits[m_size - 1] == 0)
		{
			m_digits.pop_back();
			m_size -= 1;
		}
		return *this;
	};


	__int32 _unsignedModuloDigit (const __int32 digit) const {
		if (digit == 0)
			throw std::exception("Divide by zero exception");
		if (digit >= m_base)
			throw std::exception("abs(digit) >= base.");
		__int32 absDigit = digit;
		if (digit < 0)
		{
			absDigit = std::abs(digit);
		}
		__int32 carry = 0;
		for (int i = m_size - 1; i >= 0; i--)
		{
			__int64 tmp = (__int64)m_digits[i] + (__int64)carry * (__int64)m_base;
			carry = tmp % absDigit;
		}
		return carry;
	};


	MyBigInteger _unsignedDiv(const MyBigInteger& other)
	{
		return _unsignedDivMod(other).first;
	}

	MyBigInteger _unsignedMod(const MyBigInteger& other)
	{
		return _unsignedDivMod(other).second;
	}

	std::pair<MyBigInteger, MyBigInteger> _unsignedDivMod(const MyBigInteger& other)
	{
		std::pair<MyBigInteger, MyBigInteger> resultPair;

		int ix1 = m_size;
		int ix2 = other.m_size;
		if (ix1 < ix2)
		{
			resultPair.second = *this;
			m_digits.clear();
			m_size = 1;
			m_digits.push_back(0);
			resultPair.first = *this;
			return resultPair;
		}
		if (ix1 == ix2)
		{
			if (m_digits[ix1 - 1] < other.m_digits[ix1 - 1])
			{
				resultPair.second = *this;
				m_digits.clear();
				m_size = 1;
				m_digits.push_back(0);
				resultPair.first = *this;
				return resultPair;
			}
		}
		__int32 otherHighestDigit = other.m_digits[ix2 - 1];
		int ix3 = 1 + ix1 - ix2;
		__int64 carry = 0;
		std::vector<__int32> result(ix3, 0);
		MyBigInteger big_rest(*this);
		MyBigInteger big_tmpProduct;
		for (int i = 0; i < ix3 != 0; i++)
		{
			__int64 thisHighestDigit = (__int64)(big_rest._getDigit(ix1 - i - 1)) + (__int64)carry * (__int64)m_base;
			carry = 0;
			if (otherHighestDigit > thisHighestDigit)
			{
				result[ix3 - i - 1] = 0;
				carry = thisHighestDigit;
			}
			else
			{
				__int32 tmpFactor = (__int32)(thisHighestDigit / otherHighestDigit);
				MyBigInteger big_otherShifted = MyBigInteger(other)._lShiftMul(ix3 - i - 1, 1);
				MyBigInteger big_tmpProduct = big_otherShifted * (__int32)tmpFactor;
				// find tmpFactor with binary search
				if (big_tmpProduct._isAbsGreaterThan(big_rest))
				{
					__int32 factor_high = tmpFactor;
					__int32 factor_low = 0;

					while (factor_high - factor_low > 1)
					{
						__int32 factor_mid = (factor_high + factor_low) / 2;
						big_tmpProduct = big_otherShifted * (__int32)factor_mid;
						if (big_tmpProduct._isAbsGreaterThan(big_rest))
						{
							factor_high = factor_mid;
						}
						else
						{
							factor_low = factor_mid;
						}
					}
					tmpFactor = factor_low;
					if (tmpFactor == 0)
						carry = thisHighestDigit;
					big_tmpProduct = big_otherShifted * (__int32)tmpFactor;
				}
				result[ix3 - i - 1] = (__int32)tmpFactor;
				big_rest._unsignedSub(big_tmpProduct);
				carry = big_rest.m_digits[ix1 - i - 1];
			}
		}
		m_digits = result;
		m_size = ix3;

		while (m_size > 1 && m_digits[m_size - 1] == 0)
		{
			m_digits.pop_back();
			m_size -= 1;
		}
		resultPair.first = *this;
		resultPair.second = big_rest;
		return resultPair;
	};

	MyBigInteger _lShiftMul(int shift, __int32 digitFactor)
	{
		*this *= digitFactor;
		for (int i = 0; i < shift; i++)
		{
			m_digits.insert(m_digits.begin(), 0);
		}
		m_size += shift;

		return *this;
	};


};	// class MyBigInteger

//MyBigInteger std::abs(const MyBigInteger& value) const
//{
//	//return value < 0 ? -value : value;
//	return value;
//}


//class MyBigIntegerComperator
//{
//public:
//	MyBigIntegerComperator()
//	{}
//	bool operator() (const MyBigInteger& o1, const MyBigInteger& o2) const
//	{
//		int size1 = o1.m_size;
//		int size2 = o2.m_size;
//		if (size1 < size2)
//			return true;
//		if (size1 > size2)
//			return false;
//		if (o1.m_digits[size1-1] < o2.m_digits[size1-1])
//			return true;
//		else
//			return false;
//	}
//};

//int a = std::abs(-3);

//template<typename T>
//auto std::abs(const T& foo)
//{
//	return T(0);
//}

// specializations for std::abs for MyBigInteger
//
//template<typename MyBigInteger>
//MyBigInteger std::abs(const MyBigInteger& value)
//{
//	return value;
//}

	