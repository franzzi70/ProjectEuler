using System;
using System.IO;


namespace P0081_PathSumTwoWays_CSharp
{
    class Program
    {

        static int[,] matrix = new int[80,80];
        static string matrixFileName = @"../../../../../p081_matrix.txt";
        static int[,] matrixMem = new int[80, 80];
        static bool memInitialized = false;


        static void LoadMatrix()
        {
            string[] lines = System.IO.File.ReadAllLines(matrixFileName);
            int linesCount = lines.Length;
            for (int i=0; i<linesCount; i++)
            {
                string[] strNumbers = lines[i].Split(",");
                int columnsCount = strNumbers.Length;
                for (int j=0;j<columnsCount; j++)
                {
                    matrix[i, j] = int.Parse(strNumbers[j]);
                }
            }
        }

        static int shortestPathLen(int i, int j)
        {
            if (!memInitialized)
            {
                memInitialized = true;
                for (int ii = 0; ii < 80; ii++)
                    for (int jj = 0; jj < 80; jj++)
                        matrixMem[ii,jj] = 0;
            }

            int val;

            int matrixVal = matrixMem[i, j];
            if (matrixVal != 0)
            {
                return matrixVal;
            }

            int fieldValue = matrix[i, j];
            if (i == 0 && j == 0)
                val = fieldValue;
            else
            if (i == 0)
                val = fieldValue + shortestPathLen(i, j - 1);
            else
            if (j== 0)
                val = fieldValue + shortestPathLen(i -1,j);
            else
                val = fieldValue + + Math.Min(shortestPathLen(i, j - 1), shortestPathLen(i - 1, j));

            matrixMem[i,j] = val;
            return val;
        }

        static int Solve()
        {
            LoadMatrix();
            int len = shortestPathLen(79, 79);
            return len;
        }

        static void Main(string[] args)
        {

            var t1 = DateTime.UtcNow;
            int solution = Solve();
            var t2 = DateTime.UtcNow;

            Console.WriteLine("solution: {0}", solution);
            Console.WriteLine("duration: {0} ms.", t2.Subtract(t1).TotalMilliseconds);
        }
    }
}
