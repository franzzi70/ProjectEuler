using System;
using System.Collections;

namespace Problem58
{
    class MyPrimes
    {
        ArrayList m_primes = new ArrayList();
        Int32 m_HighestPrime;

        public MyPrimes()
        {
            Int32 a;
            a = 2;
            m_primes.Add((object)a);
            a = 3;
            m_primes.Add((object)a);
            a = 5;
            m_primes.Add((object)a);
            a = 7;
            m_primes.Add((object)a);
            m_HighestPrime = 7;
        }
        ~MyPrimes()
        {

        }

        private void AddPrimes (Int32 overNum)
        {
            // add primes including one number > overNum
            if (m_HighestPrime > overNum)
            {
                return; // end condition already met
            }
            if (m_HighestPrime * m_HighestPrime <= overNum)
            {
                AddPrimes((Int32)Math.Ceiling(Math.Sqrt(overNum)));
            }
            bool done = false;
            Int32 testNum = m_HighestPrime + 2;
            while (!done)
            {
                while (!done)
                {
                    bool IsPrime = true;
                    for (int i = 0; i < m_primes.Count; i++)
                    {
                        Int32 prime = (Int32)m_primes[i];
                        if ((testNum % prime) == 0)
                        {
                            IsPrime = false;
                            break;
                        }
                        if ((prime * prime) >= testNum)
                        {
                            IsPrime = true;
                            break;
                        }
                    }
                    if (IsPrime)
                    {
                        m_primes.Add(testNum);
                        m_HighestPrime = testNum;

                        if (testNum > overNum)
                            done = true;
                    }

                    testNum += 2;
                }
            }
        }

        private bool FindPrime (Int32 n)
        {
            // check with stored primes.
            // find prime using binary search:
            int ix_low = 0;
            int ix_high = m_primes.Count - 1;
            int ix_mid;
            Int32 num_low, num_high, num_mid;

            while (true)
            {

                num_low = (Int32)m_primes[ix_low];
                if (num_low == n)
                {
                    return true;
                }
                num_high = (Int32)m_primes[ix_high];
                if (num_high == n)
                {
                    return true;
                }

                if (ix_low >= ix_high)
                {
                    return false;
                }

                ix_mid = (ix_low + ix_high) / 2;
                num_mid = (Int32)m_primes[ix_mid];
                if (num_mid == n)
                {
                    return true;
                }

                if (num_mid > n)
                {
                    ix_high = ix_mid - 1;
                    ix_low += 1;
                }
                else
                {   // num_mid < n
                    ix_low = ix_mid + 1;
                    ix_high -= 1;
                }
            }
        }

        public bool IsPrime(Int32 n)
        {
            if (m_HighestPrime >= n)
            {
                return FindPrime(n);
            }
            else
            {   // extend primes arry first, then call IsPrime again.

                AddPrimes(n);
                return FindPrime(n);
            }
        }

        public void PrintPrime(int n)
        {
            Console.WriteLine("number {0} prime: {1}", n, IsPrime(n));
        }

        public void PrintStoredPrimes()
        {
            for (int i=0;i<m_primes.Count;i++)
            {
                Console.WriteLine("{0}", m_primes[i]);
            }
        }

        // alternative prime test with brute force calc:
        public bool IsPrime2(Int32 n)
        {

            switch (n)
            {
                case 2:
                case 3:
                case 5:
                case 7:
                    return true;
            }

            if ((n % 2) == 0)
            {
                return false;
            }

            Int32 upperBound = (Int32)Math.Ceiling(Math.Sqrt(n));
            for (Int32 i = 3; i <= upperBound; i += 2)
            {
                if ((n % i) == 0)
                {
                    return false;
                }
            }
            return true;
        }

    }


    class Program
    {
        static void solve()
        {
            MyPrimes p = new MyPrimes();

            int primeCount = 0;
            int innerSize = 1;
            Int32 testNum = 1;
            while (true)
            {
                int newSize = innerSize + 2;
                // one right
                testNum += 1;
                // InnerSize up -> corner
                testNum += innerSize;
                if (p.IsPrime2(testNum))
                {
                    primeCount += 1;
                }
                // (innerSize+1) left -> corner
                testNum += (innerSize + 1);
                if (p.IsPrime2(testNum))
                {
                    primeCount += 1;
                }
                // (innerSize+1) down -> corner
                testNum += (innerSize + 1);
                if (p.IsPrime2(testNum))
                {
                    primeCount += 1;
                }
                // (innerSize+1) right -> corner
                testNum += (innerSize + 1);
                if (p.IsPrime2(testNum))
                {
                    primeCount += 1;
                }

                // end of round

                int diagCount = (newSize / 2) * 4 + 1;
                double r = (double)primeCount / (double)diagCount;

                innerSize = newSize;


                // Console.WriteLine("lastNumber: {0}, prime count {1}, diagCount: {2}, ratio:{3}, side len:{4}.", testNum, primeCount, diagCount, r, newSize);

                if (r < (double)0.1 /*|| testNum >= 50000000*/)
                {
                    Console.WriteLine("lastNumber: {0}, prime count {1}, diagCount: {2}, ratio:{3}, side len:{4}.", testNum, primeCount, diagCount, r, newSize);
                    // p.PrintStoredPrimes();
                    break;
                }

            }

        }




        static void Main(string[] args)
        {
            MyPrimes p = new MyPrimes();

            var t0 = DateTime.UtcNow;

            solve();

            var t2 = DateTime.UtcNow;

            Console.WriteLine("duration: {0} ms.", t2.Subtract(t0).TotalMilliseconds);

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
