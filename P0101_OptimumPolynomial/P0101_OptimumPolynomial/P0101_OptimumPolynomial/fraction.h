#pragma once

#include <string>

template <class T> class Fraction {
public:
	enum class ReduceMode : char {
		Auto,
		Manual
	};
	Fraction<T>() : numerator(0), denominator(1), reduceMode(ReduceMode::Auto), reduced(false) {};
	Fraction<T>(const T& value) : numerator(value), denominator(1), reduceMode(ReduceMode::Auto), reduced(false) {};
	Fraction<T>(const T& numerator, const T& denominator, ReduceMode reduceMode = ReduceMode::Auto) : numerator(numerator), denominator(denominator), reduceMode(reduceMode), reduced(false) {
		if (reduceMode == ReduceMode::Auto)
		{
			reduce();
		}
	};
	Fraction<T>(const Fraction<T>& other) : numerator(other.numerator), denominator(other.denominator), reduceMode(other.reduceMode), reduced(other.reduced) {};
	Fraction<T> operator + (const Fraction<T>& other) {
		if (denominator == other.denominator)
		{
			return Fraction<T>(numerator + other.numerator, denominator, reduceMode);
		}
		else
		{
			return Fraction<T>(numerator * other.denominator + other.numerator * denominator, denominator * other.denominator, reduceMode);
		}
	};
	Fraction<T> operator - (const Fraction<T>& other) {
		if (denominator == other.denominator)
		{
			return Fraction<T>(numerator - other.numerator, denominator, reduceMode);
		}
		else
		{
			return Fraction<T>(numerator * other.denominator - other.numerator * denominator, denominator * other.denominator, reduceMode);
		}
	};
	Fraction<T> operator * (const Fraction<T>& other) {
		return Fraction<T>(numerator * other.numerator, denominator * other.denominator, reduceMode);
	};
	Fraction<T> operator / (const Fraction<T>& other) {
		return Fraction<T>(numerator * other.denominator, denominator * other.numerator, reduceMode);
	};
	Fraction<T> operator - () {
		return Fraction<T>(-numerator, denominator, reduceMode);
	};
	Fraction<T> operator + () {
		return Fraction<T>(numerator, denominator, reduceMode);
	};
	Fraction<T> operator = (const T& other) {
		numerator = other;
		denominator = 1;
		reduceMode = ReduceMode::Auto;
		return *this;
	};

	//Fraction<T> operator ++ () {
	//	numerator += denominator;
	//	return *this;
	//};
	//Fraction<T> operator -- () {
	//	numerator -= denominator;
	//	return *this;
	//};
	bool operator < (const Fraction<T>& other) {
		return numerator * other.denominator < other.numerator * denominator;
	};
	bool operator > (const Fraction<T>& other) {
		return numerator * other.denominator > other.numerator * denominator;
	};
	bool operator <= (const Fraction<T>& other) {
		return numerator * other.denominator <= other.numerator * denominator;
	};
	bool operator >= (const Fraction<T>& other) {
		return numerator * other.denominator >= other.numerator * denominator;
	};
	bool operator == (const Fraction<T>& other) {
		return numerator * other.denominator == other.numerator * denominator;
	};
	bool operator != (const Fraction<T>& other) {
		return (T) numerator * other.denominator != (T) other.numerator * denominator;
	};
	Fraction<T> operator = (const Fraction<T>& other) {
		numerator = other.numerator;
		denominator = other.denominator;
		return *this;
	};
	Fraction<T> operator += (const Fraction<T>& other) {
		if (denominator == other.denominator)
		{
			numerator += other.numerator;
			reduced = false;
		}
		else
		{
			numerator = (T) numerator * other.denominator + (T)other.numerator * denominator;
			denominator *= other.denominator;
			reduced = false;
		}
		if (reduceMode == ReduceMode::Auto)
		{
			reduce();
		}
		return *this;
	};
	Fraction<T> operator -= (const Fraction<T>& other) {
		if (denominator == other.denominator)
		{
			numerator -= other.numerator;
			reduced = false;
		}
		else
		{
			numerator = (T) numerator * other.denominator - (T) other.numerator * denominator;
			denominator *= other.denominator;
			reduced = false;
		}
		if (reduceMode == ReduceMode::Auto)
		{
			reduce();
		}
		return *this;
	};
	Fraction<T> operator *= (const Fraction<T>& other) {
		numerator *= other.numerator;
		denominator *= other.denominator;
		reduced = false;
		if (reduceMode == ReduceMode::Auto)
		{
			reduce();
		}
		return *this;
	};

	//operator std::string () const {
	//		return std::string(std::to_string(numerator)).append(1, '/').append(std::to_string(denominator));
	//};
	
	operator std::string() const {
		return std::string((std::string)numerator).append(1, '/').append((std::string)denominator);
		//return std::to_string(numerator).append(1, '/').append(std::to_string(denominator));
	};

	Fraction<T> operator /= (const Fraction<T>& other) {
		numerator *= other.denominator;
		denominator *= other.numerator;
		reduced = false;
		if (reduceMode == ReduceMode::Auto)
		{
			reduce();
		}
		return *this;
	};
	Fraction<T>getReduced()
	{
		Fraction<T> v (numerator , denominator);
		v.reduce();
		return v;
	};

	T ggt(T a, T b)
	{
		T _a = a < 0 ? -a : a;
		T _b = b < 0 ? -b : b;
		T r;
		do {
			r = _a % _b;
			if (r == 0)
			{
				// std::cout << "ggt(" << a << "," << b << ") = " << _b << std::endl;
				return _b;
			}

			_a = _b;
			_b = r;
		} while (true);
	}


	void reduce()
	{
		if (reduced)
		{
			return;
		}
		T a = ggt(numerator, denominator);
		numerator /= a;
		denominator /= a;
		if (denominator < 0)	// denominator should always be positive
		{
			numerator = -numerator;
			denominator = -denominator;
		}
		reduced = true;
	};
	T numerator;
	T denominator;

	bool reduced = false;
	ReduceMode reduceMode;
};


