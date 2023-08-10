using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


namespace Main
{

    internal class Program
    {
        static decimal sqrt2 = (decimal)Math.Sqrt(2) / 2;

        static decimal testForN(decimal n)
        {
            decimal n2 = n - 1;
            decimal testB = Math.Floor(n * sqrt2)-3;
            decimal diff = -1;
            do
            {
                diff = 2 * (testB * testB - testB) - (n * n - n);

                if (diff == 0)
                    return testB;
                testB += 1;
            }
            while (diff < 0);

            return -1;
        }

        static decimal Solve()
        {
            //decimal testNum = 1000000000001;
            decimal prevN = 21;
            decimal lastN = 120;



            //decimal testNum = 150000;
            do
            {
                decimal nextN = 6*lastN - prevN - 2;

                decimal blueCount = testForN(nextN);
                if (blueCount > 0 && nextN > 1000000000000)
                {
                    return blueCount;
                }

                prevN = lastN;
                lastN = nextN;
            }
            while (true);
        }

        static void Main(string[] args)
        {

            long startTime = DateTime.Now.Ticks;
            decimal solution = Solve();
            double ms = new TimeSpan(DateTime.Now.Ticks - startTime).TotalMilliseconds;
            System.Console.WriteLine(@"solution: {0}", solution);
            System.Console.WriteLine(@"duration: {0} ms", ms);
        }
    }
}
