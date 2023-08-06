using System;
using System.Numerics;

namespace Problem57
{
    class Program
    {

        static int numdigits (BigInteger n, int calcMode = 1)
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

                    if (numdigits(num+denom, calcMode) > numdigits(denom, calcMode))
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
            expand(1000,1);
            var tm0 = DateTime.UtcNow;

            var tm2 = DateTime.UtcNow;

            Console.WriteLine("duration:{0} ms.", tm2.Subtract(tm0).TotalMilliseconds);

        }
    }
}
