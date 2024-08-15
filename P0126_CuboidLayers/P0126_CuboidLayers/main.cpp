// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <iostream>
#include <chrono>
#include <vector>
#include <list>
#include <fstream>

const int MAXCUBECOUNT = 20'000;

class Cube {
public:
    Cube(int x, int y, int z) : x(x), y(y), z(z) {}
    int x, y, z;
};

class Cuboid
{
public:
    Cuboid(int x, int y, int z) : x(x), y(y), z(z) {}
    int x, y, z;
    int surfaceArea() { return 2 * (x * y + y * z + z * x); }
};

class CubeSpace
{
public:
	CubeSpace(Cuboid& innerCuboid, int padding)
    {
        xsize = innerCuboid.x + padding * 2;
        ysize = innerCuboid.y + padding * 2;
        zsize = innerCuboid.z + padding * 2;
        zFactor = xsize * ysize;
        space = std::vector<char>(xsize * ysize * zsize, 0);
        _placeCuboid(innerCuboid);
    }
    int xsize, ysize, zsize;
    std::vector<char> space;
    std::list<std::list<Cube>> cubesLayers;
    char lastLayerNum = 1;

    int addLayer()
	{
        char newLayerNum = lastLayerNum + 1;
        std::list<Cube> newLayer;
        std::list<Cube> lastLayer = cubesLayers.back();
        for (auto cube : lastLayer)
        {
			int x = cube.x;
			int y = cube.y;
			int z = cube.z;
            int spaceOffset = x + y * xsize + z * zFactor;
            if (space[spaceOffset-1] == 0)
			{
                space[spaceOffset-1] = newLayerNum;
                newLayer.push_back(Cube(x-1, y, z));
            }
            if (space[spaceOffset + 1] == 0)
            {
                space[spaceOffset + 1] = newLayerNum;
                newLayer.push_back(Cube(x+1, y, z));
            }
            if (space[spaceOffset - xsize] == 0)
			{
                space[spaceOffset - xsize] = newLayerNum;
                newLayer.push_back(Cube(x, y-1, z));
			}
            if (space[spaceOffset + xsize] == 0)
			{
                space[spaceOffset + xsize] = newLayerNum;
				newLayer.push_back(Cube(x, y+1, z));
			}
            if (space[spaceOffset - zFactor] == 0)
            {
                space[spaceOffset - zFactor] = newLayerNum;
                newLayer.push_back(Cube(x, y, z - 1));
            }
            if (space[spaceOffset + zFactor] == 0)
			{
                space[spaceOffset + zFactor] = newLayerNum;
                newLayer.push_back(Cube(x, y, z + 1));
			}
        }
		cubesLayers.push_back(newLayer);
        lastLayerNum = newLayerNum;
        return (int)newLayer.size();
	}
private:
    int zFactor = 0;
    void _placeCuboid(Cuboid &innerCuboid)
    {
        lastLayerNum = 1;
        int xmid = xsize / 2;
        int ymid = ysize / 2;
        int zmid = zsize / 2;
        int xstart = xmid - innerCuboid.x / 2;
        int ystart = ymid - innerCuboid.y / 2;
        int zstart = zmid - innerCuboid.z / 2;
        std::list<Cube> layer;
        for (int x = xstart; x < xstart + innerCuboid.x; x++)
		{
			for (int y = ystart; y < ystart + innerCuboid.y; y++)
			{
                int zFactor = xsize * ysize;
				for (int z = zstart; z < zstart + innerCuboid.z; z++)
				{
                    placeCube(x, y, z, lastLayerNum, layer);
				}
			}
		}
        cubesLayers.push_back(layer);
    }

    void placeCube(int spaceOffset, int x, int y, int z, char layerNum, std::list<Cube> &layer)
	{
		space[spaceOffset] = layerNum;
		layer.push_back(Cube(x, y, z));
	}

    void placeCube(int x, int y, int z, int layerNum, std::list<Cube>& layer)
    {
        space[x + y * xsize + z * zFactor] = layerNum;
        layer.push_back(Cube(x, y, z));
    }
};

int cubeCount_old(int x, int y, int layers)
{
    int sf0 = 2 * (x * y + x + y);
    if (layers == 0)
        return sf0;

    int t1 = 2 * (x * y + x + y);
    int t2 = layers * 4 * (x + y);
    int t3 = 4 * (layers * (layers - 1) + layers);
    return t1 + t2 + t3;
}

int cubeCount(int x, int y, int layers)
{
    int sf0 = 2 * (x * y + x + y);
    if (layers == 0)
        return sf0;
    
    int t1 = 2 * (x * y + x + y);
    //int t2 = layers * 4 * (x + y);
    //int t3 = 4 * (layers * (layers - 1) + layers);
    //int t23 = 4 * (layers * (layers - 1) + layers * ( x + y + 1));
    int t23 = 4 * (layers * (layers + x + y));
    return t1 + t23;
}

int cubeCount(int x, int y, int z, int layers)
{
    int t1 = 2 * (x * (y + z) + y * z);
    if (layers == 0)
        return t1;

    int t2 = layers * 4 * (x + y + z);
    int t3 = 4 * (layers * (layers - 1));
    return t1 + t2 + t3;
}

void test()
{
    //Cuboid c1(3, 2, 1);
    //CubeSpace cs(c1);
    //int blockCount = cs.addLayer();
    //std::cout << blockCount << std::endl;

    //int blockCount2 = cs.addLayer();
    //std::cout << blockCount2 << std::endl;

    //int blockCount3 = cs.addLayer();
    //std::cout << blockCount3 << std::endl;

    //for (int i = 0; i < 15; i++)
    //{
    //    int blockCount = cs.addLayer();
    //    std::cout << blockCount << std::endl;
    //}

    //for (int i = 0; i < 15; i++)
    //{
    //    int c = cubeCount(3, 2, i);
    //    std::cout << c << std::endl;
    //}

    __int64 sum = 0;
    for (int j = 0; j < 1000'000; j++)
    {
        for (int i = 0; i < 1000; i++)
        {
            int c = cubeCount(3, 2, i);
            sum += c;
            // std::cout << c << std::endl;
        }
    }
    std::cout << sum << std::endl;
}

void writeToFile(std::vector<int>counts, std::string filename)
{
    std::ofstream file;
	file.open(filename);
	for (int i = 0; i < MAXCUBECOUNT; i++)
	{
		file << i << "," << counts[i] << std::endl;
	}
	file.close();
}

int solve()
{
     //test();
     //return 0;

    int maxSideLen = MAXCUBECOUNT / 4 + 2;

    std::vector<int> counts (MAXCUBECOUNT+1,0);

    __int64 calculationCount = 0;
    for (int x = 1; x <= maxSideLen; x++)
    {
        int maxY = x;
        //std::cout << "x: " << x << std::endl;
        for (int y = 1; y <= maxY; y++)
        {
            int maxZ = y;
            //std::cout << "y: " << y << std::endl;
            for (int z = 1; z <= maxZ; z++)
            {
                int layers = 0;
                while (true)
                {
                    int c = cubeCount(x, y, z, layers);
                    calculationCount += 1;
                    if (c > MAXCUBECOUNT)
                    {
                        // major performance improvement by pruning y- and z- runs
                        // (about 3000 times faster: 6ms instead of 18000 ms)
                        if (layers == 0)
                        {
                            maxZ = z;
                            if (z == 1)
                                maxY = y;
                        }
                        break;
                    }
                    counts[c] += 1;
                    layers += 1;
                }
            }
        }
    }

    std::cout << "calculation count: " << calculationCount << std::endl;

    //for (int x = 1; x <= maxSideLen; x++)
    //{
    //    std::cout << "x: " << x << std::endl;
    //    for (int y = 1; y <= x; y++)
    //    {
    //        std::cout << "y: " << y << std::endl;
    //        Cuboid c1(x, y, 1);
    //        CubeSpace cs(c1, 50);
    //        int layers = 0;
    //        while (true)
    //        {
    //            //int c = cubeCount(x, y, layers);
    //            int c = cs.addLayer();

    //            if (c > MAXCUBECOUNT)
    //                break;
    //            counts[c] += 1;
    //            layers += 1;
    //        }
    //    }
    //}

    //std::string filename = "D:\\FZROOT\\tmp\\CuboidLayers\\cubecounts_calc3_20000.csv";
    //writeToFile(counts, filename);

    for (int i=0;i<MAXCUBECOUNT;i++)
	{
		if (counts[i] == 1000)
			return i;
	}

    return 0;
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
