using System;


namespace P0082_PathSumThreeWays_CSharp
{
    class Program
    {
        static int[,] matrix = new int[80, 80];
        static string matrixFileName = @"../../../../../p082_matrix.txt";
        static int[,,] matrixMem = new int[80, 80, 3];
        static bool memInitialized = false;


        static void LoadMatrix()
        {
            string[] lines = System.IO.File.ReadAllLines(matrixFileName);
            int linesCount = lines.Length;
            for (int i = 0; i < linesCount; i++)
            {
                string[] strNumbers = lines[i].Split(",");
                int columnsCount = strNumbers.Length;
                for (int j = 0; j < columnsCount; j++)
                {
                    matrix[i, j] = int.Parse(strNumbers[j]);
                }
            }
        }

        static int shortestPathLen2(int i, int j, int startLine, int endLine, int currentDirection)
        {
            // allow both vertical directions

            if (!memInitialized)
            {
                memInitialized = true;
                for (int ii = 0; ii < 80; ii++)
                    for (int jj = 0; jj < 80; jj++)
                        for (int kk = 0; kk < 3; kk++)
                            matrixMem[ii, jj, kk] = 0;
            }

            int val;


            int matrixVal = matrixMem[i, j, currentDirection +1];
            if (matrixVal != 0)
            {
                return matrixVal;
            }


            int fieldValue = matrix[i, j];
            if (i == startLine && j == 0)
                val = fieldValue;
            else
            if (j == 0)
            {
                int vertStep = Math.Sign(startLine - i);
                val = fieldValue + shortestPathLen2(i + vertStep, j, startLine, endLine, vertStep);
            }
            else
            {
                int leftVal = shortestPathLen2(i, j - 1, startLine, endLine, 0);
                val = leftVal;
                if (currentDirection == 1 || currentDirection == 0)
                {
                    if (i < 79)
                    {
                        int downVal = shortestPathLen2(i + 1, j, startLine, endLine, 1);
                        val = Math.Min(val, downVal);
                    }
                }
                if (currentDirection == -1 || currentDirection == 0)
                {
                    if (i > 0)
                    {
                        int upVal = shortestPathLen2(i - 1, j, startLine, endLine, -1);
                        val = Math.Min(val, upVal);
                    }
                }
                val = fieldValue + val;
            }

            matrixMem[i, j, currentDirection + 1] = val;
            return val;
        }

        static int Solve()
        {
            LoadMatrix();
            int minLen = 100000000;
            for (int i=0;i<80;i++)
                for (int j=0;j<80;j++)
                {
                    memInitialized = false;
                    int len = shortestPathLen2(j, 79, i, j, 0);
                    if (len < minLen)
                    {
                        minLen = len;
                    }
                }
            return minLen;
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
