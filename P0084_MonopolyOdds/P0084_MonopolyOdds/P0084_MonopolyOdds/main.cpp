// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <iostream>
#include <chrono>
#include <random>
#include <algorithm>
#include <vector>
#include <map>
#include <string>
#include "fraction.h"
#include "main.h"

typedef Fraction<int> frac_t;

int g_diceSideCount = 4;


static std::default_random_engine g_diceGenerator;
int getRandomNumber(int low, int high)
{
    //static generator 
    static bool initialized = false;

    if (!initialized)
    {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        g_diceGenerator.seed(seed);
        initialized = true;
    }

    std::uniform_int_distribution<int> distribution(low, high);
    return distribution(g_diceGenerator);
}




enum class ChestType : char {
    Other,
    GotoStart,
    GotoJail
};

ChestType chestCardStack[]{
    ChestType::Other,
    ChestType::Other,
    ChestType::Other,
    ChestType::Other,
    ChestType::Other,
    ChestType::Other,
	ChestType::Other,
    ChestType::Other,
    ChestType::Other,
    ChestType::Other,
    ChestType::Other,
    ChestType::Other,
    ChestType::Other,
    ChestType::Other,
    ChestType::GotoStart,
    ChestType::GotoJail
    };

    

 enum class ChanceType : char {
    Other,
    GotoStart,
    GotoJail,
    GotoC1,
    GotoE3,
    GotoH2,
    GotoR1,
    GotoNextR,
    GotoNextU,
    GotoBack3
};

ChanceType chanceCardStack[] {
    ChanceType::Other,
    ChanceType::Other,
    ChanceType::Other,
    ChanceType::Other,
    ChanceType::Other,
    ChanceType::Other,
    ChanceType::GotoStart,
    ChanceType::GotoJail,
    ChanceType::GotoC1,
    ChanceType::GotoE3,
    ChanceType::GotoH2,
    ChanceType::GotoR1,
    ChanceType::GotoNextR,
    ChanceType::GotoNextR,
    ChanceType::GotoNextU,
    ChanceType::GotoBack3
    };


enum class FieldType : char {
    Common,
    Start,
    Jail,
    Chance,
    CommunityChest,
    Rail,
    UtilityCompany,
    GoToJail
};


class Field
{
public:
    Field(FieldType type) : m_type(type) {};
    FieldType m_type;
};

Field fields[]{
    {FieldType::Start},
    {FieldType::Common},
    {FieldType::CommunityChest},
    {FieldType::Common},
    {FieldType::Common},
    {FieldType::Rail},
    {FieldType::Common},
    {FieldType::Chance},
    {FieldType::Common},
    {FieldType::Common},
    {FieldType::Jail},
    {FieldType::Common},
    {FieldType::UtilityCompany},
    {FieldType::Common},
    {FieldType::Common},
    {FieldType::Rail},
    {FieldType::Common},
    {FieldType::CommunityChest},
    {FieldType::Common},
    {FieldType::Common},
    {FieldType::Common},
    {FieldType::Common},
    {FieldType::Chance},
    {FieldType::Common},
    {FieldType::Common},
    {FieldType::Rail},
    {FieldType::Common},
    {FieldType::Common},
    {FieldType::UtilityCompany},
    {FieldType::Common},
    {FieldType::GoToJail},
    {FieldType::Common},
    {FieldType::Common},
    {FieldType::CommunityChest},
    {FieldType::Common},
    {FieldType::Rail},
    {FieldType::Chance},
    {FieldType::Common},
    {FieldType::Common},
    {FieldType::Common}
};

frac_t prob[40];

//frac_t prob1[40][40];

int rollDice(int numberofDices, int diceSideCount)
{
    int sum = 0;
    for (int i = 0; i < numberofDices; i++)
    {
        sum += getRandomNumber(1, diceSideCount);
    }
    return sum;
}

void init()
{
    // shuffle chestCardStack
    for (int i = 0; i < 16; ++i)
	{
		int r = getRandomNumber(0, 15);
        std::swap(chestCardStack[i], chestCardStack[r]);
	}

    // shuffle changeCardStack
    for (int i = 0; i < 16; ++i)
    {
        int r = getRandomNumber(0, 15);
		std::swap(chanceCardStack[i], chanceCardStack[r]);
    }
}

ChanceType fetchChanceCard()
{
    static int index = 0;
    ChanceType card = chanceCardStack[index];
    index = (index + 1) % 16;
    return card;
}

ChestType fetchChestCard()
{
    static int index = 0;
    ChestType card = chestCardStack[index];
    index = (index + 1) % 16;
    return card;
}

int getTargetField(int fieldNum)
{
    int targetField = fieldNum;
    switch (fields[fieldNum].m_type)
    {
	case FieldType::Chance:
        switch (fetchChanceCard())
        {
		case ChanceType::GotoStart:
			targetField = 0;
			break;
		case ChanceType::GotoJail:
			targetField = 10;
			break;
		case ChanceType::GotoC1:
			targetField = 11;
			break;
		case ChanceType::GotoE3:
			targetField = 24;
			break;
		case ChanceType::GotoH2:
			targetField = 39;
			break;
		case ChanceType::GotoR1:
			targetField = 5;
			break;
		case ChanceType::GotoNextR:
            if (fieldNum == 7)
            {
				targetField = 15;
			}
            else if (fieldNum == 22)
            {
				targetField = 25;
			}
            else if (fieldNum == 36)
            {
				targetField = 5;
			}
			break;
		case ChanceType::GotoNextU:
            if (fieldNum == 7 || fieldNum == 36)
            {
				targetField = 12;
			}
            else if (fieldNum == 22)
            {
				targetField = 28;
			}
			break;
		case ChanceType::GotoBack3:
			targetField = fieldNum - 3;
			break;
		default:
			break;
		}
		break;
	case FieldType::CommunityChest:
        switch (fetchChestCard())
        {
		case ChestType::GotoStart:
			targetField = 0;
			break;
		case ChestType::GotoJail:
			targetField = 10;
			break;
		default:
			break;
		}
		break;
	case FieldType::GoToJail:
		targetField = 10;
		break;
	default:
		break;
	}
	return targetField;
}



void test()
{
    // assign equal probility for all fields in array prob
    frac_t probEqual(1, 40);
    for (int i = 0; i < 40; ++i)
    {
        prob[i] = probEqual;
    }

    // sum all probabilities
    frac_t sum0(0, 1);
    for (int i = 0; i < 40; ++i)
    {
        sum0 += prob[i];
    }


    // shift from probabilities GotoJail to Jail
    prob[30] += prob[10];
    prob[10] = frac_t(0, 1);

    // shift from probabilities Chance to according fields
    prob[7] *= frac_t(6, 16);
    prob[22] *= frac_t(6, 16);
    prob[36] *= frac_t(6, 16);

    //// chance goto start
    //prob[0] += probEqual * frac_t(3, 16);
    //// chance goto jail
    //prob[10] += probEqual * frac_t(3, 16);
    //// goto C1
    //prob[11] += probEqual * frac_t(3, 16);
    //// goto E3
    //prob[24] += probEqual * frac_t(3, 16);
    //// goto H2
    //prob[39] += probEqual * frac_t(3, 16);
    //// go to R1
    //prob[5] += probEqual * frac_t(3, 16);
    //// go to next railroad
    //prob[5] += probEqual * frac_t(2, 16);
    //prob[15] += probEqual * frac_t(2, 16);
    //prob[25] += probEqual * frac_t(2, 16);
    //// go to next utility company
    //prob[12] += probEqual * frac_t(2, 16);
    //prob[28] += probEqual * frac_t(1, 16);
    //// go back 3 squares
    //prob[4] += probEqual * frac_t(1, 16);
    //prob[19] += probEqual * frac_t(1, 16);
    //prob[33] += probEqual * frac_t(1, 16);

    // community chest
    prob[2] *= frac_t(14, 16);
    prob[17] *= frac_t(14, 16);
    prob[33] *= frac_t(14, 16);

    prob[0] += probEqual * frac_t(3, 16);
    prob[10] += probEqual * frac_t(3, 16);

    // sum all probabilities
    frac_t sum(0, 1);
    for (int i = 0; i < 40; ++i)
    {
        sum += prob[i];
    }
}


std::string simulate(int maxRoundCount)
{
    init();

    // test();
    int currentField = 0;
    int nextField = 0;
    int roundCount = 0;
    int doubleCount = 0;
    std::vector<int> visitCount(40, 0);

    while (roundCount < maxRoundCount)
    {
		// roll dice
		int dice1 = getRandomNumber(1, 4);
		int dice2 = getRandomNumber(1, 4);
		int diceSum = dice1 + dice2;

		// check double
        if (dice1 == dice2)
        {
			doubleCount++;
		}
        else
        {
			doubleCount = 0;
		}

		// goto jail
        if (doubleCount == 3)
        {
			currentField = 10;
			doubleCount = 0;
		}
        else
        {
			// move to next field
			nextField = (currentField + diceSum) % 40;

			// check goto jail
            if (nextField == 30)
            {
				currentField = 10;
			}
            else
            {
				// check chance or community chest
                if (fields[nextField].m_type == FieldType::Chance || fields[nextField].m_type == FieldType::CommunityChest)
                {
					// get target field
					int targetField = getTargetField(nextField);
					// move to target field
					currentField = targetField;
				}
                else
                {
					// move to next field
					currentField = nextField;
				}
			}
		}

		// count field visits
        visitCount[currentField] += 1;

		// next round
		roundCount += 1;
	}

    // create 6 digit modal string
    std::string modalString = "";
    for (int i = 0; i < 3; ++i)
    {
		int maxIndex = std::max_element(visitCount.begin(), visitCount.end()) - visitCount.begin();
        if (maxIndex < 10)
        {
            modalString += "0";
            modalString += std::to_string(maxIndex);
        }
        else
		    modalString += std::to_string(maxIndex);
		visitCount[maxIndex] = 0;
	}

    return modalString;
}

std::string solve()
{
    const int simulationCount = 10000;
    // define modalstringMap
    std::map<std::string, int> modalStringMap;

    for (int i = 0; i < simulationCount; i++)
    {
        std::string modalString = simulate(1000);
        // count occurances of modal string
        auto it = modalStringMap.find(modalString);
        if (it != modalStringMap.end())
        {
			it->second += 1;
		}
        else
        {
			modalStringMap.insert(std::make_pair(modalString, 1));
		}
    }
   
    // solution: "101524"


 //   std::vector<std::pair<std::string, int>> modalStringVector(modalStringMap.begin(), modalStringMap.end());
 //   std::sort(modalStringVector.begin(), modalStringVector.end(), [](const std::pair<std::string, int>& p1, const std::pair<std::string, int>& p2) { return p1.second > p2.second; });
 //   // print modal string vector
 //   for (auto& p : modalStringVector)
 //   {
	//	std::cout << p.first << " " << p.second << std::endl;
	//}


    
    
    // return modal string with highest occurance
    auto it = std::max_element(
        modalStringMap.begin(),
        modalStringMap.end(),
            [](const std::pair<std::string, int>& p1, const std::pair<std::string, int>& p2) { return p1.second < p2.second; });
    return it->first;

}

int main()
{

    auto t1 = std::chrono::high_resolution_clock::now();
    std::string solution = solve();
    auto t2 = std::chrono::high_resolution_clock::now();
    auto microSec = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
    std::cout << "solution: " << solution << std::endl << "duration: " << microSec << " micro seconds (" << ms << " ms)" << std::endl;
}
