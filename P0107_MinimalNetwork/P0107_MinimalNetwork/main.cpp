// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <iostream>
#include <chrono>
#include <map>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <tuple>

typedef std::vector<std::vector<int>> Matrix;
extern std::vector<std::string> data;
extern std::vector<std::string> data2;
std::map<int, std::tuple<int, int>> sortedEdges;

Matrix createMatrix(std::vector<std::string> strings)
{
	Matrix matrix;
    for (std::string& line : strings)
    {
		std::vector<int> row;
        // parse integers separated by space
        //std::string::size_type sz;
        std::string delimiter = ",";
        size_t pos = 0;
        size_t foundPos = 0;
        std::string token;
        do
        {
			foundPos = line.find(delimiter, pos);
            if (pos == std::string::npos)
            {
                token = line.substr(pos+1, line.length() - foundPos);
            }
			token = line.substr(pos,foundPos - pos);
            pos = foundPos + 1;

			if (token == "-")
				row.push_back(-1);
			else
				row.push_back(std::stoi(token));
		} while (foundPos != std::string::npos);

		matrix.push_back(row);
	}
	return matrix;
}

bool pathExists(Matrix matrix, int start, int end, std::set<int>& visited)
{
    if (start == end)
		return true;

	visited.insert(start);

    for (int i = 0; i < matrix.size(); i++)
    {
        if (matrix[start][i] > 0 && visited.find(i) == visited.end())
        {
			if (pathExists(matrix, i, end, visited))
				return true;
		}
	}
	return false;
}

bool pathExists(Matrix matrix, int start, int end)
{
	std::set<int> visited;
	return pathExists(matrix, start, end, visited);
}




int edgeSum(Matrix& matrix)
{
    int sum = 0;
    for (int i = 0; i < matrix.size(); i++)
    {
        for (int j = i; j < matrix.size(); j++)
        {
            if (matrix[i][j] > 0)
            {
                sum += matrix[i][j];
            }
        }
    }
    return sum;
}



// algorigh from Prim
int addEdge(Matrix& fullGraph, Matrix& minGraph, std::set<int>& visited)
{
    int min = INT_MAX;
    int minI;
    int minJ;
    size_t nodeCount = fullGraph.size();

    for (int i = 0; i < nodeCount; i++)
    {
        if (visited.find(i) != visited.end())
        {
            for (int j = 0; j < nodeCount; j++)
            {
                if (
                    fullGraph[i][j] >= 0 && 
                    fullGraph[i][j] < min &&
                    visited.find(j) == visited.end())
                {
					min = fullGraph[i][j];
                    minI = i;
                    minJ = j;
				}
			}
        }
    }
    if (min == INT_MAX)
		return -1;
    minGraph[minI][minJ] = min;
    minGraph[minJ][minI] = min;
    visited.insert(minJ);
    return min;
}

Matrix createMinSpanningGraph(Matrix& fullGraph)
{
    Matrix minGraph;
    for (int i = 0; i < fullGraph.size(); i++)
    {
		std::vector<int> row;
        for (int j = 0; j < fullGraph.size(); j++)
        {
			row.push_back(-1);
		}
		minGraph.push_back(row);
	}

    int edgeSum = 0;
    int startNode = std::get<0>(sortedEdges.begin()->second);
    std::set<int> addedNodes;
    addedNodes.insert(startNode);
    do
    {
        int edgeLen = addEdge(fullGraph, minGraph, addedNodes);
        if (edgeLen < 0)
            break;
        edgeSum += edgeLen;
    } while (true);
    return minGraph;
}

void createSortedEdges(Matrix& matrix, std::map<int, std::tuple<int, int>>& sortedEdges)
{
    for (int i = 0; i < matrix.size(); i++)
    {
        for (int j = i; j < matrix.size(); j++)
        {
            if (matrix[i][j] > 0)
            {
                sortedEdges.insert(std::pair<int, std::tuple<int, int>>(matrix[i][j], std::tuple<int, int>(i, j)));
            }
        }
    }
}

bool GraphIsConnected(Matrix& matrix)
{
    for (int i = 0; i < matrix.size();i++)
    {
        std::set<int> visited;
        if (!pathExists(matrix, 0, 1, visited))
            return false;
    }
    return true;
}

int solve()
{
    // solve with Kruskal's algorithm for minimal spanning tree

    Matrix matrix = createMatrix(data);
    bool isConnected = GraphIsConnected(matrix);
    if (!isConnected)
		return -1;

    createSortedEdges(matrix, sortedEdges);
    int FullGraphSum = edgeSum(matrix);

    Matrix minGraph = createMinSpanningGraph(matrix);
    int minGraphSum = edgeSum(minGraph);

    int sumDiff = FullGraphSum - minGraphSum;
    // result: 259679

    return sumDiff;
}

int main()
{

    auto t1 = std::chrono::high_resolution_clock::now();
    int solution = solve();
    auto t2 = std::chrono::high_resolution_clock::now();
    auto microSec = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
    std::cout << "solution: " << solution << std::endl << "duration: " << microSec << " micro seconds (" << ms << " ms)" << std::endl;
}
