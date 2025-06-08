// ProjectEulerCppTemplate.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <chrono>
#include <set>


//#define TARGETSUM 15
//#define MINTOTARGET (TARGETSUM-9)

#undef DEBUG_REF
#undef DEBUG_MISSING_SYMMETRIES

std::set<uint64_t> solutions;
int64_t rejection_count = 0;

union square{
    uint8_t digits[16];
    uint64_t numbers[2];
    };

bool sq_equal(square a, square b)
{
    bool result =
        a.numbers[0] == b.numbers[0]
        &&
        a.numbers[1] == b.numbers[1]
        ;
    return result;
}

void sq_transpose(square src, square& dest)
{
    for (uint32_t row = 0, rowIx =0; row < 4; row ++)
    {
        for (uint32_t col = 0, colIx = 0; col < 4; col++)
        {
            dest.digits[rowIx + col] = src.digits[colIx + row];
            colIx += 4;
        }
        rowIx += 4;
    }
}

void sq_mirror_x(square src, square& dest)
{
    // mirror on vertical axis
    for (uint32_t rowIx = 0; rowIx < 16; rowIx += 4)
    {
        for (uint32_t col = 0; col < 4; col++)
        {
            dest.digits[rowIx + col] = src.digits[rowIx + 3 - col];
        }
    }
}

void sq_mirror_y(square src, square& dest)
{
    // mirror on horizontal axis
    for (uint32_t col = 0; col < 4; col += 1)
    {
        for (uint32_t rowIx = 0; rowIx < 16; rowIx += 4)
        {
            dest.digits[rowIx + col] = src.digits[12 - rowIx + col];
        }
    }
}

void sq_print(square sq)
{
	// print the square in a 4x4 grid format
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            std::cout << (int)sq.digits[i * 4 + j] << " ";
        }
        std::cout << std::endl;
    }
	std::cout << std::endl;
}

uint64_t sq_pack(square sq)
{
    return sq.numbers[0] + (sq.numbers[1] << 4);
}

void sq_unpack(int64_t packed, square& dest)
{
    uint64_t mask1 = 0x0f0f0f0f0f0f0f0f;
    uint64_t mask2 = 0xf0f0f0f0f0f0f0f0;
    dest.numbers[0] = packed & mask1;
    dest.numbers[1] = (packed & mask2) >> 4;
}

void ql_generate_equals(square sq)
{
	square equals[8];
    equals[0] = sq;
    sq_transpose(equals[0], equals[1]);
	sq_mirror_x(equals[0], equals[2]);
	sq_mirror_x(equals[1], equals[3]);
	sq_mirror_y(equals[0], equals[4]);
	sq_mirror_y(equals[1], equals[5]);
	sq_mirror_y(equals[2], equals[6]);
    sq_mirror_y(equals[3], equals[7]);
    // print all the squares
    for (int i = 0; i < 8; i++)
    {
        std::cout << "Square " << i << ":" << std::endl;
        sq_print(equals[i]);
	}
}

void test()
{
    square sq;
    sq.numbers[0] = 0;
    sq.numbers[1] = 0;
    for (int i = 0; i < 16; i++)
    {
        sq.digits[i] = i % 10;
    }

    square sq2;
    sq_transpose(sq, sq2);

	sq_print(sq);
	sq_print(sq2);

	sq_mirror_x(sq, sq2);
    sq_print(sq2);

    sq_mirror_y(sq, sq2);
    sq_print(sq2);

	uint64_t packed = sq_pack(sq);
	std::cout << "packed: " << packed << std::endl;

	std::cout << "unpacked: " << std::endl;
	square sq3;
	sq_unpack(packed, sq3);
	sq_print(sq3);


	ql_generate_equals(sq);

    bool eq = sq_equal(sq, sq2);
    std::cout << eq << std::endl;
}

int64_t register_solutions(square& sq, uint32_t TARGETSUM)
{

#ifdef DEBUG_REF
    if (TARGETSUM == 12)
    {
        static bool initialized = false;
        static square ref_sq[8];
        if (!initialized)
        {
            uint8_t ref[16] = { 6, 3, 3, 0, 5, 0, 4, 3, 0, 7, 1, 4, 1, 2, 4, 5 };
            for (int i = 0; i < 16; i++)
            {
                ref_sq[0].digits[i] = ref[i];
            }
            sq_transpose(ref_sq[0], ref_sq[1]);
            sq_mirror_x(ref_sq[0], ref_sq[2]);
            sq_mirror_x(ref_sq[1], ref_sq[3]);
            sq_mirror_y(ref_sq[0], ref_sq[4]);
            sq_mirror_y(ref_sq[1], ref_sq[5]);
            sq_mirror_y(ref_sq[2], ref_sq[6]);
            sq_mirror_y(ref_sq[3], ref_sq[7]);

            initialized = true;
        }

        // check if the square is equal to any of the reference squares
        for (int i = 0; i < 8; i++)
        {
            if (sq_equal(sq, ref_sq[i]))
            {
                // print the square if it matches with reference index
                std::cout << "Found a solution matching reference square " << i << ": " << std::endl;
                sq_print(sq);
            }
        }
    }
#endif

    // check if any digit is out of range (0-9)
    for (int i = 0; i < 16; i++)
    {
        if (sq.digits[i] < 0 || sq.digits[i] > 9)
            return 0;
    }
    
    // check values to be sum of 12 (TARGETSUM) vertically, horizontally and diagonally, using loops
    for (int i = 0; i < 4; i++)
    {
        int sumv = sq.digits[i] + sq.digits[i + 4] + sq.digits[i + 8] + sq.digits[i + 12];
        if (sumv != TARGETSUM)
            return 0;
	}
    for (int i = 0; i < 4; i++)
    {
        int sumh = sq.digits[i * 4] + sq.digits[i * 4 + 1] + sq.digits[i * 4 + 2] + sq.digits[i * 4 + 3];
        if (sumh != TARGETSUM)
			return 0;
	}
	int sumd1 = sq.digits[0] + sq.digits[5] + sq.digits[10] + sq.digits[15];
    if (sumd1 != TARGETSUM)
		return 0;
	int sumd2 = sq.digits[3] + sq.digits[6] + sq.digits[9] + sq.digits[12];
	if (sumd2 != TARGETSUM)
		return 0;

#ifdef DEBUG_MISSING_SYMMETRIES
    uint64_t packed = sq_pack(sq);
    if (solutions.find(packed) == solutions.end())
    {
        solutions.insert(packed);
        return 1; // Found a new solution
    }
    else
    {
        //std::cout << "Duplicate solution found, not counting: " << packed << std::endl;
        return 0; // Duplicate solution, do not count
    }
#endif

    return 1;
}

int64_t solve_quarter(square& sq, uint32_t TARGETSUM)
{

	const int32_t MINTOTARGET = TARGETSUM - 9; // Minimum sum for a valid target

    int64_t solution_count = 0;
    const int pos34 = 11;
    int sumv4 = sq.digits[3] + sq.digits[7];
    for (int digit_34 = 0; digit_34 < 10; digit_34++)
    {
        square sq_test = sq;
        sq_test.digits[pos34] = digit_34;
        if (sumv4 + digit_34 > TARGETSUM)
            continue;
        if (sumv4 + digit_34 < MINTOTARGET)
            continue;

        // forcing value below:
        int d44 = TARGETSUM - sumv4 - digit_34;
        sq_test.digits[pos34 + 4] = d44;
        // forcing diagonal value:
        int sumd1 = sq_test.digits[0] + sq_test.digits[5] + d44;
        if (sumd1 > TARGETSUM)
            continue;
        if (sumd1 < MINTOTARGET)
            continue;
        sq_test.digits[10] = TARGETSUM - sumd1;
        // forcing value (4,2)
        int sumv3 = sq_test.digits[2] + sq_test.digits[6] + sq_test.digits[10];
        if (sumv3 > TARGETSUM)
            continue;
        if (sumv3 < MINTOTARGET)
            continue;
        sq_test.digits[14] = TARGETSUM - sumv3;

        // check remaining sums:
        int sumh3 = sq_test.digits[8] + sq_test.digits[9] + sq_test.digits[10] + sq_test.digits[11];
        int sumh4 = sq_test.digits[12] + sq_test.digits[13] + sq_test.digits[14] + sq_test.digits[15];

        if (sumh3 != TARGETSUM)
            continue;
        if (sumh4 != TARGETSUM)
            continue;

        int64_t reg_count = register_solutions(sq_test, TARGETSUM);
        if (reg_count ==0)
			rejection_count++;
        solution_count += reg_count;
    }
    return solution_count; // Placeholder for the actual solution logic
}

int64_t solve_half(square& sq, const uint32_t TARGETSUM)
{
    int64_t solution_count = 0;
	const int32_t MINTOTARGET = TARGETSUM - 9; // Minimum sum for a valid target

    const int pos31 = 8;
    int sumv1 = sq.digits[0] + sq.digits[4];
    for (int digit_31 = 0; digit_31 < 10; digit_31++)
    {
        square sq_test = sq;
        sq_test.digits[pos31] = digit_31;
        if (sumv1 + digit_31 > TARGETSUM)
            continue;
        if (sumv1 + digit_31 < MINTOTARGET)
            continue;
        // forcing value below:
        int d41 = TARGETSUM - sumv1 - digit_31;
        sq_test.digits[pos31 + 4] = d41;
        // forcing diagonal value:
        int sumd1 = sq_test.digits[3] + sq_test.digits[6] + d41;
        if (sumd1 > TARGETSUM)
            continue;
        if (sumd1 < MINTOTARGET)
            continue;
        sq_test.digits[9] = TARGETSUM - sumd1;
        // forcing value (4,2)
        int sumv2 = sq_test.digits[1] + sq_test.digits[5] + sq_test.digits[9];
        if (sumv2 > TARGETSUM)
            continue;
        if (sumv2 < MINTOTARGET)
            continue;
        sq_test.digits[13] = TARGETSUM - sumv2;

        bool overflow = false;
        if (sq_test.digits[8] + sq_test.digits[9] > TARGETSUM)
            overflow = true;
        if (sq_test.digits[12] + sq_test.digits[13] > TARGETSUM)
            overflow = true;

        if (!overflow)
            solution_count += solve_quarter(sq_test, TARGETSUM);
    }
	return solution_count; // Placeholder for the actual solution logic
}

void find_missing_symmetries()
{
	std::cout << "Finding missing symmetries..." << std::endl;
	int64_t count = 0;
    for (int64_t solution : solutions)
    {
		count++;
        square sq;
        sq_unpack(solution, sq);
        //sq_print(sq);
        // generate all symmetries
        square equals[8];
        equals[0] = sq;
        sq_transpose(equals[0], equals[1]);
        sq_mirror_x(equals[0], equals[2]);
        sq_mirror_x(equals[1], equals[3]);
        sq_mirror_y(equals[0], equals[4]);
        sq_mirror_y(equals[1], equals[5]);
        sq_mirror_y(equals[2], equals[6]);
        sq_mirror_y(equals[3], equals[7]);
        for (int i = 0; i < 8; i++)
        {
            //sq_print(equals[i]);
            uint64_t packed = sq_pack(equals[i]);
            if (solutions.find(packed) == solutions.end())
            {
                //print the square
                std::cout << "Missing symmetry for solution: " << solution << std::endl;
                //std::cout << "Symmetry " << i << ":" << std::endl;
                std::cout << "Missing symmetry found" << i << ": " << packed << std::endl;
                sq_print(equals[i]);
                //solutions.insert(packed);
            }
		}
    }
	std::cout << "Total solutions checked: " << count << std::endl;
}

int64_t solve_n(uint32_t TARGETSUM)
{

    const int32_t MINTOTARGET = TARGETSUM - 9; // Minimum sum for a valid target

    int64_t solution_count = 0;
    square sq;

    for (int seed = 0; seed < 1'000'000; seed++)
    {
        sq.numbers[0] = 0;
        sq.numbers[1] = 0;
        //if (seed==405336)
        //if (seed == 84402)
        //if (seed == 254422)
        //{
        //    std::cout << "sample square" << std::endl;
        //}
        int num = seed;
        sq.digits[0] = num % 10;
        num /= 10;
        sq.digits[1] = num % 10;
        num /= 10;
        sq.digits[2] = num % 10;
        num /= 10;
        sq.digits[4] = num % 10;
        num /= 10;
        sq.digits[5] = num % 10;
        num /= 10;
        sq.digits[6] = num % 10;

        bool overflow = false;

        for (int i = 0; i < 3; i++)
        {
            if (sq.digits[i] + sq.digits[i + 4] > TARGETSUM)
            {
                overflow = true;
                break;
            }
        }
        for (int i = 0; i < 8; i += 4)
        {
            int sum = sq.digits[i] + sq.digits[i + 1] + sq.digits[i + 2];
            if ((sum < MINTOTARGET) || (sum > TARGETSUM))
            {
                overflow = true;
                break;
            }
            sq.digits[i + 3] = TARGETSUM - sum;
        }

        if (sq.digits[3] + sq.digits[7] > TARGETSUM)
            overflow = true;

        if (sq.digits[0] + sq.digits[5] > TARGETSUM)
            overflow = true;
        if (sq.digits[3] + sq.digits[6] > TARGETSUM)
            overflow = true;

        if (!overflow)
        {
            solution_count += solve_half(sq, TARGETSUM);
        }
    }

    //std::cout << "Rejections: " << rejection_count << std::endl;

#ifdef DEBUG_MISSING_SYMMETRIES
    find_missing_symmetries();
#endif

    return solution_count;
}

int64_t solve()
{
    int64_t solution_count = 0;
    
    for (int i = 0; i < 18 ; i++)
    {
		int64_t solution_count_n = solve_n(i);
		std::cout << "Solutions for target sum " << i << " and " << 36-i << ": " << solution_count_n << std::endl;
        solution_count += solution_count_n * 2;
    }
	int64_t solution_count_18 = solve_n(18);
	std::cout << "Solutions for target sum 18: " << solution_count_18 << std::endl;
	solution_count += solution_count_18; // 18 is not doubled, as it is the center püossible sums

    // 7130034
    return solution_count;
}


int main()
{

    auto t1 = std::chrono::high_resolution_clock::now();
    volatile int64_t solution = solve();
    auto t2 = std::chrono::high_resolution_clock::now();
    auto microSec = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
    std::cout << "solution: " << solution << std::endl << "duration: " << microSec << " micro seconds (" << ms << " ms)" << std::endl;
    if (microSec > 300'000'000)
    {
        std::cout << "(" << ((float)microSec) / 60'000'000 << " minutes )" << std::endl;
    }
}
