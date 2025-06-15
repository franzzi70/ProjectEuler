using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace P0148_ExploringPascalsTriangle_CSharp
{
    internal class Program
    {

        const int MAX_N = 999_999_999;
        const int FACTOR = 7;
        const int LEVELS = 11;

        //uint64_t nRowsCount(uint32_t rows, uint32_t factor, bool invert = true)
        //{
        //    uint32_t a_factorPow[LEVELS];
        //    uint32_t a_digits[LEVELS];
        //    uint32_t a_count[LEVELS];
        //    uint32_t ix_high = 0;

        //    uint32_t scalef = 1;
        //    for (uint32_t i = 0; i < LEVELS; i++)
        //    {
        //        a_digits[i] = 0;
        //        a_count[i] = 0;
        //        a_factorPow[i] = scalef;
        //        scalef *= FACTOR;
        //    }


        //    int64_t sum = 0;

        //    for (uint32_t n = 0; n < rows; n++)
        //    {


        //        for (uint32_t i = 0; i < LEVELS; i++)
        //        {
        //            a_count[i] = 0;
        //        }

        //        for (uint32_t ix = ix_high; ix > 0; ix--)
        //        {
        //            uint32_t f = a_digits[ix] + a_count[ix];
        //            int32_t pow = a_factorPow[ix];
        //            int32_t fCount = pow - (n % pow) - 1;
        //            a_count[0] += f * fCount;
        //            for (uint32_t il = ix - 1; il > 0; il--)
        //            {
        //                a_count[il] += f * a_digits[il];
        //            }
        //        }
        //        int64_t count = a_count[0];


        //        sum += invert ?
        //            n + 1 - count :
        //            count;

        //        // update a_digit array with next number n
        //        bool carry = false;
        //        uint32_t inc_ix = 0;
        //        uint32_t d = a_digits[inc_ix] + 1;
        //        a_digits[inc_ix] = d > 6 ? 0 : d;
        //        while (d == factor)
        //        {
        //            inc_ix += 1;
        //            d = a_digits[inc_ix] + 1;
        //            a_digits[inc_ix] = d > 6 ? 0 : d;
        //        }
        //        if (inc_ix > ix_high)
        //            ix_high = inc_ix;
        //    }

        //    return sum;
        //}

        static ulong nRowsCount(uint rows, uint factor, bool invert = true)
        {
            uint[] a_factorPow = new uint[LEVELS];
            uint[] a_digits = new uint[LEVELS];
            uint[] a_count = new uint[LEVELS];
            uint ix_high = 0;

            uint scalef = 1;
            for (uint i = 0; i < LEVELS; i++)
            {
                a_digits[i] = 0;
                a_count[i] = 0;
                a_factorPow[i] = scalef;
                scalef *= FACTOR;
            }

            long sum = 0;

            for (uint n = 0; n < rows; n++)
            {
                for (uint i = 0; i < LEVELS; i++)
                {
                    a_count[i] = 0;
                }

                for (uint ix = ix_high; ix > 0; ix--)
                {
                    uint f = a_digits[ix] + a_count[ix];
                    int pow = (int)a_factorPow[ix];
                    int fCount = pow - (int)(n % (uint)pow) - 1;
                    a_count[0] += (uint)(f * fCount);
                    for (uint il = ix - 1; il > 0; il--)
                    {
                        a_count[il] += f * a_digits[il];
                    }
                }
                long count = a_count[0];

                sum += invert ? n + 1 - count : count;

                // update a_digit array with next number n
                uint inc_ix = 0;
                uint d = a_digits[inc_ix] + 1;
                a_digits[inc_ix] = d > 6 ? 0u : d;
                while (d == factor)
                {
                    inc_ix += 1;
                    d = a_digits[inc_ix] + 1;
                    a_digits[inc_ix] = d > 6 ? 0u : d;
                }
                if (inc_ix > ix_high)
                    ix_high = inc_ix;
            }

            return (ulong)sum;
        }

        static ulong nRowsCount_direct(uint rows, uint factor, bool invert = true)
        {
            uint[] a_factorPow = new uint[LEVELS];
            uint[] a_LfactorPow = new uint[LEVELS];
            uint[] a_digits = new uint[LEVELS];
            uint[] a_count = new uint[LEVELS];
            uint ix_high = 0;

            uint scalef = 1;
            for (uint i = 0; i < LEVELS; i++)
            {
                a_digits[i] = 0;
                a_count[i] = 0;
                a_factorPow[i] = scalef;
                scalef *= FACTOR;
            }

            long sum = 0;

            for (uint n = 0; n < rows; n++)
            {
                for (uint i = 0; i < LEVELS; i++)
                {
                    a_count[i] = 0;
                }

                for (uint ix = ix_high; ix > 0; ix--)
                {
                    uint f = a_digits[ix] + a_count[ix];
                    int pow = (int)a_factorPow[ix];
                    int fCount = pow - (int)(n % (uint)pow) - 1;
                    a_count[0] += (uint)(f * fCount);
                    for (uint il = ix - 1; il > 0; il--)
                    {
                        a_count[il] += f * a_digits[il];
                    }
                }
                long count = a_count[0];

                sum += invert ? n + 1 - count : count;

                // update a_digit array with next number n
                uint inc_ix = 0;
                uint d = a_digits[inc_ix] + 1;
                a_digits[inc_ix] = d > 6 ? 0u : d;
                while (d == factor)
                {
                    inc_ix += 1;
                    d = a_digits[inc_ix] + 1;
                    a_digits[inc_ix] = d > 6 ? 0u : d;
                }
                if (inc_ix > ix_high)
                    ix_high = inc_ix;
            }

            return (ulong)sum;
        }

        static ulong solve()
        {
            return nRowsCount(1_000_000_000, 7);
            // 2129970655314432
        }

        static void Main(string[] args)
        {
            var t1 = DateTime.Now;
            ulong solution = solve();
            var t2 = DateTime.Now;
            var microSec = (long)((t2 - t1).TotalMilliseconds * 1000);
            var ms = (long)(t2 - t1).TotalMilliseconds;
            Console.WriteLine($"solution: {solution}\nduration: {microSec} micro seconds ({ms} ms)");
            if (microSec > 300_000_000)
            {
                Console.WriteLine($"({(float)microSec / 60_000_000} minutes )");
            }
        }
    }
}
