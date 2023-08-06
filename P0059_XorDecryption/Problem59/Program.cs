using System;

namespace Problem59
{

    class Program
    {
        static bool decode(byte[] src, byte[]dest, byte[] key)
        {
            int textLen = src.Length;
            int keyLen = key.Length;
            int letterCount = 0;

            for (int i = 0; i<textLen;i++)
            {
                byte ch = (byte)((src[i]) ^ (key[i % keyLen]));
                dest[i] = ch;

                if (
                    (ch >= 65 && ch <= 65+26)
                    ||
                    (ch >= 97 && ch <= 97 + 26)
                    ||
                    ch == 32
                    )
                {
                    letterCount += 1;
                }
            }
            double letterRatio = (double)letterCount / (double)textLen;

            if (letterRatio > 0.9)
                return true;
            else
                return false;
        
        }

        static string arrToString(byte[] dest)
        {
            string s = "";
            for (int i = 0; i < dest.Length; i++)
                s = s + (char)dest[i];
            return s;
        }

        static void decode(byte[] src, byte[] dest)
        {
            byte[] keyArr = { 97, 97, 97 };

            for (byte i = 0; i < 26; i++)
            {
                keyArr[0] = (byte)(97 + i);
                for (byte j = 0; j < 26; j++)
                {
                    keyArr[1] = (byte)(97 + j);
                    for (byte k = 0; k < 26; k++)
                    {
                        keyArr[2] = (byte)(97 + k);
                        Console.WriteLine("key:{0}{1}{2}", (char)keyArr[0], (char)keyArr[1], (char)keyArr[2]);
                        if (decode(src, dest, keyArr))
                        {
                            Console.WriteLine("{0}",arrToString(dest));
                            int asciiSum = 0;
                            for (int c=0;c<dest.Length;c++)
                            {
                                asciiSum += (int)dest[c];
                            }
                            Console.WriteLine("sum of ASCII values:{0}", asciiSum);
                            Console.ReadLine();
                            // return;
                        }
                    }
                }
            }
        }

        static void solve()
        {
            string text = System.IO.File.ReadAllText(@"p059_cipher.txt");
            string[] tokens = text.Split(',');
            int arrSize = tokens.Length;
            byte[] src_arr = new byte[arrSize];
            for (int i = 0; i < arrSize; i++)
                src_arr[i] = (byte)int.Parse(tokens[i]);

            byte[] target_arr = new byte[arrSize];

            decode(src_arr, target_arr);

        }

        static void Main(string[] args)
        {
            /*
             * byte a = (byte)65;
            byte b = (byte)42;

            byte c = (byte)(a ^ b);
            byte d = (byte)(c ^ b);
            Console.WriteLine("a,b,c,d: {0},{1},{2},{3}", a, b, c, d);
            */

            Console.WriteLine("test: a: {0}.", (char)((byte)97));

            solve();
            Console.WriteLine("Hello World!");
        }
    }
}
