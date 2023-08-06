/*
using System;
using System.Numerics;

namespace Problem57
{
    class Program
    {

        static int numdigits(BigInteger n, int calcMode = 1)
        {
            if (calcMode == 1)
            {
                return n.ToString().Length;
            }
            else
            {
                BigInteger num = n;
                int digitCount = 0;
                while (num > 0)
                {
                    num /= 10;
                    digitCount += 1;
                }
                return digitCount;
            }
        }

        static void expand(int n, int calcMode = 1)
        {
            int counter = 0;
            BigInteger num = 1;
            BigInteger denom = 2;

            if (n == 1)
            {
                num = 3;
                denom = 2;
            }
            else
            {
                for (int i = 0; i < n - 1; i++)
                {
                    BigInteger v = 2 * denom + num;
                    num = denom;
                    denom = v;

                    if (numdigits(num + denom, calcMode) > numdigits(denom, calcMode))
                    {
                        counter += 1;
                    }
                }
                num += denom;
                // Console.WriteLine("{0} / {1}", num, denom);
            }
            Console.WriteLine("digits of numerator > digits of denominator: {0} times.", counter);
        }

        static void Main(string[] args)
        {
            expand(1000, 1);
            var tm0 = DateTime.UtcNow;

            Console.WriteLine("Hello World!");

            var tm2 = DateTime.UtcNow;

            Console.WriteLine("duration:{0} ms.", tm2.Subtract(tm0).TotalMilliseconds);

        }
    }
}
*/

#include <iostream>
#include <vector>
#include <string>

class BigInteger
{
private:
    std::vector<__int32> m_arr();
public:
    BigInteger(int n)
    {

    }
    BigInteger(BigInteger &n)
    {
        
    }

    const std::string ToString()
    {

    }

    BigInteger& Mult(int n)
    {
        return *this;
    }

    BigInteger& Add(BigInteger n)
    {
        return *this;
    }

    void Div(int n)
    {
        
    }

    bool GT(int n)
    {
        return false;
    }
};

static int numdigits(BigInteger n, int calcMode = 1)
{
    if (calcMode == 1)
    {
        return n.ToString().size();
    }
    else
    {
        BigInteger num = n;
        int digitCount = 0;
        while (num.GT (0))
        {
            num.Div (10);
            digitCount += 1;
        }
        return digitCount;
    }
}

static void expand(int n, int calcMode = 1)
{
    int counter = 0;
    BigInteger num = 1;
    BigInteger denom = 2;

    if (n == 1)
    {
        num = 3;
        denom = 2;
    }
    else
    {
        for (int i = 0; i < n - 1; i++)
        {
            BigInteger v = BigInteger(denom).Mult(2).Add(num);
            num = denom;
            denom = v;

            if (numdigits(BigInteger(num).Add(denom), calcMode) > numdigits(denom, calcMode))
            {
                counter += 1;
            }
        }
        num.Add(denom);
        // Console.WriteLine("{0} / {1}", num, denom);
    }
    std::cout << "digits of numerator > digits of denominator: " << counter << " times." << std::endl;
}

int main(int argc, char* argv[])
{
    // not ready yet, cSharp code (see above) not fully ported yet.
    return 0;
}