using System;
using Primes01_CS;

namespace perfTest01
{
    class Program
    {
        static void MeasurePrimeTest(IMyPrimes primeObj, int testCount, int primeRange)
        {
            var t0 = DateTime.UtcNow;

            Random r = new Random();
            int PrimeCount = 0;

            for (int i=0;i<testCount;i++)
            {
                Int32 num = r.Next(2, primeRange);
                if (primeObj.IsPrime(num))
                    PrimeCount += 1;
            }

            var t2 = DateTime.UtcNow;

            Console.WriteLine("primeTests: {0}, primeRange: {1}, primeCount: {2}", testCount, primeRange, PrimeCount);
            Console.WriteLine("duration: {0} ms.", t2.Subtract(t0).TotalMilliseconds);

        }

        static void MeasurePrime2Test(IMyPrimes primeObj, int testCount, int primeRange)
        {
            var t0 = DateTime.UtcNow;

            Random r = new Random();
            int PrimeCount = 0;

            for (int i = 0; i < testCount; i++)
            {
                Int32 num = r.Next(2, primeRange);
                if (primeObj.IsPrime2(num))
                    PrimeCount += 1;
            }

            var t2 = DateTime.UtcNow;

            Console.WriteLine("primeTests: {0}, primeRange: {1}, primeCount: {2}", testCount, primeRange, PrimeCount);
            Console.WriteLine("duration: {0} ms.", t2.Subtract(t0).TotalMilliseconds);

        }

        static void suite1(int testCount, int primeRange)
        {
            Console.WriteLine("---------- class Primes01_CS.IMyPrimes ----------");
            Primes01_CS.IMyPrimes p;
            p = new Primes01_CS.MyPrimes();
            for (int i = 0; i < 10; i++)
            {
                MeasurePrimeTest(p, testCount, primeRange);
            }

            Console.WriteLine("---------- class Primes01_CS.SortedSet ----------");

            p = new Primes01_CS.MyPrimes_SortedSet();
            for (int i = 0; i < 10; i++)
            {
                MeasurePrimeTest(p, testCount, primeRange);
            }

            Console.WriteLine("---------- class Primes01_CS.Bitset ----------");

            p = new Primes01_CS.MyPrimes_Bitset(primeRange);
            for (int i = 0; i < 10; i++)
            {
                MeasurePrimeTest(p, testCount, primeRange);
            }

            Console.WriteLine("---------- class Primes01_CS.Bitset.IsPrime2() ----------");

            p = new Primes01_CS.MyPrimes_SortedSet();
            for (int i = 0; i < 10; i++)
            {
                MeasurePrime2Test(p, testCount, primeRange);
            }
        }

        static void Main(string[] args)
        {
            //Primes01_CS.MyPrimes p = new MyPrimes();
            //Primes01_CS.MyPrimes_SortedSet p = new Primes01_CS.MyPrimes_SortedSet();

            suite1(100000, 1000000);
            suite1(100000, 10000000);

            //p.PrintPrime(13);
            //p.PrintPrime(1001);
            //p.PrintPrime(23);
            //p.PrintPrime(29);
            //p.PrintPrime(100);
            //p.PrintPrime(101);
            //p.PrintStoredPrimes();
        }
    }
}


