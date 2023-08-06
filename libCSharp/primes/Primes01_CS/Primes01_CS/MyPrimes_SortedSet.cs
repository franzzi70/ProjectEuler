using System;
using System.Collections;
using System.Collections.Generic;

namespace Primes01_CS
{
    public class MyPrimes_SortedSet : IMyPrimes
    {
        SortedSet<Int32> m_primes = new SortedSet<Int32>();
        Int32 m_HighestPrime;

        public MyPrimes_SortedSet()
        {
            m_primes.Add(2);
            m_primes.Add(3);
            m_primes.Add(5);
            m_primes.Add(7);
            m_HighestPrime = 7;
        }
        ~MyPrimes_SortedSet()
        {

        }

        private void AddPrimes(Int32 overNum)
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
                    foreach (Int32 prime in m_primes)
                    {
                        //Int32 prime = (Int32)m_primes[i];
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

        private bool FindPrime(Int32 n)
        {
            return m_primes.Contains(n);
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
            foreach (Int32 prime in m_primes)
            {
                Console.WriteLine("{0}", prime);
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

}
