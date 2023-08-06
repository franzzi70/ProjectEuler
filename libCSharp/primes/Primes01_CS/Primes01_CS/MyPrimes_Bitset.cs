using System;
using System.Runtime.CompilerServices;

namespace Primes01_CS
{

    public class NumberToBigException : System.Exception
    {
        public NumberToBigException(string msg) : base(msg) { }
    }

    public class MyPrimes_Bitset : IMyPrimes
    {
        /************************ interface IMyPrimes ************************/
        public bool IsPrime(Int32 n)
        {
            return isPrime(n);
        }
        public void PrintPrime(int n)
        {
            Console.WriteLine("number {0} prime: {1}", n, IsPrime(n));
        }
        public void PrintStoredPrimes()
        {
            Int64 _highestPrime = this.highestPrime();
            this.ListPrimes(1, _highestPrime);
        }
        public bool IsPrime2(Int32 n)
        {
            return isPrime(n);
        }
        /************************ interface IMyPrimes ************************/

        const Int64 DEFAULT_HIGHNUM = 10000000;
        UInt64[] m_arrBits;
        Int64 m_highestNum = DEFAULT_HIGHNUM;

        private Int64 highestPrime()
        {
            for (Int64 i = m_highestNum; i>0 ; i--)
            {
                if (isPrime(i))
                    return i;
            }
            return 0;
        }

        public MyPrimes_Bitset()
        {
            init(DEFAULT_HIGHNUM);
        }

        public MyPrimes_Bitset(Int64 highNum)
        {
            init(highNum);
        }

        void init(Int64 highNum)
        {
            //int bitCount = ((int)Math.Ceiling(
            //        Math.Log2(m_highestNum)
            //        / 2 // non-negative numbers do be stored as bit, they are all prime (except number 2)
            //        )) + 1;
            m_highestNum = highNum;
            Int64 bitCount = m_highestNum;
            Int64 arrSize = bitCount / 64 + 1;
            m_arrBits = new UInt64[arrSize];
            System.Array.Fill(m_arrBits, (UInt64)0xAAAAAAAAAAAAAAAA);

            var watch = System.Diagnostics.Stopwatch.StartNew();
            calcSieve();
            //the code that you want to measure comes here
            watch.Stop();
            var elapsedMs = watch.ElapsedMilliseconds;
            //int PrimeCount = CountPrimes(1, 1000000);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining | MethodImplOptions.AggressiveOptimization)]
        void setBit(Int64 bitNum, bool value)
        {
            UInt64 rest = ((UInt64)bitNum) & (UInt64)63;
            UInt64 arrPos = ((UInt64)bitNum) >> 06;   // 64 == 100000 binary.
            UInt64 bitMask = (UInt64)(((UInt64)1) << (int)rest);
            if (value)
            {
                m_arrBits[arrPos] |= bitMask;
            }
            else
            {
                m_arrBits[arrPos] &= (0xFFFFFFFFFFFFFFFF ^ bitMask);
            }
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining | MethodImplOptions.AggressiveOptimization)]
        bool getBit(Int64 bitNum)
        {
            UInt64 rest = ((UInt64)bitNum) & (UInt64)63;
            UInt64 arrPos = ((UInt64)bitNum) >> 06;   // 64 == 100000 binary.
            UInt64 bitMask = (UInt64)(((UInt64)1) << (int)rest);
            return !((m_arrBits[arrPos] & bitMask) == 0);
        }

        [MethodImpl(MethodImplOptions.AggressiveOptimization)]
        void calcSieve()
        {
            setBit(0, false);
            setBit(1, false);
            setBit(2, true);
            setBit(3, true);
            //setBit(4, false);
            //setBit(5, true);
            for (Int64 i=9;i <= m_highestNum; i+=6)
            {
                setBit(i, false);   // multiples of 3 are not prime
            }
            Int64 upToNumber = (Int64)Math.Ceiling(Math.Sqrt(m_highestNum));
            for (Int64 probeNum = 5; probeNum <= upToNumber; probeNum += 2)
            {
                if (!isPrime(probeNum)) // already marked as non-prime continue in loop
                    continue;

                if (_isPrime_Calc(probeNum))
                {
                    Int64 Offs = probeNum * 2;
                    for (Int64 i = probeNum * probeNum; i <= m_highestNum; i += Offs)  // double the number is even, and already marked as not prime
                    {
                        setBit(i, false);    // multiples of this number are not prime
                        //m_highestPrime = probeNum;
                    }
                }
            }
        }

        [MethodImpl(MethodImplOptions.AggressiveOptimization)]
        private bool _isPrime_Calc(Int64 probeNum)
        {
            //if (probeNum < 2)
            //    return false; // two is lowest prime.
            //if (probeNum == 2)  // two is a prime.
            //    return true;
            if (probeNum > m_highestNum)
                throw new NumberToBigException(string.Format("number {0} to big for prime test. highest stored number: {1}.", probeNum, m_highestNum));
            Int64 hightestTestPrime = (Int64)Math.Ceiling(Math.Sqrt(probeNum));
            if ((probeNum & 1) == 0)    // no even number is a prime (but 2)
                return false;
            for (Int64 i = 3; i <= hightestTestPrime; i += 2)
            {
                if (isPrime(i)) // only devide by prime numbers for test.
                    if (probeNum % i == 0)
                        return false;
            }
            return true;
        }

        public bool isPrime(Int64 num)
        {
            return getBit(num);
        }

        public int CountPrimes (Int64 from, Int64 to)
        {
            int primeCount = 0;
            for (Int64 i=from; i<= to; i++)
            {
                if (isPrime(i))
                    primeCount += 1;
            }
            return primeCount;
        }

        public void ListPrimes(Int64 first, Int64 last)
        {
            for (Int64 i = first; i <= last; i++)
            {
                if (getBit(i))
                {
                    Console.WriteLine(i);
                }
            }
        }

    }

}