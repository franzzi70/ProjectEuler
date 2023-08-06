using System;
using System.Collections.Generic;

namespace P61_Cyclical_figurate_numbers
{
    class Program
    {
        // static SortedDictionary<int, SortedDictionary<int, int>> octagonal = new SortedDictionary<int, SortedDictionary<int, int>>();

        static SortedDictionary<int, SortedDictionary<int, int>>[] arrNumbers = new SortedDictionary<int, SortedDictionary<int, int>>[6];

        static int func1(int i) { return (i * (i + 1)) >> 1; }
        static int func2(int i) { return i * i; }
        static int func3(int i) { return (i * (3*i - 1)) >> 1; }
        static int func4(int i) { return i * (2*i - 1); }
        static int func5(int i) { return (i * (5*i - 3)) >> 1; }
        static int func6(int i) { return i * (3*i -2); }

        static void InitNumbers()
        {
            for (int i=0;i<6;i++)
            {
                arrNumbers[i] = new SortedDictionary<int, SortedDictionary<int, int>>();
            }
            InitDict(arrNumbers[0],func1);
            InitDict(arrNumbers[1], func2);
            InitDict(arrNumbers[2], func3);
            InitDict(arrNumbers[3], func4);
            InitDict(arrNumbers[4], func5);
            InitDict(arrNumbers[5], func6);
        }

        static void InitDict(SortedDictionary<int, SortedDictionary<int, int>> dict, Func<int, int> f)
        {
            int p = 0;
            int count = 0;
            for (int i = 0; p < 10000; i++)
            {
                p = f(i);
                if (p < 1000)
                    continue;
                if (p < 10000)
                {
                    InitNumber(p, dict);
                    count += 1;
                }
            }
        }

        static void InitNumber(int num, SortedDictionary<int, SortedDictionary<int, int>> dict)
        {
            int key = num % 100;

            if (dict.ContainsKey(key))
            {
                dict[key].Add(num, num);
            }
            else
            {
                SortedDictionary<int, int> newEntry = new SortedDictionary<int, int>();
                newEntry.Add(num, num);
                dict.Add(key, newEntry);
            }
        }


        static void solve()
        {
            InitNumbers();

            IList<IList<int>> permutations = Permute(new int[] { 0, 1, 2, 3, 4 });
            foreach (var list in permutations)
            {
                FindCycle(list);
                //Console.WriteLine($"    [{string.Join(',', list)}]");
            }

        }

        static void FindCycle(IList<int> listOrder)
        {
            List<int> solution = new List<int>();

            foreach(var p in arrNumbers[5])
            {
                foreach(var p2 in p.Value)
                {
                    if (FindCycle(p2.Value, p2.Value, listOrder, 0, ref solution))
                    {
                        int sum = 0;
                        foreach(int n in solution)
                        {
                            Console.WriteLine("{0}", n);
                            sum += n;
                        }
                        Console.WriteLine("sum: {0}", sum);
                    }
                }
            }
        }

        static bool FindCycle(int firstNumber, int inputNumber, IList<int> listOrder, int depth, ref List<int> solution)
        {
            if (depth > 4)
            {
                if (firstNumber % 100 == inputNumber / 100)
                {
                    solution.Add(inputNumber);
                    return true;
                }
                else
                    return false;
            }
            var selectedList = arrNumbers[listOrder[depth]];

            int key = inputNumber / 100;
            if (selectedList.ContainsKey(key))
            {
                foreach(var p in selectedList[key])
                {
                    if (FindCycle(firstNumber, p.Value, listOrder, depth + 1, ref solution))
                    {
                        solution.Add(inputNumber);
                        return true;
                    }
                }
                return false;
            }
            else
                return false;
        }




        static IList<IList<int>> Permute(int[] nums)
        {
            var list = new List<IList<int>>();
            return DoPermute(nums, 0, nums.Length - 1, list);
        }

        static IList<IList<int>> DoPermute(int[] nums, int start, int end, IList<IList<int>> list)
        {
            if (start == end)
            {
                // We have one of our possible n! solutions,
                // add it to the list.
                list.Add(new List<int>(nums));
            }
            else
            {
                for (var i = start; i <= end; i++)
                {
                    Swap(ref nums[start], ref nums[i]);
                    DoPermute(nums, start + 1, end, list);
                    Swap(ref nums[start], ref nums[i]);
                }
            }

            return list;
        }

        static void Swap(ref int a, ref int b)
        {
            var temp = a;
            a = b;
            b = temp;
        }


        static void Main(string[] args)
        {
            System.Diagnostics.Stopwatch w = new System.Diagnostics.Stopwatch();
            w.Start();
            solve();
            var ts = w.Elapsed;
            Console.WriteLine("{0} ms, ({1} minutes and {2} seconds).", ts.TotalMilliseconds, ts.Minutes, ts.Seconds);
        }
    }
}
