// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <iostream>
#include <chrono>
#include <cassert>
#include <vector>
#include <list>

const int MAXBLOCKBUFCOUNT = 1000;

const int MAXVALUE = 200;
std::vector<class block*> g_minBlocks(MAXVALUE + 1, (block*)nullptr);
int g_registerCount = 0;
std::list<block*> g_allBlocks;
std::list<std::list<block*>*> g_generations;

class block;
struct BlockCountRec {
    block* b;
    int count;
    };

class block {
public:
    block()
    {
        value = 1;
        multCount = 0;
        (void)registerBlock();
    }
    block(block* left, block* right) {
        assert(left->value >= right->value);
        value = left->value + right->value;
        this->left = left;
        this->right = right;
        multCount = -1;
        multCount = determineMultCount();
        (void)registerBlock();
        }
    block(block* left, block* right, int combinedMultCount) {
        assert(left->value >= right->value);
        value = left->value + right->value;
        this->left = left;
        this->right = right;
        multCount = combinedMultCount;
        (void)registerBlock();
    }
    int value;
    int multCount;
    
    static std::list<block*>* generateBlocks()
    {
        int skipCount = 0;
        std::list<block*> *newBlockList = new std::list<block*>();
        std::list<block*> *lastGeneration = g_generations.back();

        for (block* b : *lastGeneration)
        {
            if (b->multCount > g_minBlocks[b->value]->multCount)
            {
                skipCount += 1;
                if (skipCount % 1000 == 0)
                    std::cout << "skipCount: " << skipCount << std::endl;
                continue;
            }
            for (std::list<block*>* bl : g_generations)
            {
                for (block* b2 : *bl)
                {
                    if (b2->multCount > g_minBlocks[b->value]->multCount)
                    {
                        continue;
                    }
                    int _mc = determineCombinedMultCount(b, b2);
                    int b_val = b->value;
                    int b2_val = b2->value;
                    int val = b_val + b2_val;
                    if (val > MAXVALUE)
                        continue;
                    block* _b = g_minBlocks[val];
                    if (_b != nullptr)
                    {
                        if (_mc > _b->multCount)
                            continue;
                    }

                    if (b2_val > b_val)
                        newBlockList->push_back(new block(b2, b, _mc));
                    else
                        newBlockList->push_back(new block(b, b2, _mc));
                }
            }
        }
        g_generations.push_back(newBlockList);
        return newBlockList;

    }

    static int determineCombinedMultCount(block* b1, block* b2)
    {
        int blockCountIndex = 0;
        BlockCountRec bc[MAXBLOCKBUFCOUNT];
        return
            determineMultCount(bc, b1, blockCountIndex)
            +
            determineMultCount(bc, b2, blockCountIndex)
            + 1
            ;
    }

private:
    int determineMultCount()
	{
        BlockCountRec bc[MAXBLOCKBUFCOUNT];
        int blockCountIndex = 0;
        return determineMultCount(bc, this, blockCountIndex);
	}
    static int determineMultCount(BlockCountRec* bc, block* b, int& blockCountIndex)
    {
        if (b->multCount == 0)
		{
			return 0;
		}
        for (int i=0;i<blockCountIndex;i++)
        {
            assert(i < MAXBLOCKBUFCOUNT);
			if (bc[i].b == b)
			{
				bc[i].count += 1;
				return 0;
			}
		}
		bc[blockCountIndex].b = b;
		bc[blockCountIndex].count = 1;
		blockCountIndex++;
        return
            determineMultCount(bc, b->left, blockCountIndex)
            +
            determineMultCount(bc, b->right, blockCountIndex)
            + 1
            ;
    }

    bool registerBlock()
	{
        g_allBlocks.push_back(this);
        block* g_b = g_minBlocks[value];
        if (g_b == nullptr)
        {
			g_minBlocks[value] = this;
            g_registerCount += 1;
            return true;
        }
        else
        {
            if (multCount < g_b->multCount)
            {
                g_minBlocks[value] = this;
                return true;
            }
            else
				return false;
        }
	}

    block* left;
    block* right;

};


int solve()
{
    
    block* b1 = new block();
    std::list<block*>* rootBlocks = new std::list<block*>();
    rootBlocks->push_back(b1);
    g_generations.push_back(rootBlocks);

    int prevSum = 0;
    int sum = 0;

	bool done = false;
    while (!done)
    {
        std::list<block*>* newBlocks = block::generateBlocks();
        // std::cout << "newBlocks.size(): " << newBlocks->size() << std::endl;

        sum = 0;

        for (int i = 1; i <= MAXVALUE; i++)
        {
            block *_b = g_minBlocks[i];
            if (_b != nullptr)
                sum += g_minBlocks[i]->multCount;
        }
        std::cout << "sum:" << sum << std::endl;
        if (sum == prevSum)
            break;
        prevSum = sum;

        std::cout << "all blocks count: " << g_allBlocks.size() << std::endl;
    }

    for (; !g_allBlocks.empty(); g_allBlocks.pop_front())
    {
        delete g_allBlocks.front();
    }

    return sum;
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
