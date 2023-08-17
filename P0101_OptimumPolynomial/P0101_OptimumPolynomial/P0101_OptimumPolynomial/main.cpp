// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <iostream>
#include <chrono>
#include <vector>
#include "fraction.h"
#include "matrix.h"
#include "MyBiginteger.h"

////typedef Fraction<__int64> frac;
//typedef Fraction<MyBigInteger> frac;
//typedef frac scalar_t;
////typedef double scalar_t;
//typedef Matrix<scalar_t> matr;

typedef MyBigInteger IntType;
typedef Fraction<IntType> frac;
typedef frac scalar_t;
typedef Matrix<scalar_t> matr;



IntType nPowk(IntType n, int k)
{
    IntType result = 1;
    for (int i = 0; i < k; i++)
    {
        result *= n;
    }
    return result;
}

template <class T> class Point {
public:
    Point() : x(0), y(0) {}
    Point(T x, T y) : x(x), y(y) {}
    Point(const Point& p) : x(p.x), y(p.y) {}
    T x;
    T y;
};

std::vector<Point<IntType>> sequence;

IntType func(IntType n)
{
    return IntType(1) - IntType(n) + nPowk(n, 2) - nPowk(n, 3) + nPowk(n, 4) - nPowk(n, 5) + nPowk(n, 6) - nPowk(n, 7) + nPowk(n, 8) - nPowk(n, 9) + nPowk(n, 10) + IntType(0);
    //return nPowk(n, 3);
}



void testMatrix()
{
    /*
    //matr m (5, 5, 0);
    //__int64 a = m(3, 4).numerator;

    //matr A(2, 2, std::vector<frac>(2, 2) = { 1, 2, 3, 4 });
    //A *= 2;
    //matr B(2, 2, std::vector<frac>(2, 2) = { 3, 4, 5, 7 });
    //matr C = A * B;
    //matr invA = A.inverse();

    ////matr D(3,3, std::vector<frac>(3, 3) = { 1, 2, 3, 3, 2, 1, 2, 1, 3 });
    matr D(3, 3, std::vector<frac>(3, 3) = { 1, 2, 3, 4, 5, 6, 7, 8, 1 });
    matr invD = D.inverse();
    //matr invinvD = invD.inverse();
    //matr one = invD * D;
    //matr one2 = D * invD;
    //matr G(3, 4, std::vector<frac>(12) = {1,3,1,9,1,1,-1,1,3,11,5,35});

    matr G(3, 4, std::vector<frac>(12) = { 2,1,-1,8,-3,-1,2,-11,-2,1,2,-3 });
    bool geSuccess = G.gaussElimimination();
    matr G2(3, 6, std::vector<frac>(12) = { 1, 2, 3,  1,0,0,   4, 5, 6,  0,1,0,   7, 8, 1,   0,0,1 });
    geSuccess = G2.gaussElimimination();
    matr G3(3, 6, std::vector<frac>(12) = { 2, -1, 0,  1,0,0,   -1, 2, -1,  0,1,0,   0, -1, 2, 0,0,1 });
    geSuccess = G3.gaussElimimination();

    matr subG2 = G2.getSubMatrix(0, 3, 3, 3);
    if (invD == subG2)
    {
        std::cout << "equal" << std::endl;
    }
    else
    {
        std::cout << "not equal" << std::endl;
    }
    */
}


IntType approxFunc(std::vector<IntType> v, IntType x)
{
    IntType tmpPow = 1;
    int len = v.size();
    IntType sum = 0;
    for (int i = 0; i < len; i++)
	{
        IntType coeff = v[i];
		sum += v[i] * tmpPow;
        tmpPow *= x;
	}
    return sum;
}

IntType getBOPn(int n)
{
    matr M(n, n+1, (scalar_t)0);
    for (int i = 0; i < n; i++)
    {
        for (int j=0;j<n;j++)
        {
            M.set(i, j, (scalar_t)nPowk(sequence[i].x, j));
		}
        M.set(i, n, (scalar_t)sequence[i].y);
    }
    //M.printMatrix();
    if (M.gaussElimimination())
    {
        //M.printMatrix();
    }
    else
        std::cout << "gaussElimimination failed" << std::endl;

    std::vector<scalar_t> v = M.getColumn(n);
    std::vector<IntType> iv;
    for (int i = 0; i < v.size(); i++)
    {
        scalar_t tmp = v[i];
        if (tmp.denominator != 1)
        {
            throw std::exception("denominator != 1");
            //std::cout << "denominator != 1" << std::endl;
        }
        iv.push_back(tmp.numerator);
    }

    // solution is in last column
    for (int i = 1; i <= n+1; i++)
    {
        IntType yApprox = approxFunc(iv, IntType(i));
        IntType seqEl = sequence[i-1].y;
        if (yApprox != seqEl)
        {
            //std::cout << "**" << (std::string)yApprox << std::endl;
            return yApprox;
            //std::cout << "approxFunc != sequence" << std::endl;
        }
        //std::cout << (std::string)yApprox << ", ";
    }
    return 0;
}


IntType solve()
{
    int polynomialDegree = 12;

    // init sequence with func
    for (int i = 1; i <= polynomialDegree+5; i++)
    {
        sequence.push_back(Point<IntType>(i, func(i)));
    }

    // print sequence
 //   for (int i = 0; i <= polynomialDegree+1; i++)
	//{
	//	std::cout << (std::string)sequence[i].x << " " << (std::string)sequence[i].y << std::endl;
	//}

    IntType sum = IntType(0);
    for (int i = 1; i < polynomialDegree + 3; i++)
    {
        sum += getBOPn(i);
    }


    return sum;
}

int main()
{

    auto t1 = std::chrono::high_resolution_clock::now();
    IntType solution = solve();
    auto t2 = std::chrono::high_resolution_clock::now();
    auto microSec = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
    std::cout << "solution: " << (std::string)solution << std::endl << "duration: " << microSec << " micro seconds (" << ms << " ms)" << std::endl;
}
