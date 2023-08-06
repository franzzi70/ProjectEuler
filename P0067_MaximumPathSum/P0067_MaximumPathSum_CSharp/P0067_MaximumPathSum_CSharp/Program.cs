using System;

namespace P0067_MaximumPathSum_CSharp
{

    class Program
    {
        static int[][] triangleNumbers = new int[100][];
        static int[][] triangleMax = new int[100][];

        static void init()
        {
            //string[] triangleLines = System.IO.File.ReadAllLines(@"D:\FZROOT\dev\ProjectEuler\0066_MaximumPathSum2\p067_triangle.txt");
            string[] triangleLines = System.IO.File.ReadAllLines(@"..\..\..\..\p067_triangle.txt");

            for (int i = 0; i < 100; i++)
            {
                int charIx = 0;
                triangleNumbers[i] = new int[i + 1];
                for (int j = 0; j <= i; j++)
                {
                    triangleNumbers[i][j] = (triangleLines[i][charIx] - '0') * 10 + triangleLines[i][charIx + 1] - '0';
                    charIx += 3;
                }
            }
        }

        static int solve()
        {
            init();
            for (int i = 99; i >= 0; i--)
            {
                triangleMax[i] = new int[i + 1];
                for (int j = 0; j <= i; j++)
                {
                    if (i == 99)
                        triangleMax[i][j] = triangleNumbers[i][j];
                    else
                    {
                        int val1 = triangleMax[i + 1][j];
                        int val2 = triangleMax[i + 1][j + 1];
                        if (val1 > val2)
                            triangleMax[i][j] = val1 + triangleNumbers[i][j];
                        else
                            triangleMax[i][j] = val2 + triangleNumbers[i][j];
                    }
                }
            }
            return triangleMax[0][0];
        }

        static void Main(string[] args)
        {
            Console.WriteLine("solution: {0}", solve());
        }
    }
}
