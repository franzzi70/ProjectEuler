

#include <iostream>
#include <string>
#include <time.h>
#include <algorithm>
#include <list>
#include <vector>

// C(n, r) and provide the current subset a of size r.
bool nextCombination(int n, int r, int* a);


bool nextCombination(int n, int r, int* a) {
    int lastNotEqualOffset = r - 1;
    while (a[lastNotEqualOffset] == n - r + (lastNotEqualOffset + 1)) {
        lastNotEqualOffset--;
    }
    if (lastNotEqualOffset < 0) {
        //std::cout << "the end\n";
        return false;
    }
    a[lastNotEqualOffset]++;
    for (int i = lastNotEqualOffset + 1; i < r; i++) {
        a[i] = a[lastNotEqualOffset] + (i - lastNotEqualOffset);
    }
    return true;
}



class Operation;
class Literal;


class Fraction {
public:
    int numerator;
    int denominator;
    Fraction() { numerator = 0; denominator = 1; };
    Fraction(const Fraction& other) { numerator = other.numerator; denominator = other.denominator; };
    Fraction(int num, int denom)
    {
        numerator = num;
        denominator = denom;
    };
};


class Expression
{
public:
    Expression() {};
    Expression(const Operation& op) {};
    Expression(const Literal& l) {};
    virtual ~Expression() {};
    virtual char nodeType() = 0;
    virtual Fraction eval() = 0;
    virtual int setTreeOperators(int index, std::string opString) = 0;
    virtual std::string toString() = 0;
};

class Operation : public Expression
{
public:
    char op;
    Expression* left;
    Expression* right;

    Operation(char opChar, Expression* leftExpr, Expression* rightExpr)
    {
        op = opChar;
        left = leftExpr;
        right = rightExpr;
    }

    Operation(const Operation& other)
    {
        op = other.op;
        left = other.left;
        right = other.right;
    }

    ~Operation()
    {
        if (left->nodeType() == 'o')
            delete left;
        if (right->nodeType() == 'o')
            delete right;
    }

    Fraction eval() {
        Fraction lEval = left->eval();
        Fraction rEval = right->eval();
        if (op == '*')
            return Fraction(lEval.numerator * rEval.numerator, lEval.denominator * rEval.denominator);
        if (op == '+')
        {
            if (lEval.denominator == rEval.denominator)
                return Fraction(lEval.numerator + rEval.numerator, lEval.denominator);
            else
            {
                return Fraction(
                    lEval.numerator * rEval.denominator + rEval.numerator * lEval.denominator,
                    lEval.denominator * rEval.denominator
                );
            }
        }
        if (op == '-')
        {
            if (lEval.denominator == rEval.denominator)
                return Fraction(lEval.numerator - rEval.numerator, lEval.denominator);
            else
            {
                return Fraction(
                    lEval.numerator * rEval.denominator - rEval.numerator * lEval.denominator,
                    lEval.denominator * rEval.denominator
                );
            }
        }
        if (op == '/')
        {
            return Fraction(lEval.numerator * rEval.denominator, lEval.denominator * rEval.numerator);
        }
        // should ne be reached, return -1 in this case.
        return Fraction(-1,1);
    }
    char nodeType()
    {
        return 'o';
    }

    int setTreeOperators(int index, std::string opString)
    {
        int newIndex = index;
        if (left->nodeType() == 'o')
        {
            newIndex = left->setTreeOperators(index, opString); 
        }
        op = opString[newIndex];
        newIndex += 1;
        if (right->nodeType() == 'o')
        {
            newIndex = right->setTreeOperators(newIndex, opString);
        }
        return newIndex;
    }
    std::string toString()
    {
        return std::string("(")
            .append(left->toString())
            .append(") ")
            .append(1, op)
            .append(" (")
            .append(right->toString())
            .append(std::string(")"))
            ;
    }
};

class Literal : public Expression
{
public:
    Literal(int n)
    {
        num = n;
    }
    Literal(Literal& other)
    {
        num = other.num;
    }
    Expression& operator = (const int n)
    {
        num = n;
        return *this;
    }
    int num;
    Fraction eval()
    {
        return Fraction(num, 1);
    }
    char nodeType()
    {
        return 'l';
    }
    int setTreeOperators(int index, std::string opString)
    {
        return index;
    }
    std::string toString()
    {
        return std::to_string(num);
    }

};



const int maxHitTest = 1000;
bool enterHitNumber(Fraction& f, std::vector<bool>& hitNumbers)
{
    int num = f.numerator;
    int denom = f.denominator;
    if (denom != 1)
    {
        if ((num % denom) != 0)
        {
            return false;
        }
        else
        {
            num = num / denom;
        }
    }
    if (num >= 1 && num <= maxHitTest)
    {
        hitNumbers[num] = true;
        return true;
    }
    return false;
}

int countContinousHits(std::vector<bool>& hitNumbers)
{
    //int highest = 0;
    for (int i = 1; i < maxHitTest; i++)
    {
        if (!hitNumbers[i])
            return i - 1;
    }
    return 0;
}

int solve()
{
    std::vector<bool> hitNumbers(maxHitTest);
    int currentHighestHitCount = 0;
    int setForHighestHitCoverage[] = { 0,0,0,0 };

    std::list<Operation*> exprStructures;
    std::vector<Literal*> literals(4);
    for (int i = 0; i < 4; i++)
    {
        literals[i] = new Literal(i + 1);
    }

    // 3 op bracket at start:

    //      2 brackets within  3 first
    Operation* op1 = new Operation('+', literals[0], literals[1]);
    Operation* op2 = new Operation('+', op1, literals[2]);
    Operation* op3 = new Operation('+', op2, literals[3]);
    exprStructures.push_back(op3);

    //      2 brackets within  3 last
    op1 = new Operation('+', literals[1], literals[2]);
    op2 = new Operation('+', literals[0], op1);
    op3 = new Operation('+', op2, literals[3]);
    exprStructures.push_back(op3);

    // 3 op bracket at end:

    //      2 brackets within  3 first
    op1 = new Operation('+', literals[1], literals[2]);
    op2 = new Operation('+', op1, literals[3]);
    op3 = new Operation('+', op2, literals[0]);
    exprStructures.push_back(op3);

    //      2 brackets within  3 last
    op1 = new Operation('+', literals[2], literals[3]);
    op2 = new Operation('+', literals[1], op1);
    op3 = new Operation('+', op2, literals[0]);
    exprStructures.push_back(op3);

    // 2 brackets structure:
    op1 = new Operation('+', literals[0], literals[1]);
    op2 = new Operation('+', literals[2], literals[3]);
    op3 = new Operation('+', op1, op2);
    exprStructures.push_back(op3);


    std::vector<std::string>operatorCombinations(64);
    std::vector<char>operators{'+', '-', '*', '/'};

    for (int i = 0; i < 64; i++)
    {
        std::string opStr = "";
        opStr.append(1, operators[(i / 16) % 4])
            .append(1, operators[(i / 4) % 4])
            .append(1, operators[i % 4]);
        operatorCombinations[i] = opStr;
    }


    // for each combination of 4 from 1..9
    // for each expression structure
    // for each operator sequence
    // for each permutatgion of a combination
    //      evaluate expression and put into checkArray to find longes sequence from 1 to n

    int a[] = { 1,2,3,4 };
    int length = sizeof(a) / sizeof(*a);
    do {
        
        for (int i = 0 ; i < maxHitTest; i++)
        {
            hitNumbers[i] = false;
        }

        for (auto exprIt = exprStructures.begin(); exprIt != exprStructures.end(); exprIt++)
        {
            for (int opIx = 0; opIx < 64 ; opIx++)
            {
                (void)(*exprIt)->setTreeOperators(0, operatorCombinations[opIx]);
                do {
                    for (int iL = 0; iL < 4; iL++)
                    {
                        literals[iL]->num = a[iL];
                    }

                    Fraction ev = (*exprIt)->eval();
                    //std::string str = (*exprIt)->toString();
                    //std::cout << ev.numerator << "/" << ev.denominator << std::endl;
                    /*bool hit = */ (void)enterHitNumber(ev, hitNumbers);
                } while (std::next_permutation(a, a + 4));
            }
        }

        // count hitnumbers:
        int hitCount = countContinousHits(hitNumbers);
        if (hitCount > currentHighestHitCount)
        {
            currentHighestHitCount = hitCount;
            for (int i = 0; i < 4; i++)
            {
                setForHighestHitCoverage[i] = a[i];
            }

        }

    } while (nextCombination(9, length, a));

    return
        setForHighestHitCoverage[0] * 1000
        + setForHighestHitCoverage[1] * 100
        + setForHighestHitCoverage[2] * 10
        + setForHighestHitCoverage[3];

}


int main()
{
    time_t t1 = clock();
    int solution = solve();
    time_t t2 = clock();
    int ms = (int)(t2 - t1) * 1000 / CLOCKS_PER_SEC;

    std::cout << "solution: " << solution << std::endl << "duration: " << ms << " ms" << std::endl;
}
