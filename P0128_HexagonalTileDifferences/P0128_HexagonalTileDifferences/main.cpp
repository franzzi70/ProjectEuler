// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <iostream>
#include <chrono>
#include "primes.h"

const int MAXPRIME = 1'000'000;
primes::PrimesSieve g_primes(MAXPRIME);

class Field
{
public:
    Field()
    {
        m_layerCount = 0;
        m_lastNumber = 1;
        m_layers.push_back(1);
        m_foundCount = 0;
    }

    int addLayer() {
        m_layerCount += 1;
        int newCount = m_layerCount * 6;
        m_layers.push_back(m_lastNumber+1);
        m_lastNumber += newCount;
        return m_layerCount;
    }

    int diffIsPrime(__int64 a, __int64 b)
    {
        // std::cout << a << " - " << b << std::endl;
        int diff = (int)(a - b);
        if (diff < 0)
        {
            diff = -diff;
        }
        return g_primes.isPrime(diff) ? 1 : 0;
    }

    __int64 getTileValue(int layer, int pos)
    {
        __int64 layerStartNumber = m_layers[layer];
        int layerSize = 6 * layer;
        if (pos < 0)
            pos += layerSize;
        if (pos < 0 || pos >= layerSize)
            pos -= layerSize;
        return layerStartNumber + pos;
    }

    void find3PrimeDiff(int layer)
    {
        if (layer == 0)
        {
            m_foundCount += 1;
            m_foundTiles.push_back(1);
            return;
        }

        int segPos = 0;
        int segPosPrev = 0;
        int segPosNext = 0;

        /*
        for (int i = 0; i < 6; i++)
        {
            for (int j = 0; j < layer; j++)
            {
                int _foundCount = 0;
                __int64 tileValue = getTileValue(layer, segPos + j);
                //if (tileValue == 19)
                //    std::cout << tileValue << std::endl;

                _foundCount += diffIsPrime(tileValue, getTileValue(layer, segPos + j + 1));
                _foundCount += diffIsPrime(tileValue, getTileValue(layer, segPos + j - 1));
                _foundCount += diffIsPrime(tileValue, getTileValue(layer - 1, segPosPrev + j));
                if (j > 0)
                    _foundCount += diffIsPrime(tileValue, getTileValue(layer - 1, segPosPrev + j - 1));
                _foundCount += diffIsPrime(tileValue, getTileValue(layer + 1, segPosNext + j));
                _foundCount += diffIsPrime(tileValue, getTileValue(layer + 1, segPosNext + j + 1));
                if (j == 0)
                    _foundCount += diffIsPrime(tileValue, getTileValue(layer + 1, segPosNext + j - 1));

                if (_foundCount == 3)
                {
                    m_foundCount += 1;
                    m_foundTiles.push_back(tileValue);
                    std::cout << "found field " << m_foundCount << ": " << tileValue << std::endl;
                }
            }
            segPos += layer;
            segPosPrev += layer - 1;
            segPosNext += layer + 1;
        }
        */

        // only corners can be prime (2 outside or inside are odd and even, just yield two primes and neighbors on layer have only 1 diff)
        // therefore only check corners:
        // more investigation leads to only checking two tiles around first corner per layer.

        int _foundCount = 0;
        __int64 tileValue = getTileValue(layer, 0);

        int layerSize = 6 * layer;
        int nextLayerSize = layerSize + 6;
        int prevLayerSize = layerSize - 6;

        //if (tileValue == 8)
        //    std::cout << "8" << std::endl;

        _foundCount += diffIsPrime(tileValue, getTileValue(layer, layerSize - 1));
        _foundCount += diffIsPrime(tileValue, getTileValue(layer - 1, 0));
        _foundCount += diffIsPrime(tileValue, getTileValue(layer + 1, 0));
        _foundCount += diffIsPrime(tileValue, getTileValue(layer + 1, 1));
        _foundCount += diffIsPrime(tileValue, getTileValue(layer + 1, nextLayerSize - 1));

        if (_foundCount == 3)
        {
            m_foundCount += 1;
            m_foundTiles.push_back(tileValue);
            std::cout << "found field " << m_foundCount << ": " << tileValue << std::endl;
        }

        _foundCount = 0;
        tileValue = getTileValue(layer, layerSize - 1);

        _foundCount += diffIsPrime(tileValue, getTileValue(layer, 0));
        _foundCount += diffIsPrime(tileValue, getTileValue(layer - 1, 0));
        if (layer == 1)
            _foundCount += diffIsPrime(tileValue, getTileValue(layer + 1, nextLayerSize - 3));
        else
            _foundCount += diffIsPrime(tileValue, getTileValue(layer - 1, -1));
        _foundCount += diffIsPrime(tileValue, getTileValue(layer + 1, nextLayerSize - 1));
        _foundCount += diffIsPrime(tileValue, getTileValue(layer + 1, nextLayerSize - 2));

        if (_foundCount == 3)
        {
            m_foundCount += 1;
            m_foundTiles.push_back(tileValue);
            std::cout << "found field " << m_foundCount << ": " << tileValue << std::endl;
        }


    }


    std::vector<__int64> m_layers;
    int m_layerCount;
    __int64 m_lastNumber;

    std::vector<__int64> m_foundTiles;
    int m_foundCount;
};




__int64 solve()
{
    Field f;

    int maxLayerCount = 1'000'000;

    for (int i=1;i<= maxLayerCount;i++)
    {
        int layerCount = f.addLayer();
        f.find3PrimeDiff(layerCount-1);
        if (layerCount % 1000 == 0)
            std::cout << "layer: " << layerCount << std::endl;
        if (f.m_foundCount >= 2000)
			break;
	}
    if (f.m_foundCount < 2000)
    {
        std::cout << "not enough found: " << f.m_foundCount << std::endl;
        std::cout << "last found: " << f.m_foundTiles[f.m_foundCount - 1] << std::endl;
        return 0;
    }
	else
		std::cout << "found enough: " << f.m_foundCount << std::endl;

    return f.m_foundTiles[1999];
}

int main()
{

    auto t1 = std::chrono::high_resolution_clock::now();
    __int64 solution = solve();
    auto t2 = std::chrono::high_resolution_clock::now();
    auto microSec = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
    std::cout << "solution: " << solution << std::endl << "duration: " << microSec << " micro seconds (" << ms << " ms)" << std::endl;
}
