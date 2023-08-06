// P62_CubicPermutationsCPP.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <sstream>
#include <map>
#include <list>
#include <string.h>
#include <string>
#include <algorithm>



class cubePair {
public:
    __int64 num;
    __int64 cube;
    cubePair(__int64 _num, __int64 _cube)
    {
        num = _num;
        cube = _cube;
    }
};

class node {
public:
    std::list<cubePair> m_numbers;

    node()
    {
    }
    
    node(__int64 num, __int64 cube)
    {
        add(num, cube);
    }

    void add(__int64 num, __int64 cube)
    {
        m_numbers.insert(m_numbers.end(), cubePair(num, cube));
    }

};


std::map<std::string,node> cubes;


std::string createMapString(__int64 num)
{
    std::ostringstream convert;
    convert << num;
    std::string s(convert.str());
    sort(s.begin(), s.end());
    return s;
}


bool check(__int64 i)
{
    //if (i == 405)
    //{
    //    std::cout << "405" << std::endl;
    //}
    __int64 icube = i * i * i;
    // std::map<int, node>iterator it;// = cubes.find(icube);
    std::string keyString = createMapString(icube);

    auto it = cubes.find(keyString);
    
    if (it == cubes.end())
    {
        cubes[keyString] = node(i,icube);
    }
    else
    {
        auto& pair = *it;
        node& n = pair.second;
        n.add(i, icube);
        if (n.m_numbers.size() >= 5)
        {
            // found !!
            std::cout << n.m_numbers.begin()->cube << std::endl;
            return true;
        }
    }
    return false;
}

int solve()
{
    bool found = false;
    for (__int64 i=0; !found ; i++)
    {
        //std::cout << i << std::endl;
        found = check(i);
    }
    return 0;
}

int main()
{
    clock_t t = clock();
    solve();
    printf("\nTime taken: %.4fs\n", (float)(clock() - t) / CLOCKS_PER_SEC);
}

