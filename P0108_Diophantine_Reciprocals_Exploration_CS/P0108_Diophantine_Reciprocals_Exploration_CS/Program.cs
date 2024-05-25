
using Primes01_CS;

int MAXPRIME = 10000000;
int PRIMEARRSIZE = (MAXPRIME / 2);

MyPrimes_Bitset g_sieve = new MyPrimes_Bitset(MAXPRIME);


void init()
{
    Console.WriteLine(g_sieve.isPrime(11));
    Console.WriteLine(g_sieve.isPrime(10));
    MyPrimes_Bitset primes_Bitset = new MyPrimes_Bitset(1000_000);
    Console.WriteLine ("init");
}



init();
// See https://aka.ms/new-console-template for more information
Console.WriteLine("Hello, World!");
