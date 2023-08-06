using System;
using System.Collections;
using System.Collections.Generic;


namespace P0083_PathSumFourWays_CSharp
{
    public class Point
    {
        public Point (int i, int j, int len)
        {
            this.i = i;
            this.j = j;
            this.len = len;
        }

        public int i;
        public int j;
        public int len;
    }

    public class Path
    {
        public Path()
        {
            m_path = new ArrayList();
            visited = new BitArray(80 * 80, false);
            len = 0;
        }

        public Path(Path orig)
        {
            this.m_path = new ArrayList(orig.m_path);
            this.visited = new BitArray(orig.visited);
            this.len = orig.len;
        }

        public void AddPoint(int i, int j, ref int[,] matrix)
        {
            int val = matrix[i, j];
            len += val;
            m_path.Add(new Point(i,j,len));
            visited[i * 80 + j] = true;
        }

        public void Print(int Format)
        {
            Console.WriteLine("len={0}, pointCount={1}", this.len, m_path.Count);
            for (int i=0;i<m_path.Count;i++)
            {
                Point p = (Point)m_path[i];
                Console.WriteLine("i,j = {0}, {1}", p.i, p.j);
            }
        }

        public Point GetLastPoint()
        {
            return (Point)m_path[m_path.Count - 1];
        }

        public int len;
        public ArrayList m_path;
        public BitArray visited;
    }


    class Program
    {
        static string matrixFileName = @"../../../../../p083_matrix.txt";
        static int[,] matrix = new int[80, 80];
        static int[] OrderedValues = new int[80 * 80];
        static int[] RunningSum = new int[80 * 80];
        static SortedList<int, int> Visited = new SortedList<int, int>();

        static SortedList<long, Path> PrioQueue = new SortedList<long, Path>();

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


        static void Init()
        {
            LoadMatrix();
            for (int i = 0; i < 80; i++)
                for (int j = 0; j < 80; j++)
                {
                    int val = matrix[i, j];
                    OrderedValues[i * 80 + j] = val;
                }
            Array.Sort(OrderedValues);
            int sum = 0;
            for (int i=0;i<80*80; i++)
            {
                sum += OrderedValues[i];
                RunningSum[i] = sum;
            }
        }

        static int salt = 0;
        static long expansionCount = 0;
        static int Solve()
        {
            Init();

            Path p0 = new Path();
            p0.AddPoint(0, 0, ref matrix);
            Visited[0] = matrix[0, 0];

        PrioQueue.Add(matrix[0, 0] << 32, p0);

            while (PrioQueue.Count > 0)
            {
                // get top element
                Path  p = PrioQueue.Values[0];
                int len = p.len;

                PrioQueue.RemoveAt(0);
                Point LastPoint = p.GetLastPoint();
                if (LastPoint.i == 79 && LastPoint.j == 79)
                {
                    Console.WriteLine("expansion count: {0}, queue size:{1}, path node count: {2}", expansionCount, PrioQueue.Count, p.m_path.Count);
                    return p.len;
                }

                int p_i = LastPoint.i;
                int p_j = LastPoint.j;
                int[] distr = {0,1,0,-1,1,0,-1,0};
                // expand:
                for (int k = 0; k < 4; k++)
                {
                    int i = distr[k * 2];
                    int j = distr[k * 2 + 1];
                    int new_i = p_i + i;
                    int new_j = p_j + j;
                    int visitedIndex = new_i * 80 + new_j;
                    if (new_i >= 0 && new_j >= 0 && new_i < 80 && new_j < 80 && !p.visited[visitedIndex])
                    {
                        expansionCount += 1;
                        if (expansionCount % 100000 == 0)
                        {
                            Console.WriteLine("expansion count: {0}, i:{1}, j:{2}, queue size:{3}", expansionCount, new_i, new_j,PrioQueue.Count);
                        }

                        int newLen = len + matrix[new_i, new_j];
                        int VisitedLen;
                        if (Visited.TryGetValue(visitedIndex, out VisitedLen))
                        {
                            if (newLen >= VisitedLen)
                                continue;
                        }
                        else
                        {
                            Visited.Add(visitedIndex, VisitedLen);
                        }

                        Path newPath = new Path(p);
                        newPath.AddPoint(new_i, new_j, ref matrix);
                        long newKey = (((long)newLen) << 32);
                        newKey += ((long)RunningSum[(79 - new_i) + (79 - new_j)]) << 32;
                        newKey += ((long)(new_i * 80 + new_j)) << 16;
                        newKey += (long)((salt % (1 << 16)));
                        if (newKey==0)
                        {
                            break;
                        }
                        salt += 1;
                        try
                        {
                            PrioQueue.Add(newKey, newPath);
                        }
                        catch (System.Exception e)
                        {
                            Console.WriteLine("queue size: {0}", PrioQueue.Count);
                            Console.WriteLine(e.Message);
                            newPath.Print(0);
                            Path colPath;
                            if (PrioQueue.TryGetValue(newKey, out colPath))
                                colPath.Print(0);
                            return 0;
                        }
                    }
                }
            }

            return 0;
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
