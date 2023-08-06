using System;
using System.Numerics;
using System.Collections;


namespace P0078_CoinPartitions_CSharp
{
    class Program
    {
        const int MAXNUM = 100000;
        static ArrayList memArray = new ArrayList();

        static int GetIndex(int n)
        {
            // using Eulers ruler shown in https://en.wikipedia.org/wiki/Partition_function_(number_theory)
            // calculated (and simplified terms) the jumpOffsets for even and odd cases, maybe more elegant way possible.
            int jumpSize;
            if (n % 2 == 0)
            {
                jumpSize = n * (3 * n  + 2) / 8;
            }
            else
            {
                jumpSize = (n * (3 * n + 4)  + 1) / 8;
            }
            return jumpSize;
        }

        static BigInteger P(int n)
        {
            if (n < 2)
            {
                BigInteger val = 1;
                if (memArray.Count == n)
                    memArray.Add(val);
                return val;
            }

            if (memArray.Count > n)
            {
                return (BigInteger)memArray[n];
            }

            //if (n <= 2)
            //    return 1;

            BigInteger sum = 0;

            int ix = 0;

            for (int i = 1; (ix = GetIndex(i)) <= n; i += 1)
            {
                BigInteger p = P(n - ix);
                if ((i - 1) % 4 < 2)
                {
                    sum += p;
                }
                else
                {
                    sum -= p;
                }
            }

            if (memArray.Count == n)
                memArray.Add(sum % 1000000);    // modulo 1000000 can be used !

            return sum;
        }

        static void Test()
        {
            for (int i = 0; i < 20; i++)
            {
                Console.WriteLine("{2} x: {0} -> offset: {1} {3}", i, GetIndex(i), i % 2 == 0 ? "*" : " ", i % 2 != 0 ? "*" : " ");
            }
            int n;
            n = 2;
            Console.WriteLine("P({0}) = {1}", n, P(n));
            n = 4;
            Console.WriteLine("P({0}) = {1}", n, P(n));
            n = 5;
            Console.WriteLine("P({0}) = {1}", n, P(n));
            n = 10;
            Console.WriteLine("P({0}) = {1}", n, P(n));
            n = 40;
            Console.WriteLine("P({0}) = {1}", n, P(n));
        }

        static BigInteger Solve()
        {
            for (int i = 1; i < MAXNUM; i++)
            {
                BigInteger p = P(i);
                if (p % 1000000 == 0)
                {
                    Console.WriteLine("{0}", p);
                    return i;
                }
                if (i % 10000 == 0)
                    Console.WriteLine("{0}", i);
            }
            return 0;
        }

        static void Main(string[] args)
        {
            // Test();
            BigInteger solution = Solve();
            Console.WriteLine("Solution: {0}", solution);
        }
    }
}
