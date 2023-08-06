using System;
using System.Collections.Generic;
using Primes01_CS;

namespace P60_PrimePairSet
{
    class Primes
    {
        static MyPrimes_Bitset global_primes = new MyPrimes_Bitset(10000000000);

        public static bool IsPrime(int n)
        {
            return global_primes.IsPrime(n);
        }

        public static Int32 NextPrime(Int32 n)
        {
            if (n == 2)
                return 3;
            Int32 testNum = n + 2;
            while (!IsPrime(testNum))
            {
                testNum += 2;
            }
            return testNum;
        }
    }


    class PrimeSetNode
    {
        public PrimeSetNode(int[] primes)
        {
            m_match = false;
            m_sum = 0;
            m_primes = new int[5];
            byte i = 0;
            for (i = 0; i < 5; i++)
            {
                int p = primes[i];
                m_primes[i] = p;
                if (p == 0)
                {
                    {
                        for (byte t = i; t < 5; t++)   // rest primes filled with 0
                        {
                            m_primes[t] = 0;
                        }
                    }
                    break;
                }
                m_sum += p;
            }
            m_numPrimes = i;   // 0 terminated prime list

            if (m_numPrimes < 5)
            {
                m_h = m_sum + (primes[m_numPrimes - 1] + 2) * (5 - m_numPrimes);
            }
            else
                m_h = m_sum;
            m_match = _TestPrimesConcat();
        }


        // determine match criteria
        private bool _TestPrimes() // (SortedSet<Int64> nonPairs)
        {
            string[] arrPrimeStr = new string[5];
            byte numPrimes = m_numPrimes;
            if (numPrimes < 2)
                return false;
            for (int ix = 0; ix < numPrimes; ix++)
                arrPrimeStr[ix] = m_primes[ix].ToString();
            // only check the last number if compatible to all previous ones
            // it is assumed that primes before that have been checked between each other already.
            if (numPrimes == 2)
            {
                string p1 = arrPrimeStr[0];
                string p2 = arrPrimeStr[1];
                if (
                    (!Primes.IsPrime(Int32.Parse(p1 + p2)))
                    ||
                    (!Primes.IsPrime(Int32.Parse(p2 + p1)))
                    )
                    return false;
                else
                    return true;
            }
            if (numPrimes >2)
            {
                for (int i = 0; i < numPrimes - 1; i++)
                {
                    int lastIx = numPrimes - 1;
                    string p1 = arrPrimeStr[i];
                    string p2 = arrPrimeStr[lastIx];
                    if (
                        (!Primes.IsPrime(Int32.Parse(p1 + p2)))
                        ||
                        (!Primes.IsPrime(Int32.Parse(p2 + p1)))
                        )
                    {
                        //nonPairs.Add(
                        //    ((Int64)n.m_primes[i]) << 32 + n.m_primes[lastIx]);
                        return false;
                    }

                }
                return true;
            }
            return false;
        }


        private bool _TestPrimesConcat() // (SortedSet<Int64> nonPairs)
        {
            byte numPrimes = m_numPrimes;
            if (numPrimes < 2)
                return false;
            // only check the last number if compatible to all previous ones
            // it is assumed that primes before that have been checked between each other already.
            if (numPrimes == 2)
            {
                int p1 = m_primes[0];
                int p2 = m_primes[1];
                if (
                    (!Primes.IsPrime(ConcatNumbers(p1,p2)))
                    ||
                    (!Primes.IsPrime(ConcatNumbers(p2, p1)))
                    )
                    return false;
                else
                    return true;
            }
            if (numPrimes > 2)
            {
                for (int i = 0; i < numPrimes - 1; i++)
                {
                    int lastIx = numPrimes - 1;
                    int p1 = m_primes[i];
                    int p2 = m_primes[lastIx];
                    if (
                        (!Primes.IsPrime(ConcatNumbers(p1, p2)))
                        ||
                        (!Primes.IsPrime(ConcatNumbers(p2 , p1)))
                        )
                    {
                        //nonPairs.Add(
                        //    ((Int64)n.m_primes[i]) << 32 + n.m_primes[lastIx]);
                        return false;
                    }

                }
                return true;
            }
            return false;
        }

        private int ConcatNumbers(int n1, int n2)
        {
            int f = 10;
            while (n2 >= f)
            {
                f *= 10;
            }
            return n1 * f + n2;
        }

        public Int64 m_h;
        public int m_sum;
        public byte m_numPrimes;
        public int[] m_primes;
        public bool m_match;
    }

    class PrimesetPrioQueue
    {
        SortedList<Int64, PrimeSetNode> m_prioQueue;
        Int64 m_salt = 0;
        Int64 m_getFirstCount = 0;

        public PrimesetPrioQueue()
        {
            m_prioQueue = new SortedList<Int64, PrimeSetNode>();
        }

        public void Add(PrimeSetNode n)
        {
            Int64 newSalt = (m_salt + 1) % 100000000;
            Int64 newKey = m_salt + ((Int64)n.m_h) * (Int64)100000000;
            //Int64 newKey = m_salt + ((Int64)n.m_sum) * (Int64)100000000;
            m_prioQueue.Add(newKey, n);
            m_salt = newSalt;
        }

        public PrimeSetNode GetFirst()
        {
            m_getFirstCount += 1;
            Int64 key = m_prioQueue.Keys[0];
            PrimeSetNode val = m_prioQueue[m_prioQueue.Keys[0]];
            if (!m_prioQueue.Remove(key))
            {
                throw new System.Exception("could not remove element from PrioQueue");
            }
            return val;
        }

        public int ItemCount
        {
            get { return m_prioQueue.Count; }
        }
    }


    class Program
    {

        static string PrimesToString(Int32[]primes)
        {
            int l = primes.Length;
            string str = "";
            for (int i=0;i<l;i++)
            {
                str = str + primes[i].ToString() + ",";
            }
            return str;
        }

        static SortedSet<UInt64> global_used2 = new SortedSet<UInt64>();
        static int global_collissions = 0;
        static int global_checked = 0;


        static bool InNonPairs(int[] nodePrimes, SortedSet<Int64> nonPairs)
        {
            for (int i = 0; i < 4; i++)
            {
                for (int j = i; j < 5; j++)
                {
                    if (nonPairs.Contains(((Int64)nodePrimes[i]) << 32 + nodePrimes[j]))
                    {
                        return true;
                    }
                }
            }
            return false;
        }

        static void Expand (PrimesetPrioQueue q, PrimeSetNode n, Dictionary<string, int> dict,
            SortedSet<Int64> nonPairs)
        {
            // try to set each prime to next prime, expand whole set if possible.
            byte numPrimes = n.m_numPrimes;
            Int32 maxPrime = 0;
            int[] nodePrimes = new int[5];
            Array.Copy(n.m_primes, nodePrimes, 5);
            for (byte i = 0; i < numPrimes; i ++)
                if (nodePrimes[i] > maxPrime)
                    maxPrime = nodePrimes[i];

            int nextOfMaxPrime = Primes.NextPrime(maxPrime);


            if (numPrimes > 2)
            {
                // increase last prime
                if (!(n.m_match))
                {
                    int tmp = nodePrimes[numPrimes - 1];
                    nodePrimes[numPrimes - 1] = nextOfMaxPrime;
                    q.Add(new PrimeSetNode(nodePrimes));
                    nodePrimes[numPrimes - 1] = tmp;    // restore array
                }
            }

            // extend matching combination
            if (numPrimes > 1 && numPrimes < 5 && n.m_match)
            {
                int tmp = nodePrimes[numPrimes];
                nodePrimes[numPrimes] = nextOfMaxPrime;
                q.Add(new PrimeSetNode(nodePrimes));
                nodePrimes[numPrimes] = tmp; // restore array;
            }

            // generate root tuples of two primes
            if (numPrimes == 2)
            {
                int p0 = nodePrimes[0];
                int p1 = nodePrimes[1];

                // skip existing combinations:
                while (global_used2.Contains(
                    (UInt64)p0 * (UInt64)0x100000000 + (UInt64)nextOfMaxPrime
                    ))
                {
                    nextOfMaxPrime = Primes.NextPrime(nextOfMaxPrime);
                    global_collissions += 1;
                }

                nodePrimes[1] = nextOfMaxPrime;
                q.Add(new PrimeSetNode(nodePrimes));
                global_used2.Add((UInt64)p0 * (UInt64)0x100000000 + (UInt64)nextOfMaxPrime);

                int n0NextPrime = Primes.NextPrime(p0);
                while (n0NextPrime < p1)
                {
                    if (global_used2.Contains(
                        (UInt64)n0NextPrime * (UInt64)0x100000000 + (UInt64)p1
                        ))
                    {
                        n0NextPrime = Primes.NextPrime(n0NextPrime);
                        continue;
                    }
                    nodePrimes[0] = n0NextPrime;
                    nodePrimes[1] = p1;
                    q.Add(new PrimeSetNode(nodePrimes));
                    global_used2.Add((UInt64)n0NextPrime * (UInt64)0x100000000 + (UInt64)p1);
                }
            }
        }



        static void solve()
        {
            int expanded_count = 0;

            PrimesetPrioQueue prioQueue = new PrimesetPrioQueue();
            int[] initialPrimes = { 3, 5, 0, 0, 0 };
            PrimeSetNode firstNode = new PrimeSetNode(initialPrimes);
            prioQueue.Add(firstNode);
            Dictionary<string, int> expandedDict = new Dictionary<string, int>();
            SortedSet<Int64> nonPairs = new SortedSet<Int64>();

            bool found = false;
            while (!found)
            {
                // get first
                PrimeSetNode n = prioQueue.GetFirst();
                if (n.m_primes[0] == 3
                    &&
                    n.m_primes[1] == 7
                    )
                {
                    ;
                }
                if (n.m_numPrimes == 5 && n.m_match)
                {
                    Console.WriteLine("solution: sum of primes = {0}", n.m_sum);
                    Console.Write("Prime numbers:");
                    for (int i = 0; i < 5; i++)
                        Console.Write("{0} ", n.m_primes[i]);
                    Console.WriteLine(" ,Sum:{0}", n.m_sum);
                    if (n.m_numPrimes >= 3)
                        return; return;
                }
                // expand
                Expand(prioQueue, n, expandedDict, nonPairs);
                expanded_count += 1;
                if (expanded_count % 100000 == 0)
                {
                    Console.WriteLine("{0} nodes expanded, {1} in priority queue.", expanded_count, prioQueue.ItemCount);
                }
                global_checked += 1;
            }
        }

        static void Main(string[] args)
        {
            System.Diagnostics.Stopwatch w = new System.Diagnostics.Stopwatch();
            w.Start();
            solve();
            w.Stop();
            var ts = w.Elapsed;
            Console.WriteLine("{0} ms, ({1} minutes and {2} seconds).", ts.TotalMilliseconds, ts.Minutes, ts.Seconds);
        }
    }
}
