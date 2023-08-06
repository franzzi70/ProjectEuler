using System;
using System.Collections;
using System.Numerics;

public class Utils
{
	public static int ggt(int a, int b)
	{
		int _a = a, _b = b;
		int r;
		do
		{
			r = _a % _b;
			if (r == 0)
			{
				// std::cout << "ggt(" << a << "," << b << ") = " << _b << std::endl;
				return _b;
			}

			_a = _b;
			_b = r;
		} while (true);
	}

	public static BigInteger ggt(BigInteger a, BigInteger b)
	{
		BigInteger _a = a, _b = b;
		BigInteger r;
		do
		{
			r = _a % _b;
			if (r == 0)
			{
				// std::cout << "ggt(" << a << "," << b << ") = " << _b << std::endl;
				return _b;
			}

			_a = _b;
			_b = r;
		} while (true);

	}

}

public class fraction
{
	public fraction(BigInteger numerator, BigInteger denominator)
	{
		_num = numerator;
		_denom = denominator;
	}
	public BigInteger _num;
	public BigInteger _denom;
};

public class term
{
	public term(int sq)
	{
		_ax = (int)Math.Sqrt((double)sq);
		_sq = sq;
		_a = _ax;
		_q = sq - (_a * _a);
		_f = 1;
	}

	public term(int ax, int sq, int a, int f, int q)
	{
		_ax = ax;
		_sq = sq;
		_a = a;
		_f = f;
		_q = q;
	}

	public term next()
	{
		int n_ax = (int)Math.Floor((double)_f * (Math.Sqrt(_sq) + _a) / (double)_q);
		int aa = _a - _q * n_ax;
		int n_a = (-aa);
		int den = _sq - (aa * aa);
		int _ggt = Utils.ggt(_q, den);
		int f = _q / _ggt;
		int q = den / _ggt;
		return new term(n_ax, _sq, n_a, f, q);
	}

	public int getAx()
	{
		return _ax;
	}

	public override bool Equals(Object obj)
	{
		//Check for null and compare run-time types.
		if ((obj == null) || !this.GetType().Equals(obj.GetType()))
		{
			return false;
		}
		term t = (term)obj;

		return
			_ax == t._ax
			&&
			_a == t._a
			&&
			_f == t._f
			&&
			_q == t._q
			&&
			_sq == t._sq
			;
	}

	public override int GetHashCode()
	{
		return (_ax ^ _sq ^ _a ^ _f ^ _q);
	}

	public int _ax;
	public int _sq;
	public int _a;
	public int _f;
	public int _q;
};

public class SqContFraction
{
	public SqContFraction(int SqNumber)
	{
		m_SqNumber = SqNumber;
		calcChain(SqNumber, false);

	}
	public int m_SqNumber;
	public int period = 0;
	ArrayList chain = new ArrayList();

	private int calcChain(int n, bool verbose = false)
	{


		int tst = (int)Math.Round(Math.Sqrt(n), 0);
		if (tst * tst == n)
			return 0;   // integers as root do not work.

		term t = new term(n);
		term t1 = t.next();
		term tn = t1;
		int i = 1;

		chain.Add(t.getAx());
		if (verbose)
			Console.Write("sqrt({0}) = [{1}; (", n, t.getAx());

		while (true)
		{
			chain.Add(tn.getAx());
			if (verbose)
				Console.Write(tn.getAx());
			tn = tn.next();
			if (tn.Equals(t1))
			{
				break;
			}
			if (verbose)
				Console.Write(",");
			i += 1;
		}
		period = i;
		if (verbose)
			Console.WriteLine(")], period = {0}", i);
		return i;
	}

	private int getChainNumber(int n)
	{
		if (n == 1)
			return (int)chain[0];
		return (int)chain[(n - 2) % period + 1];
	}

	public fraction calcFraction(int n)
	{
		int chainNumber = getChainNumber(n);
		fraction accu = new fraction(getChainNumber(n), 1);
		for (int i = n - 1; i >= 1; --i)
		{
			// kehrwert
			BigInteger tmp = accu._num;
			accu._num = accu._denom;
			accu._denom = tmp;

			// auf gleichen nenner bringen und addieren
			BigInteger Num1 = getChainNumber(i) * accu._denom + accu._num;
			BigInteger Denom1 = accu._denom;
			// kuerzen
			BigInteger _ggt = BigInteger.GreatestCommonDivisor(Num1, Denom1);
			// int _ggt = 1;
			accu._num = Num1 / _ggt;
			accu._denom = Denom1 / _ggt;
		}
		return accu;
	}

}


namespace _0066_DiophantineEquation_CSharp
{
	class Program
	{
		static int solve()
		{
			BigInteger maxX = 0;
			int DforMaxX = 0;

			for (int D = 2; D <= 1000; D++)
			{
				int testSq = (int)Math.Round(Math.Sqrt(D), 0);
				if (testSq * testSq == D)   // squares do not have a solution
					continue;


				SqContFraction a = new SqContFraction(D);

				int k;
				if (a.period % 2 == 1)
					k = a.period * 2;
				else
					k = a.period;

				fraction test = a.calcFraction(k);
				//Console.WriteLine("{0} ^2 - {2} * {1} ^2 = ", test._num, test._denom, D); 

				//Console.Write(test._num * test._num - D * test._denom * test._denom);

				if (test._num > maxX)
				{
					maxX = test._num;
					DforMaxX = D;
					Console.WriteLine("{0} ^2 - {2} * {1} ^2 = {3}", test._num, test._denom, D, test._num * test._num - D * test._denom * test._denom);
				}
				//fraction test2 = a.calcFraction(a.period);
			}

			return DforMaxX;
		}

		static void Main(string[] args)
		{
			Console.WriteLine("solution: {0}", solve());
		}
	}
}
