using System;
using System.Numerics;

namespace P65_ConvergentsOfE_CSharp_V2
{

    public class fraction
    {
        public fraction(BigInteger numerator, BigInteger denominator)
        {
            _num = numerator;
            _denom = denominator;
        }
        public BigInteger _num;
        public BigInteger _denom;
    };



    class Program
    {
        static int[] nums = new int[100];

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

        static fraction calcFract(int n)
        {
            fraction accu = new fraction(nums[n - 1], 1);
            for (int i = n - 1; i >= 1; --i)
            {
                // kehrwert
                BigInteger tmp = accu._num;
                accu._num = accu._denom;
                accu._denom = tmp;

                // auf gleichen nenner bringen und addieren
                BigInteger Num1 = nums[i - 1] * accu._denom + accu._num;
                BigInteger Denom1 = accu._denom;
                // kuerzen
                BigInteger _ggt = ggt(Num1, Denom1);
                // int _ggt = 1;
                accu._num = Num1 / _ggt;
                accu._denom = Denom1 / _ggt;
            }
            return accu;
        }

        static void initNums()
        {
            nums[0] = 2;
            int n;
            for (int i = 2, k = 1; i <= 100; i++)
            {
                if (i % 3 == 0)
                {
                    n = k * 2;
                    k += 1;
                }
                else
                    n = 1;

                nums[i - 1] = n;
            }
        }

        static int digitSum(BigInteger n)
        {
            int sum = 0;
            BigInteger q = n;
            while (q != 0)
            {
                sum += (int)(q % 10);
                q = q / 10;
            }
            return sum;
        }

        static int solve()
        {
            initNums();

            //fraction result = calcFract(10);
            fraction result = calcFract(100);

            return digitSum(result._num);

        }            

        static void Main(string[] args)
        {
            int digitSum = solve();

            Console.WriteLine("digit sum = {0}", digitSum);
        }
    }
}
