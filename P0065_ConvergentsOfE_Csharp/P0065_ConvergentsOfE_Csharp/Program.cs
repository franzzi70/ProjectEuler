using System;
using System.Numerics;

namespace P0065_ConvergentsOfE_Csharp
{


    class Program
    {

        static BigInteger ggt(BigInteger a, BigInteger b)
        {
            BigInteger _a = a, _b = b;
            BigInteger r;
            do
            {
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

        static BigInteger kgv(BigInteger a, BigInteger b)
        {
            return (a / ggt(a, b)) * b;
        }

        static BigInteger getFactor(int index)
        {
            // e = [2; 1, 2, 1, 1, 4, 1, 1, 6, 1, ... , 1, 2k, 1, ...] als kettenbruch.
            if (1 == index)
                return 2;
            else
            {
                if (0 == index % 3)
                {
                    return 2 * (index / 3);
                }
                else
                    return 1;
            }
        }

        static (BigInteger, BigInteger) approxERecursive(int index, (BigInteger, BigInteger) tail)
        {
            BigInteger factor = getFactor(index);
            BigInteger tail_num = tail.Item1;
            BigInteger tail_denom = tail.Item2;
            BigInteger new_num = factor * tail_denom + tail_num;
            BigInteger _ggt = ggt(tail_denom, new_num);

            if (1 == index)
            {
                return (new_num / _ggt, tail_denom / _ggt);
            }
            else
            {
                return approxERecursive(index - 1, (tail_denom / _ggt, new_num / _ggt));
            }

        }

        static (BigInteger, BigInteger) approxE(int n)
        {
            return approxERecursive(n, (0, 1));
        }

        static int solve()
        {
            int sum = 0;
            (BigInteger, BigInteger) frac = approxE(100);
            BigInteger frac_num = frac.Item1;
            BigInteger frac_denom = frac.Item2;
            Console.WriteLine("{0} / {1} approx. {2}", frac_num, frac_denom, (double)frac_num / (double)frac_denom);

            BigInteger x = frac_num;
            while (x > 0)
            {
                int digit = (int)(x % 10);
                sum += digit;
                x /= 10;
            }
            return sum;
        }

        static void Main(string[] args)
        {
            Console.WriteLine("{0}", solve());
        }
    }
}
