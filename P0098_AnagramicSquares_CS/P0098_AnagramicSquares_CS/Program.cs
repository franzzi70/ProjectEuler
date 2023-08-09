using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Collections;
using System.IO;
using System.Data.SqlTypes;
using System.Net.Http.Headers;
using System.ComponentModel;

namespace Main
{
    internal sealed class Pair
    {
        public Pair(string str1, String str2)
        {
            Word1 = str1;
            Word2 = str2;
        }

        public string Word1 { get; set; }
        public string Word2 { get; set; }
    }

    internal class Program
    {

        static bool IsAnagram(String str1, String str2)
        {
            char[] chars1 = str1.ToCharArray();
            char[] chars2 = str2.ToCharArray();
            Array.Sort(chars1);
            Array.Sort(chars2);
            String sorted1 = new String(chars1);
            String sorted2 = new String(chars2);
            return sorted1.Equals(sorted2);
        }

        static bool IsPalindrome(String str1, String str2)
        {
            char[] chars1 = str1.ToCharArray();
            char[] chars2 = str2.ToCharArray();
            Array.Reverse(chars2);
            String reversed2 = new String(chars2);
            return str1.Equals(reversed2);
        }

        static List<Pair> ParseAnagrams()
        {
            StreamReader f = new StreamReader(@"0098_words.txt");
            List<String> words = new List<String>();

            String fileContent = f.ReadToEnd();
            f.Close();

            // Console.Write(fileContent);

            int startIx = 0;
            while (true)
            {
                int endIx = fileContent.IndexOf(',', startIx);
                if (endIx == -1)
                {
                    break;
                }
                string word = fileContent.Substring(startIx + 1, endIx - startIx - 2);
                words.Add(word);
                startIx = endIx + 1;
            }

            List<Pair> pairList = new List<Pair>();
            for (int i = 0; i < words.Count; i++)
            {
                for (int j = i + 1; j < words.Count; j++)
                {
                    String word1 = words[i];
                    String word2 = words[j];
                    if (word1.Length != word2.Length)
                    {
                        continue;
                    }
                    if (word1.Equals(word2))
                    {
                        continue;
                    }
                    if (word1.Length == word2.Length)
                    {
                        if (IsAnagram(word1, word2) && !IsPalindrome(word1, word2))
                        pairList.Add(new Pair ( word1, word2 ));
                    }
                }
            }

            //for (int i = 0; i < pairList.Count; i++)
            //{
            //    Console.WriteLine("{0} {1}", pairList[i].Word1, pairList[i].Word2);
            //}

            return pairList;
        }


        static long getHighSqAnagramNumber (string str1, string str2, long q1 )
        {
            char[] digitChars = { '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0' };
            int[] charDigits = {
                -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                -1, -1, -1, -1, -1, -1
                };

            long n1 = q1 * q1;
            if (n1 == 400400100)
            {
                Console.WriteLine(n1);
            }
            int len1 = str1.Length;
            // assign chars to digits of n1
            for (int i = len1-1; i >= 0; i--)
            {
                char c = str1[i];
                int digit = (int)(n1 % 10);
                if (charDigits[c-65] == -1)
                {
                    char storedDigitChar = digitChars[digit];
                    if (storedDigitChar != '\0' && storedDigitChar != c)
                    {
                        return 0;
                    }
                    charDigits[c-65] = digit;
                    digitChars[digit] = c;
                }
                else
                {
                    if (charDigits[c-65] != digit)
                    {
                        return 0;
                    }
                }
                n1 /= 10;
            }
            long n2 = 0;

            // assign chars to digits of n2
            for (int i = 0; i < str2.Length; i++)
            {
                char c = str2[i];
                int digit = charDigits[c-65];
                if (i==0 && digit==0)
                {
                    return 0;
                }
                if (digit == -1)
                {
                    return 0;
                }
                n2 = n2 * 10 + digit;
            }

            long q2 = (long)(Math.Round(Math.Sqrt(n2)));
            if (q2*q2 != n2)
            {
                return 0;
            }

            if ( n2> n1)
            {
                return n2;
            }
            else
            { 
                return n1;
            }    
        }

        static long getPairNumber(String str1, String str2)
        {
            int len1 = str1.Length;
            long highP10 = 1;
            for (int i = 0; i < len1; i++)
            {
                highP10 *= 10;
            }
            long lowP10 = highP10 / 10;
            long firstTestNum = (long)(Math.Sqrt(highP10)) + 1;
            
            long highestNumber = 0;

            long q = firstTestNum;
            while (q * q >=lowP10)
            {
                long prod1 = q * q;
                if (prod1 >= highP10)
                {
                    q -= 1;
                    continue;
                }
                if (prod1 < lowP10)
                {
                    break;
                }

                long checkNum1 = getHighSqAnagramNumber(str1, str2, q);
                if (checkNum1 > highestNumber)
                {
                    highestNumber = checkNum1;
                }

                long checkNum2 = getHighSqAnagramNumber(str2, str1, q);
                if (checkNum2 > highestNumber)
                {
                    highestNumber = checkNum2;
                }

                q -= 1;
            }

            return highestNumber;
        }

        static long Solve()
        {
            List<Pair> pairList = ParseAnagrams();
            long highestNumber = 0;

            foreach (Pair pair in pairList)
            {
                long num1 = getPairNumber(pair.Word1, pair.Word2);
                if (num1 > highestNumber)
                {
                    highestNumber = num1;
                }
                long num2 = getPairNumber(pair.Word2, pair.Word1);
                if (num2 > highestNumber)
                {
                    highestNumber = num2;
                }
            }

            return highestNumber;
        }

        static void Main(string[] args)
        {

            long startTime = DateTime.Now.Ticks;
            long solution = Solve();
            double ms = new TimeSpan(DateTime.Now.Ticks - startTime).TotalMilliseconds;
            System.Console.WriteLine(@"solution: {0}", solution);
            System.Console.WriteLine(@"duration: {0} ms", ms);
            //System.Console.WriteLine("press any key to continue...");
            Console.ReadKey(true);
        }
    }
}
