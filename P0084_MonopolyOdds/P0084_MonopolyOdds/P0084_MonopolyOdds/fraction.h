#pragma once


template <class T> class Fraction {
public:
	enum class ReduceMode : char {
		Auto,
		Manual
	};
	Fraction<T>() : numerator(0), denominator(1), reduceMode(ReduceMode::Auto), reduced(false) {};
	Fraction<T>(T value) : numerator(value), denominator(1), reduceMode(ReduceMode::Auto), reduced(false) {};
	Fraction<T>(T numerator, T denominator, ReduceMode reduceMode = ReduceMode::Auto) : numerator(numerator), denominator(denominator), reduceMode(reduceMode), reduced(false) {
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
			numerator = numerator * other.denominator + other.numerator * denominator;
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
			numerator = numerator * other.denominator - other.numerator * denominator;
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
		T a = numerator;
		T b = denominator;
		while (b != 0)
		{
			T t = b;
			b = a % b;
			a = t;
		}
		Fraction<T> v (numerator / a, denominator / a);
		v.reduce();
		return v;
	};
	void reduce()
	{
		if (reduced)
		{
			return;
		}
		T a = numerator;
		T b = denominator;
		while (b != 0)
		{
			T t = b;
			b = a % b;
			a = t;
		}
		numerator /= a;
		denominator /= a;
		reduced = true;
	};
	T numerator;
	T denominator;

	bool reduced = false;
	ReduceMode reduceMode;

};


