using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Primes01_CS
{
    public interface IMyPrimes
    {
        public bool IsPrime(Int32 n);
        public void PrintPrime(int n);
        public void PrintStoredPrimes();
        public bool IsPrime2(Int32 n);
    }
}
